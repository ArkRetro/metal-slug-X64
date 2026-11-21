#!/usr/bin/env python3
"""Sizes and repeated data of the private packs (docs/COMPRESION.md).

Reads romfs/scene.pack (Mission 1, chunk table in build/scene_meta.h),
romfs/missionN.pack (any version pack_mission.py wrote; a compressed pack is
measured through tools/pack_store.py's decoder), build/bgp/*.bgp and the
wav64 files; prints the size of each section, the RAM each stage's tables
take when loaded, and what is repeated (SHA-1 of pages, 16x16 tiles,
palettes, programs, blobs, grids, wav64). Nothing is written.

usage: measure_packs.py [--root DIR] [--romfs DIR]   (JSON on stdout)
"""
import argparse,hashlib,json,re,struct,sys,zlib
from collections import Counter,defaultdict
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from msxc_compress import lz4_block
import pack_store
PAGE=2048;CHUNK=16384
def h(b):return hashlib.sha1(b).digest()
def unrle(d):return b''.join(bytes([d[i+1]])*d[i] for i in range(0,len(d),2))

def parse_mission(path):
    """Sections (name, offset, length) and tables of a mission pack (versions 3..5)."""
    mp=pack_store.MissionPack(path);blob=mp.raw
    sec=[('header',0,mp.header_bytes),('stages',mp.header_bytes,128*mp.stages),('cells',mp.cells_at,22*mp.cells),('images',mp.images_at,8*mp.images),
         ('palettes',mp.palettes_at,32*mp.palettes),('chunk_table',mp.chunks_at,16*len(mp.chunks)),('actors',mp.actors_at,8*mp.actors),
         ('actor_images',mp.actor_images_at,12*mp.actor_images),('resources',mp.resources_at,4*mp.resources),('grids',mp.grids_at,mp.grid_bytes)]
    own=[c for c in mp.chunks if not c[3]>>8]
    body_at=min(c[0] for c in own) if own else len(blob)
    o=(mp.grids_at+mp.grid_bytes+3)&~3
    sec.append(('stage_blobs',o,(mp.data_offset if mp.data_length else body_at)-o))
    if mp.data_length:sec.append(('msxd',mp.data_offset,mp.data_length))
    sec.append(('chunks',body_at,len(blob)-body_at))
    return dict(pack=mp,blob=blob,mission=mp.mission,sections=sec,chunks=mp.chunks,stages=mp.stages,version=mp.version,
                counts=dict(palettes=mp.palettes,images=mp.images,cells=mp.cells,actors=mp.actors,actor_images=mp.actor_images,resources=mp.resources))
def decode_chunks(blob,ch):
    return [pack_store.decode(blob[off:off+ln],crc,codec) for off,ln,crc,codec in ch]
def scene_chunks(root,meta_path=None):
    meta=(meta_path or root/'build/scene_meta.h').read_text()
    body=re.search(r'scene_chunks\[[^\]]*\]\s*=\s*\{(.*?)\};',meta,re.S).group(1)
    return [tuple(int(x,0) for x in m) for m in re.findall(r'\{\s*(\w+)u?,\s*(\w+)u?,\s*(\w+)u?,\s*(\w+)u?\s*\}',body.replace('u,',',').replace('u}','}'))]
