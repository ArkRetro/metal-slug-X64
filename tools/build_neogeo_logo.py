#!/usr/bin/env python3
"""Build the native 3D NEOGEO boot logo and its jingle from a local eyecatch capture.

Input: a capture made by tools/mame/run_neogeo_eyecatch.py (the BIOS eyecatch
of the user's own BIOS/cartridge) plus filesystem/sprites.bin (C-ROM tiles,
tools/import_rom.py) and the cartridge zip (header byte 0x115 = tile bank).

1. The flat NEOGEO letters are rebuilt from the C-ROM tiles exactly where the
   BIOS placed them (SCB1..SCB4 of the VRAM dump) and compared pixel by pixel
   with MAME's own screenshot of that frame.
2. The anti-aliased pens become a coverage field; marching squares at 50 %
   coverage gives the letter outlines (with the holes of O), simplified with
   Douglas-Peucker and triangulated by ear clipping. The silhouette is extruded
   into a closed mesh: front and back caps plus side walls with smoothed
   normals on curves and hard normals on corners. The mesh must be a closed,
   consistently wound 2-manifold.
3. The jingle is cut from the capture's PCM, from the BIOS sound command $02
   (play eyecatch music) to the end of its tail, and converted to WAV64.

Outputs (ROM-derived, outside Git): filesystem/logo/neogeo-logo.mesh,
filesystem/logo/neogeo-jingle.wav64, filesystem/logo/neogeo-logo.json, and
build/logo-preview/*.png for review. See docs/LOGO_NEOGEO.md."""
import argparse,hashlib,json,math,os,re,struct,subprocess,wave,zipfile
from pathlib import Path
import numpy as np
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
# Coverage of the eyecatch pens: the BIOS palette at the flat-logo frame is
# 0 transparent, 1 0xFFF, 2 0xDDD, 3 0xAAA, 4 0x555 (white letters with three
# anti-aliasing steps). The palette dump is checked against this below.
EXPECTED_PALETTE=[0x0000,0x0FFF,0x0DDD,0x0AAA,0x7555]

def neo_rgb(c):
    dark=(c>>15)&1
    def ch(hi,lo):return ((hi<<2)|(lo<<1)|(dark^1))*255//63
    return (ch((c>>8)&15,(c>>14)&1),ch((c>>4)&15,(c>>13)&1),ch(c&15,(c>>12)&1))

def tile_pens(sprites,tile):
    d=sprites[tile*128:tile*128+128];out=np.zeros((16,16),dtype=np.uint8)
    for y in range(16):
        for x in range(16):
            row=d[(64 if x<8 else 0)+y*4:];b=x&7
            out[y,x]=((row[0]>>b)&1)|(((row[2]>>b)&1)<<1)|(((row[1]>>b)&1)<<2)|(((row[3]>>b)&1)<<3)
    return out

def logo_layout(vram,bank):
    """Columns of the unflipped logo copy: (screen x, screen y of row 0, [tiles])."""
    cols=[];x=y=0
    for n in range(1,382):
        s2,s3,s4=vram[0x8000+n],vram[0x8200+n],vram[0x8400+n]
        if s3&0x40:x+=((s2>>8)&15)+1
        else:x,y=s4>>7,496-(s3>>7)
        tiles=[]
        for i in range(32):
            lo,attr=vram[n*64+2*i],vram[n*64+2*i+1]
            tiles.append((lo|((attr>>4)&15)<<16,attr))
        own=[(i,t,a) for i,(t,a) in enumerate(tiles) if t>>8==bank]
        if own and not any(a&3 for _,_,a in own):
            assert (s2&0xfff)==0xfff,'Logo column is zoomed at the chosen frame'
            cols.append((x,y,[t if t>>8==bank else None for t,_ in tiles[:4]],own[0][2]>>8))
    assert len(cols)>=10,'Logo columns not found in the VRAM dump'
    return cols

def render_logo(sprites,cols):
    x0=min(c[0] for c in cols);w=max(c[0] for c in cols)+16-x0
    pens=np.zeros((64,w),dtype=np.uint8)
    for x,_,tiles,_ in cols:
        for r,t in enumerate(tiles):
            if t is not None:pens[r*16:r*16+16,x-x0:x-x0+16]=tile_pens(sprites,t)
    return pens,x0

def marching_squares(v,iso):
    h,w=v.shape;seg=[]
    def pt(edge):
        k,x,y=edge
        if k=='h':a,b=v[y,x],v[y,x+1];t=(iso-a)/(b-a);return (x+t,float(y))
        a,b=v[y,x],v[y+1,x];t=(iso-a)/(b-a);return (float(x),y+t)
    for y in range(h-1):
        for x in range(w-1):
            tl,tr,br,bl=v[y,x]>=iso,v[y,x+1]>=iso,v[y+1,x+1]>=iso,v[y+1,x]>=iso
            c=tl|tr<<1|br<<2|bl<<3
            if c in (0,15):continue
            T,B,L,R=('h',x,y),('h',x,y+1),('v',x,y),('v',x+1,y)
            centre=(v[y,x]+v[y,x+1]+v[y+1,x+1]+v[y+1,x])/4>=iso
            table={1:[(L,T)],2:[(T,R)],3:[(L,R)],4:[(R,B)],6:[(T,B)],7:[(L,B)],8:[(B,L)],9:[(T,B)],
                   11:[(R,B)],12:[(L,R)],13:[(T,R)],14:[(L,T)],
                   5:[(T,R),(B,L)] if centre else [(L,T),(R,B)],10:[(L,T),(R,B)] if centre else [(T,R),(B,L)]}
            seg+=table[c]
    links={}
    for a,b in seg:links.setdefault(a,[]).append(b);links.setdefault(b,[]).append(a)
    assert all(len(n)==2 for n in links.values()),'Open contour'
    loops=[];seen=set()
    for start in links:
        if start in seen:continue
        loop=[start];seen.add(start);prev,cur=None,start
        while True:
            a,b=links[cur];nxt=b if a==prev else a
            if nxt==start:break
            loop.append(nxt);seen.add(nxt);prev,cur=cur,nxt
        loops.append([pt(e) for e in loop])
    return loops

def area(p):return sum(p[i-1][0]*p[i][1]-p[i][0]*p[i-1][1] for i in range(len(p)))/2

def simplify(loop,tol):
    n=len(loop);far=max(range(n),key=lambda i:(loop[i][0]-loop[0][0])**2+(loop[i][1]-loop[0][1])**2)
    def dp(pts):
        if len(pts)<3:return pts
        (ax,ay),(bx,by)=pts[0],pts[-1];dx,dy=bx-ax,by-ay;L=math.hypot(dx,dy) or 1e-9
        d=[abs((p[0]-ax)*dy-(p[1]-ay)*dx)/L for p in pts[1:-1]];i=int(np.argmax(d))+1
        if d[i-1]<=tol:return [pts[0],pts[-1]]
        return dp(pts[:i+1])[:-1]+dp(pts[i:])
    a=dp(loop[:far+1]);b=dp(loop[far:]+[loop[0]])
    return a[:-1]+b[:-1]

def inside(pt,poly):
    x,y=pt;c=False
    for i in range(len(poly)):
        (x1,y1),(x2,y2)=poly[i-1],poly[i]
        if (y1>y)!=(y2>y) and x<(x2-x1)*(y-y1)/(y2-y1)+x1:c=not c
    return c

def cross(o,a,b):return (a[0]-o[0])*(b[1]-o[1])-(a[1]-o[1])*(b[0]-o[0])