def stage_ram(p,k):
    """Bytes scene_stage_load takes in the stage's slots (its NEED list)."""
    b=p['blob'];hb=p['pack'].header_bytes;st=struct.unpack_from('>IHHIIIHHhhIIII',b,hb+128*k);blobs=struct.unpack_from('>9I',b,hb+128*k+44)
    cells,images,grid=st[5],st[13],st[6]*st[7];c=p['counts'];r=lambda x:(x+15)&~15
    t=dict(cells=r(cells*8),cells_before=r(cells*22),images=r(images*12),palettes=r(32*c['palettes']),chunk_table=r(16*len(p['chunks'])),grid=r(grid),grid_dynamic=r(grid),
           ring=4096 if st[7]>32 else 0,unknown=r((grid+7)//8) if st[11] else 0,index_order=r(cells*2),index_runs=r(cells*8),strips=r(2048*8)+r(512*8),
           stage_blobs=sum(r(blobs[3*i+2]) for i in range(3)),actors=r(8*c['actors']),actor_images=r(12*c['actor_images']),resources=r(4*c['resources']))
    data=next((s for s in p['sections'] if s[0]=='msxd'),None)
    if data:
        programs=struct.unpack_from('>H',b,data[1]+8)[0];t['msxd']=r(data[2]);t['program_table']=r(programs*12)
    return t
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1]);ap.add_argument('--romfs',type=Path,help='directory of the packs (default ROOT/romfs)')
    a=ap.parse_args();root=a.root;out={};romfs=a.romfs or root/'romfs'
    packs={};pages={}   # name -> list of 2048-byte pages
    sp=romfs/'scene.pack'
    if sp.exists():
        blob=sp.read_bytes();ch=scene_chunks(root,romfs/'scene_meta.h' if (romfs/'scene_meta.h').exists() else None);dec=decode_chunks(blob,ch)
        pages['scene']=[d[i:i+PAGE] for d in dec for i in range(0,CHUNK,PAGE)]
        out['scene.pack']=dict(bytes=len(blob),chunks=len(ch),codecs=dict(Counter(('raw','run pairs','lz4')[c[3]] for c in ch)),decoded=len(ch)*CHUNK)
    for f in sorted(romfs.glob('mission[2-6].pack')):
        p=parse_mission(f);raw=p['blob'];packs[f.name]=p;dec=[p['pack'].chunk(i) for i in range(len(p['chunks']))]
        pages[f.stem]=[d[i:i+PAGE] for d in dec for i in range(0,CHUNK,PAGE)]
        out[f.name]=dict(bytes=len(raw),version=p['version'],sections={n:l for n,_,l in p['sections']},chunks=len(p['chunks']),
            codecs=dict(Counter(('raw','run pairs','lz4')[c[3]&255] for c in p['chunks'] if not c[3]>>8)),held_by_other_packs=dict(Counter(c[3]>>8 for c in p['chunks'] if c[3]>>8)),
            stage_ram={f's{k+1}':stage_ram(p,k) for k in range(p['stages'])},
            tables_zlib=len(zlib.compress(p['blob'][:p['sections'][-1][1]],9)),tables_lz4=len(lz4_block(p['blob'][:p['sections'][-1][1]])))
    # Compression the chunks would still take (per 16 KiB chunk, so the cache can decode any one alone).
    for name,pg in pages.items():
        chunks=[b''.join(pg[i:i+8]) for i in range(0,len(pg),8)]
        key='scene.pack' if name=='scene' else name+'.pack'
        out[key]['chunks_stored']=sum(min(CHUNK,len(rle_bytes(c))) for c in chunks)
        out[key]['chunks_lz4_each']=sum(min(CHUNK,len(lz4_block(c))) for c in chunks)
        out[key]['chunks_zlib_each']=sum(len(zlib.compress(c,9)) for c in chunks)
    # Repeated data.
    dup={}
    allp=Counter();where=defaultdict(set)
    for name,pg in pages.items():
        c=Counter(h(x) for x in pg);allp.update(c.keys())
        for k in c:where[k].add(name)
        zero=h(bytes(PAGE))
        dup[name+' pages']=dict(total=len(pg),distinct=len(c),repeated_bytes=(len(pg)-len(c))*PAGE,empty=c.get(zero,0))
    dup['pages shared between packs']=dict(distinct_shared=sum(1 for k,v in where.items() if len(v)>1 and k!=h(bytes(PAGE))),bytes=sum((len(v)-1)*PAGE for k,v in where.items() if len(v)>1 and k!=h(bytes(PAGE))))
    # 16x16 tiles of the pages (8 bytes x 16 rows at 4 bpp, 16 per page).
    tiles=Counter();tw=defaultdict(set)
    for name,pg in pages.items():
        for x in pg:
            for ty in range(4):
                for tx in range(4):
                    t=b''.join(x[(ty*16+y)*32+tx*8:(ty*16+y)*32+tx*8+8] for y in range(16))
                    k=h(t);tiles[k]+=1;tw[k].add(name)
    total=sum(tiles.values());zt=h(bytes(128))
    dup['tiles 16x16 (aligned)']=dict(total=total,distinct=len(tiles),empty=tiles.get(zt,0),repeated_bytes=(total-len(tiles))*128,
                                      shared_between_packs=sum(1 for k,v in tw.items() if len(v)>1 and k!=zt))
    # Tables of the mission packs.
    pal=Counter();palw=defaultdict(set);prog=Counter();progw=defaultdict(set);blobc=Counter();cells=Counter();imgs=Counter();grid=Counter()
    for name,p in packs.items():
        b=p['blob'];s={n:(o,l) for n,o,l in p['sections']}
        o,l=s['palettes']
        for i in range(0,l,32):k=b[o+i:o+i+32];pal[k]+=1;palw[k].add(name)
        o,l=s['images']
        for i in range(0,l,8):imgs[(name,b[o+i:o+i+8])]+=1
        o,l=s['cells']
        for i in range(0,l,22):cells[(name,b[o+i:o+i+22])]+=1
        if 'msxd' in s:
            o,l=s['msxd'];nb,np_=struct.unpack_from('>HH',b,o+6)
            for i in range(nb):
                rom,at,n=struct.unpack_from('>III',b,o+12+12*i);blobc[b[o+at:o+at+n]]+=1
            for i in range(np_):
                tag,at,n=struct.unpack_from('>IIH',b,o+12+12*nb+12*i);k=b[o+at:o+at+10*n];prog[k]+=1;progw[k].add(name)
        for k in range(p['stages']):
            st=struct.unpack_from('>IHHIIIHHhhIIII',b,p['pack'].header_bytes+128*k);g=b[st[10]:st[10]+st[6]*st[7]];grid[h(g)]+=1
            # 8x8 blocks of the grid (column major): how much of it repeats.
    rep=lambda c,size=None:dict(total=sum(c.values()),distinct=len(c),repeated_bytes=sum((v-1)*(len(k) if size is None else size) for k,v in c.items()))
    dup['palettes (mission packs)']=dict(rep(pal),shared_between_packs=sum(1 for v in palw.values() if len(v)>1))
    dup['image records']=rep(Counter({k[1]+k[0].encode():v for k,v in imgs.items()}),8)
    dup['cell records']=rep(Counter({k[1]+k[0].encode():v for k,v in cells.items()}),22)
    dup['MSXD programs']=dict(rep(prog),shared_between_packs=sum(1 for v in progw.values() if len(v)>1))
    dup['MSXD blobs']=rep(blobc)
    dup['collision grids']=dict(total=sum(grid.values()),distinct=len(grid))
    # scene.pack palettes against the missions'.
    meta=root/'build/scene_meta.h'
    if meta.exists():
        m=re.search(r'scene_palettes\[[^\]]*\]\[16\]\s*=\s*\{(.*?)\};',meta.read_text(),re.S)
        if m:
            rows=re.findall(r'\{([^{}]*)\}',m.group(1));sc={struct.pack('>16H',*[int(x,0) for x in r.replace('u','').split(',') if x.strip()]) for r in rows}
            dup['palettes (mission packs)']['also_in_scene_pack']=sum(1 for k in pal if k in sc);dup['scene.pack palettes']=dict(total=len(rows),distinct=len(sc))
    out['repeated']=dup
    # Audio and the rest of the cartridge file system.
    wav=sorted((root/'filesystem/audio').glob('*.wav64'))+sorted((root/'filesystem/cinematics').glob('*.wav64'))+sorted((root/'filesystem/logo').glob('*.wav64'))
    data={f:f.read_bytes() for f in wav};byh=defaultdict(list)
    for f,d in data.items():byh[h(d)].append(f.name)
    body=lambda d:d[struct.unpack_from('>I',d,16)[0] if len(d)>20 else 0:]
    contained=[]
    big=sorted(data.items(),key=lambda t:-len(t[1]))
    for f,d in data.items():
        tail=d[64:] if len(d)>128 else d
        for g,e in big:
            if g!=f and len(e)>len(d) and tail in e:contained.append((f.name,g.name,len(d)));break
    out['audio']=dict(files=len(wav),bytes=sum(len(d) for d in data.values()),game_audio_bytes=sum(len(d) for f,d in data.items() if f.parent.name=='audio'),
        identical_groups=[v for v in byh.values() if len(v)>1],identical_bytes=sum(len(data[f])*(len(v)-1) for v in byh.values() if len(v)>1 for f in [next(x for x in data if x.name==v[0])]),
        contained=contained,largest=[(f.name,len(d)) for f,d in big[:8]])
    fs=root/'build/game-romfs'
    if fs.exists():
        groups=Counter()
        for f in fs.rglob('*'):
            if f.is_file():
                rel=f.relative_to(fs);g=rel.parts[0] if len(rel.parts)>1 else ('*.'+f.suffix[1:] if f.suffix in ('.wav64','.msxc','.bgp','.pack') else f.name)
                groups[g]+=f.stat().st_size
        out['game-romfs']=dict(sorted(groups.items(),key=lambda t:-t[1]),total=sum(groups.values()))
        bg=sorted(fs.glob('*.bgp'))
        if bg:out['bgp']={f.name:dict(bytes=f.stat().st_size,zlib=len(zlib.compress(f.read_bytes(),9))) for f in bg}
    print(json.dumps(out,indent=1,default=str))
def rle_bytes(data):
    out=0;i=0;n=len(data)
    while i<n:
        j=i+1
        while j<n and j-i<255 and data[j]==data[i]:j+=1
        out+=2;i=j
    return b'x'*out
if __name__=='__main__':main()