def bridge(outer,hole):
    """Merge a hole (CW) into the outer polygon (CCW), Eberly's visible-vertex method."""
    mi=max(range(len(hole)),key=lambda i:hole[i][0]);M=hole[mi];best=None
    for i in range(len(outer)):
        a,b=outer[i],outer[(i+1)%len(outer)]
        if (a[1]>M[1])==(b[1]>M[1]) and a[1]!=M[1] and b[1]!=M[1]:continue
        if a[1]==b[1]:continue
        t=(M[1]-a[1])/(b[1]-a[1])
        if not 0<=t<=1:continue
        ix=a[0]+t*(b[0]-a[0])
        if ix>=M[0] and (best is None or ix<best[0]):best=(ix,i)
    assert best is not None,'Hole without visible outer edge'
    ix,i=best;I=(ix,M[1]);j=i if outer[i][0]>outer[(i+1)%len(outer)][0] else (i+1)%len(outer)
    P=outer[j];cand=j;bestang=None
    for k,q in enumerate(outer):
        if k==j:continue
        prev,nxt=outer[k-1],outer[(k+1)%len(outer)]
        if cross(prev,q,nxt)>0:continue
        if tri_contains((M,I,P),q):
            ang=abs(math.atan2(q[1]-M[1],q[0]-M[0]))
            if bestang is None or ang<bestang:bestang,cand=ang,k
    ring=hole[mi:]+hole[:mi]
    return outer[:cand+1]+ring+[ring[0],outer[cand]]+outer[cand+1:]

def tri_contains(t,p):
    a,b,c=t
    if p in (a,b,c):return False
    d1,d2,d3=cross(a,b,p),cross(b,c,p),cross(c,a,p)
    neg=(d1<0) or (d2<0) or (d3<0);pos=(d1>0) or (d2>0) or (d3>0)
    return not (neg and pos)

def earclip(poly):
    idx=list(range(len(poly)));tris=[];guard=0
    while len(idx)>3:
        guard+=1;assert guard<100000,'Ear clipping did not converge'
        found=False
        for k in range(len(idx)):
            i0,i1,i2=idx[k-1],idx[k],idx[(k+1)%len(idx)]
            a,b,c=poly[i0],poly[i1],poly[i2]
            if cross(a,b,c)<=1e-9:continue
            if any(tri_contains((a,b,c),poly[j]) for j in idx if j not in (i0,i1,i2)):continue
            tris.append((i0,i1,i2));del idx[k];found=True;break
        if not found:
            # Remove a degenerate (collinear) vertex left by simplification or a bridge.
            for k in range(len(idx)):
                if abs(cross(poly[idx[k-1]],poly[idx[k]],poly[idx[(k+1)%len(idx)]]))<=1e-9:del idx[k];found=True;break
        assert found,('No ear found',len(idx))
    tris.append(tuple(idx));return tris

def extrude(polys,depth,smooth_deg):
    """polys: list of (outer CCW, [holes CW]) in model units (y up)."""
    verts=[];tris=[];cap_points=[];cap_index={};cap_tris=[];side=[]
    def add(p,n):verts.append((p[0],p[1],p[2],n[0],n[1],n[2]));return len(verts)-1
    def cap(p):
        if p not in cap_index:cap_index[p]=len(cap_points);cap_points.append(p)
        return cap_index[p]
    zf,zb=depth/2,-depth/2
    for outer,holes in polys:
        merged=outer
        for h in sorted(holes,key=lambda h:-max(p[0] for p in h)):merged=bridge(merged,h)
        for a,b,c in earclip(merged):
            pa,pb,pc=merged[a],merged[b],merged[c]
            cap_tris.append((cap(pa),cap(pb),cap(pc)))
            f=[add((p[0],p[1],zf),(0,0,1)) for p in (pa,pb,pc)];tris.append(tuple(f))
            k=[add((p[0],p[1],zb),(0,0,-1)) for p in (pa,pc,pb)];tris.append(tuple(k))
        for loop in [outer]+holes:
            n=len(loop);en=[]
            for i in range(n):
                a,b=loop[i],loop[(i+1)%n];dx,dy=b[0]-a[0],b[1]-a[1];L=math.hypot(dx,dy)
                en.append((dy/L,-dx/L))
            cos_lim=math.cos(math.radians(smooth_deg))
            def vnorm(i,edge):
                # Normal of loop vertex i as used by edge `edge` (i-1 -> i or i -> i+1).
                a,b=en[i-1],en[i]
                if a[0]*b[0]+a[1]*b[1]>=cos_lim:
                    s=(a[0]+b[0],a[1]+b[1]);L=math.hypot(*s);return (s[0]/L,s[1]/L,0)
                e=en[edge];return (e[0],e[1],0)
            for i in range(n):
                j=(i+1)%n;a,b=loop[i],loop[j]
                na,nb=vnorm(i,i),vnorm(j,i)
                fn=en[i];mx,my=(a[0]+b[0])/2,(a[1]+b[1])/2
                side.append((a[0],a[1],b[0],b[1],na[0],na[1],nb[0],nb[1],fn[0],-(fn[0]*mx+fn[1]*my)))
                af,ab=add((a[0],a[1],zf),na),add((a[0],a[1],zb),na)
                bf,bb=add((b[0],b[1],zf),nb),add((b[0],b[1],zb),nb)
                tris+=[(af,ab,bb),(af,bb,bf)]
    # Orient every triangle along its intended normal (outward).
    out=[]
    for t in tris:
        p=[np.array(verts[i][:3]) for i in t];g=np.cross(p[1]-p[0],p[2]-p[0])
        n=sum(np.array(verts[i][3:]) for i in t)
        out.append(t if g.dot(n)>=0 else (t[0],t[2],t[1]))
    # Share identical vertices (same position and normal).
    index={};uniq=[];remap=[]
    for v in verts:
        k=tuple(round(c,5) for c in v)
        if k not in index:index[k]=len(uniq);uniq.append(v)
        remap.append(index[k])
    return uniq,[tuple(remap[i] for i in t) for t in out],(cap_points,cap_tris,side)

def check_closed(verts,tris):
    key=lambda i:tuple(round(c,4) for c in verts[i][:3]);edges={}
    for t in tris:
        for a,b in ((t[0],t[1]),(t[1],t[2]),(t[2],t[0])):
            edges[(key(a),key(b))]=edges.get((key(a),key(b)),0)+1
    bad=[e for e,c in edges.items() if c!=1 or edges.get((e[1],e[0]),0)!=1]
    assert not bad,('Mesh not closed or inconsistently wound',len(bad),bad[:3])
    return len(edges)//2

def preview(verts,tris,path,yaw):
    """Offline flat-shaded orthographic preview (review only)."""
    W,H=480,200;img=np.zeros((H,W,3),dtype=np.uint8);zb=np.full((H,W),-1e9)
    c,s=math.cos(math.radians(yaw)),math.sin(math.radians(yaw));sc=W*0.9/max(abs(v[0]) for v in verts)/2
    P=[(v[0]*c+v[2]*s,v[1],-v[0]*s+v[2]*c) for v in verts];N=[(v[3]*c+v[5]*s,v[4],-v[3]*s+v[5]*c) for v in verts]
    for t in tris:
        q=[(W/2+P[i][0]*sc,H/2-P[i][1]*sc,P[i][2]) for i in t]
        if cross(q[0],q[2],q[1])<=0:continue
        n=np.mean([N[i] for i in t],0);l=max(0.15,np.dot(n,(0.3,0.5,0.81)))
        xs,ys=[p[0] for p in q],[p[1] for p in q]
        for y in range(max(0,int(min(ys))),min(H,int(max(ys))+1)):
            for x in range(max(0,int(min(xs))),min(W,int(max(xs))+1)):
                p=(x+.5,y+.5);d=[cross(q[k],q[(k+1)%3],p) for k in range(3)]
                if all(v<=0 for v in d) or all(v>=0 for v in d):
                    tot=sum(d) or 1e-9;z=sum(q[(k+2)%3][2]*d[k] for k in range(3))/tot
                    if z>zb[y,x]:zb[y,x]=z;img[y,x]=(int(255*l),int(200*l),int(80*l))
    Image.fromarray(img).save(path)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('capture',type=Path,help='tools/mame/run_neogeo_eyecatch.py output')
    p.add_argument('--rom-dir',type=Path,required=True,help='Directory with mslugx.zip')
    p.add_argument('--frame',type=int,default=400,help='Dumped frame with the flat logo')
    p.add_argument('--tolerance',type=float,default=0.45,help='Outline simplification (pixels)')
    p.add_argument('--depth',type=float,default=14.0,help='Extrusion depth (pixels)')
    p.add_argument('--gain',type=float,default=4.0,help='Jingle gain (the capture plays it far below the game music)')
    p.add_argument('--no-preview',action='store_true')
    a=p.parse_args();cap=a.capture
    with zipfile.ZipFile(a.rom_dir/'mslugx.zip') as z:p1=z.read('250-p1.p1')
    # P1 is stored word-swapped: 68000 byte 0x114 is file byte 0x115.
    flag,bank=p1[0x115],p1[0x114]
    sprites=(ROOT/'filesystem/sprites.bin').read_bytes()
    vram=struct.unpack('>34304H',(cap/f'vram{a.frame}.bin').read_bytes())
    pal=struct.unpack('>4096H',(cap/f'palette{a.frame}.bin').read_bytes())
    cols=logo_layout(vram,bank);palette=cols[0][3]
    assert all(c[3]==palette for c in cols)
    assert list(pal[palette*16:palette*16+5])==EXPECTED_PALETTE,[hex(v) for v in pal[palette*16:palette*16+5]]
    pens,x0=render_logo(sprites,cols);y0=cols[0][1]
    # Pixel comparison with MAME's screenshot of the same frame.
    shot=np.asarray(Image.open(cap/'snap'/f'frame{a.frame:04d}.png').convert('RGB')).astype(int)
    colours=np.array([neo_rgb(c) for c in pal[palette*16:palette*16+16]])
    best=None
    for dy in range(-32,33):
        top=y0+dy
        if top<0 or top+64>shot.shape[0]:continue
        region=shot[top:top+64,x0:x0+pens.shape[1]];mask=pens>0
        diff=np.abs(region[mask]-colours[pens[mask]]).max() if mask.any() else 999
        if best is None or diff<best[0]:best=(int(diff),dy)
    assert best and best[0]<=8,('Logo does not match the MAME screenshot',best)
    cov=np.array([0,1,0xD/15,0xA/15,5/15]+[0]*11)[pens]
    cov=np.pad(cov,2)
    loops=[l for l in marching_squares(cov,0.5) if abs(area(l))>1.0]
    raw=sum(len(l) for l in loops)
    loops=[simplify(l,a.tolerance) for l in loops]
    ys=[q[1] for l in loops for q in l];xs=[q[0] for l in loops for q in l]
    cx,cy=(min(xs)+max(xs))/2,(min(ys)+max(ys))/2
    model=[[(q[0]-cx,-(q[1]-cy)) for q in l] for l in loops]
    depth=[sum(inside(l[0],m) for m in model if m is not l) for l in model]
    polys=[]
    for l,d in zip(model,depth):
        if d%2:continue
        if area(l)<0:l=l[::-1]
        holes=[h if area(h)<0 else h[::-1] for h,dh in zip(model,depth) if dh==d+1 and inside(h[0],l)]
        polys.append((l,holes))
    verts,tris,(cap_points,cap_tris,side)=extrude(polys,a.depth,35)
    edges=check_closed(verts,tris)
    out=ROOT/'filesystem/logo';out.mkdir(parents=True,exist_ok=True)
    width,height=max(xs)-min(xs),max(ys)-min(ys)
    # Runtime layout (version 2): cap outline points (x, y), side edges
    # (a, b, smoothed normals at a and b, flat normal x and the visibility
    # constant -(n . midpoint)), then cap triangles (front winding). The
    # renderer extrudes them itself; the expanded mesh above is what passed
    # the closed-manifold check.
    blob=struct.pack('>4sHHHHfff',b'NGLM',2,len(cap_points),len(side),len(cap_tris),width,height,a.depth)
    blob+=b''.join(struct.pack('>2f',*q) for q in cap_points)+b''.join(struct.pack('>10f',*e) for e in side)
    blob+=b''.join(struct.pack('>3H',*t) for t in cap_tris)
    (out/'neogeo-logo.mesh').write_bytes(blob)
    # Jingle: from the BIOS command $02 (play eyecatch music) to the end of the tail.
    text=(cap/'debug.log').read_text()
    start_frame=int(re.search(r'AUDIO_COMMAND frame=(\d+) pc=[0-9A-F]+ value=02',text)[1])
    times={}
    for line in (cap/'frames.jsonl').read_text().splitlines():
        r=json.loads(line)
        if 'frame' in r:
            s,frac=r['time'].split('.',1);times[r['frame']]=int(s)+float('0.'+frac.replace(',',''))
    with wave.open(str(cap/'reference.wav')) as w:
        rate=w.getframerate();pcm=np.frombuffer(w.readframes(w.getnframes()),dtype=np.int16).reshape(-1,2)
    # The command is written during frame `start_frame`; its PCM begins at that frame's start.
    first=int(times[start_frame-1]*rate)
    level=np.abs(pcm.astype(int)).max(1);loud=np.nonzero(level[first:]>64)[0]
    onset=first+int(loud[0]);gap=np.diff(loud);stop=np.nonzero(gap>rate//4)[0]
    end=first+int(loud[stop[0]] if len(stop) else loud[-1])+rate//20
    clip=pcm[first:end].astype(np.float64)*a.gain
    peak=np.abs(clip).max();assert peak<32767,('Jingle gain clips',peak)
    clip=clip.astype(np.int16);fade=min(len(clip),rate//50)
    clip[-fade:]=(clip[-fade:]*np.linspace(1,0,fade)[:,None]).astype(np.int16)
    with wave.open(str(out/'neogeo-jingle.wav'),'wb') as w:
        w.setnchannels(2);w.setsampwidth(2);w.setframerate(rate);w.writeframes(clip.tobytes())
    conv=os.path.join(os.environ.get('N64_INST','/workspace/toolchains/package/opt/libdragon'),'bin/audioconv64')
    subprocess.run([conv,'--wav-resample','32000','--wav-compress','0','--wav-loop','false','-o',str(out),str(out/'neogeo-jingle.wav')],check=True,stdout=subprocess.DEVNULL)
    sha=lambda q:hashlib.sha256(q.read_bytes()).hexdigest()
    meta=dict(source='Neo Geo BIOS eyecatch (euro BIOS, MAME capture, header byte 0x114 cleared in emulation)',
              cartridge_eyecatch_flag=flag,tile_bank=bank,palette=palette,layout_columns=len(cols),
              tiles=[t for c in cols for t in c[2] if t is not None],bitmap=[int(pens.shape[1]),64],
              screenshot_match=dict(max_rgb_difference=best[0],row_offset=best[1]),
              outline=dict(loops=len(loops),raw_points=raw,points=sum(len(l) for l in loops),tolerance=a.tolerance),
              mesh=dict(vertices=len(verts),triangles=len(tris),cap_points=len(cap_points),cap_triangles=len(cap_tris),side_edges=len(side),closed_edges=edges,width=width,height=height,depth=a.depth),
              jingle=dict(command_frame=start_frame,first_sample=first,onset_sample=onset,end_sample=end,rate=rate,
                          seconds=round((end-first)/rate,3),gain=a.gain,peak=int(peak)),
              mesh_sha256=sha(out/'neogeo-logo.mesh'),wav64_sha256=sha(out/'neogeo-jingle.wav64'),
              capture_audio_sha256=sha(cap/'reference.wav'))
    (out/'neogeo-logo.json').write_text(json.dumps(meta,indent=2)+'\n')
    if not a.no_preview:
        pv=ROOT/'build/logo-preview';pv.mkdir(parents=True,exist_ok=True)
        Image.fromarray((cov*255).astype(np.uint8)).resize((cov.shape[1]*3,cov.shape[0]*3),Image.NEAREST).save(pv/'coverage.png')
        for yaw in (0,35,70):preview(verts,tris,pv/f'mesh-yaw{yaw}.png',yaw)
    print(json.dumps({k:meta[k] for k in ('outline','mesh','jingle','screenshot_match')},indent=1))

if __name__=='__main__':main()
