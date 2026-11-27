#!/usr/bin/env python3
"""Adds actor frames to the mission packs already on the cartridge, without
their stage sources (B82, docs/PERSONAJES.md "Pilotos del Slugnoid").

The packs of a cartridge depend on each other (version 5: chunks and 'MSXD'
data taken from a lower mission's pack, whose size and CRC each pack's
trailer names), and their sources (filesystem/missionN/sK, rest-native.json)
can be missing while the packs themselves are at hand (the user's ROM 52).
This rewrites the packs of a directory in mission order: each pack named in
--add gets the frames of its exports (tools/export_rest_sprites.py output,
validated against MAME) that it does not have yet, with the palette
resources it lacks, exactly as pack_mission.py lays them out (64x64 CI4
parts on new pages, new chunks after its own, palette 0 overridden by the
resource; the descriptors only one table 09649C[1..3] poses go to that
character's block, version 6 with its 'MSXC' record:
pack_mission.rider_descriptors); every other pack only has its references
to the rewritten packs moved (borrowed chunk offsets, 'MSXD' lender offset,
trailer size and CRC). Nothing else changes: the stage tables, cells, map
images, grids, blobs and chunks keep their bytes (only their offsets move).

usage: pack_add_actors.py DIR OUT --add 2:EXPORT[,EXPORT...] --add 5:... [--resources 377,4,5]
  DIR: missionN.pack files (read only); OUT: the rewritten set (all of them)."""
import argparse,json,struct,zlib,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import pack_store,pack_mission
from pack_scene import PAGE
from decode_level_script import Program
HEAD=48;STAGE=128

class Shift:
    """Old offset -> new offset of a rewritten pack: the bytes after the
    resources table move by `head` (the tables grew), the chunk bodies by
    `body`."""
    def __init__(self,tables_end,body_start,head,body):self.t,self.b,self.h,self.d=tables_end,body_start,head,body
    def __call__(self,o):return o+self.d if o>=self.b else o+self.h if o>=self.t else o

def parse(raw):
    p=dict(raw=raw)
    p['version'],p['mission'],p['stages'],p['palettes']=struct.unpack_from('>HHHH',raw,4)
    assert p['version'] in (5,6),('pack version',p['version'])
    (p['images'],p['nchunk'],p['cells'],p['grid_bytes'],p['actors'],p['actor_images'],p['resources'],p['data_offset'],p['data_length'])=struct.unpack_from('>9I',raw,12)
    o=HEAD+STAGE*p['stages'];p['cells_at']=o;o+=22*p['cells'];p['images_at']=o;o+=8*p['images'];p['pal_at']=o;o+=32*p['palettes']
    p['chunks_at']=o;o+=16*p['nchunk'];p['actors_at']=o;o+=8*p['actors'];p['aimg_at']=o;o+=12*p['actor_images'];p['res_at']=o;o+=4*p['resources']
    p['riders']=None
    if p['version']==6:
        assert raw[o:o+4]==b'MSXC';p['riders']=struct.unpack_from('>4I',raw,o+4);o+=20
    p['tables_end']=o
    p['chunks']=[list(struct.unpack_from('>4I',raw,p['chunks_at']+16*i)) for i in range(p['nchunk'])]
    own=[c[0] for c in p['chunks'] if not c[3]>>8&0xff]
    p['body_start']=min(own) if own else len(raw)
    p['trailer']=raw[-64:] if raw[-64:-60]==b'MSXX' else None
    p['body_end']=len(raw)-(64 if p['trailer'] else 0)
    return p

def fix_refs(p,shifts,sizes):
    """References of pack p to the rewritten packs (mission -> Shift, mission -> (bytes, crc))."""
    for c in p['chunks']:
        holder=c[3]>>8&0xff
        if holder in shifts:c[0]=shifts[holder](c[0])
    data=bytearray(p['raw'][p['data_offset']:p['data_offset']+p['data_length']]) if p['data_length'] else bytearray()
    if len(data)>=20 and data[:4]==b'MSXD' and struct.unpack_from('>H',data,4)[0]==2:
        holder,=struct.unpack_from('>H',data,10)
        if holder in shifts:struct.pack_into('>I',data,12,shifts[holder](struct.unpack_from('>I',data,12)[0]))
    trailer=bytearray(p['trailer']) if p['trailer'] else None
    if trailer:
        n,=struct.unpack_from('>H',trailer,4)
        for i in range(n):
            m,_,_,_=struct.unpack_from('>HHII',trailer,8+12*i)
            if m in sizes:struct.pack_into('>II',trailer,8+12*i+4,*sizes[m])
    return data,trailer

def frame_parts(f):
    pens=bytes.fromhex(f['pens_hex']);w,h=f['width'],f['height'];out=[]
    for py0 in range(0,h,64):
        for px0 in range(0,w,64):
            pw,ph=min(64,w-px0),min(64,h-py0);data=bytearray(PAGE)
            for y in range(ph):
                for x in range(pw):data[y*32+x//2]|=(pens[(py0+y)*w+px0+x]&15)<<(0 if x&1 else 4)
            out.append((bytes(data),f['x']+px0,f['y']+py0,pw,ph))
    return out

def rewrite(p,exports,riders,shifts,sizes,want=(),earlier=()):
    """The pack's bytes with the frames of `exports` it lacks; returns (raw, Shift, report)."""
    raw=p['raw'];mission=p['mission']
    data,trailer=fix_refs(p,shifts,sizes)
    actors=[struct.unpack_from('>IHH',raw,p['actors_at']+8*i) for i in range(p['actors'])]
    aimgs=[struct.unpack_from('>HHhhHH',raw,p['aimg_at']+12*i) for i in range(p['actor_images'])]
    res=[struct.unpack_from('>HH',raw,p['res_at']+4*i) for i in range(p['resources'])]
    pals=[struct.unpack_from('>16H',raw,p['pal_at']+32*i) for i in range(p['palettes'])]
    have={a[0] for a in actors};have_res={r[0] for r in res}
    old_riders=p['riders'] or (p['actor_images'],0,0,0)
    assert old_riders[0]+sum(old_riders[1:])==p['actor_images']
    # The old images: common [0, first), then the blocks of characters 1..3.
    blocks=[aimgs[:old_riders[0]]];at=old_riders[0]
    for n in old_riders[1:]:blocks.append(aimgs[at:at+n]);at+=n
    def block_of(first):
        b=0;at=old_riders[0]
        if first<at:return 0,first
        for c,n in enumerate(old_riders[1:],1):
            if first<at+n:return c,first-at
            at+=n
        raise AssertionError(first)
    entries=[(tag,)+block_of(first)+(count,) for tag,first,count in actors]   # tag, block, index in block, count (count 0: empty)
    frames={};palettes={}
    for e in exports:
        d=json.loads(Path(e).read_text())
        for f in d['frames']:frames.setdefault(f['descriptor'],f)
        for k,v in d['palettes'].items():assert palettes.setdefault(k,v)==v,('palette differs between the exports',k)
    added=[];pages=[];page_ids={}
    for name,f in sorted(frames.items(),key=lambda kv:int(kv[0],16)):
        tag=int(name,16)
        if tag in have:continue
        c=riders.get(tag,0)
        if c and mission not in pack_mission.RIDER_MISSIONS:continue
        first=len(blocks[c])
        for data_page,x,y,w,h in frame_parts(f):
            if data_page not in page_ids:page_ids[data_page]=len(pages);pages.append(data_page)
            blocks[c].append(('new',page_ids[data_page],0,x,y,w,h))
        entries.append((tag,c,first,len(blocks[c])-first));added.append(name)
    # The palette resources the frames draw with (--resources) that the pack lacks, in resource order.
    new_res=[]
    for k,v in sorted(palettes.items(),key=lambda kv:int(kv[0],16)):
        r=int(k,16)
        if r in have_res or r not in want:continue
        key=tuple(v)+(0,)*(16-len(v))
        if key not in pals:pals.append(key)
        new_res.append((r,pals.index(key)))
    res=sorted(res+new_res)
    # New pages: new chunks after the pack's own chunks and the borrowed ones (page = chunk * 8 + k).
    # A chunk an earlier rewritten pack added whose pages all are among these (or empty) is taken from that pack
    # (a borrowed record, as pack_mission.py does: each page once on the cartridge); the other pages go to new chunks.
    needed={x:i for i,x in enumerate(pages)};page_id={};borrowed=[]
    for m,recs in earlier:
        for ps,rec in recs:
            if not all(x in needed or x==bytes(PAGE) for x in ps) or all(needed.get(x) in page_id for x in ps if x in needed):continue
            cid=p['nchunk']+len(borrowed)
            for k,x in enumerate(ps):
                if x in needed:page_id.setdefault(needed[x],cid*8+k)
            borrowed.append([rec[0],rec[1],rec[2],(rec[3]&0xff)|m<<8])
    own=[x for i,x in enumerate(pages) if i not in page_id]
    while len(own)%8:own.append(bytes(PAGE))
    first_new=p['nchunk']+len(borrowed)
    for k,x in enumerate(own):
        if x in needed:page_id.setdefault(needed[x],first_new*8+k)
    decoded=[b''.join(own[i:i+8]) for i in range(0,len(own),8)]
    stored=pack_store.encode(decoded)
    def page_of(im):return (page_id[im[1]],)+im[2:] if im[0]=='new' else im
    counts=[len(b) for b in blocks];firsts=[0]
    for n in counts[:-1]:firsts.append(firsts[-1]+n)
    images=[page_of(im) for b in blocks for im in b]
    actors=sorted((tag,firsts[c]+i if n else 0,n) for tag,c,i,n in entries)
    version=6 if any(counts[1:]) else p['version']
    tables=bytearray()
    for pl in pals:tables+=struct.pack('>16H',*pl)
    chunk_at=len(tables);tables+=bytes(16*(p['nchunk']+len(borrowed)+len(decoded)))
    for a in actors:tables+=struct.pack('>IHH',*a)
    for im in images:tables+=struct.pack('>HHhhHH',*im)
    for r in res:tables+=struct.pack('>HH',*r)
    if version==6:tables+=b'MSXC'+struct.pack('>4I',counts[0],*counts[1:])
    old_tables=raw[p['pal_at']:p['tables_end']]
    head_shift=len(tables)-len(old_tables)
    # The rest of the head (grids, blobs, 'MSXD' and its padding) as it was, then 64-byte alignment of the bodies.
    rest=bytearray(raw[p['tables_end']:p['data_offset']])+data if p['data_length'] else bytearray(raw[p['tables_end']:p['body_start']])
    head=bytearray(raw[:p['pal_at']])+tables
    struct.pack_into('>HHHH',head,4,version,mission,p['stages'],len(pals))
    struct.pack_into('>9I',head,12,p['images'],p['nchunk']+len(borrowed)+len(decoded),p['cells'],p['grid_bytes'],len(actors),len(images),len(res),
                     p['data_offset']+head_shift if p['data_length'] else 0,p['data_length'])
    head+=rest
    head+=bytes((-len(head))%64)
    body_shift=len(head)-p['body_start']
    sh=Shift(p['tables_end'],p['body_start'],head_shift,body_shift)
    # Stage records: grid, unknown mask, blobs, patches, recon (absolute offsets into the head).
    for k in range(p['stages']):
        s=HEAD+STAGE*k
        for off in (28,32):
            v,=struct.unpack_from('>I',head,s+off)
            if v:struct.pack_into('>I',head,s+off,sh(v))
        for b in range(3):
            o=s+44+12*b+4;v,n=struct.unpack_from('>II',head,o)
            if n:struct.pack_into('>I',head,o,sh(v))
        v,n=struct.unpack_from('>IH',head,s+112)
        if n:struct.pack_into('>I',head,s+112,sh(v))
        v,n=struct.unpack_from('>II',head,s+118)
        if n:struct.pack_into('>I',head,s+118,sh(v))
    body=bytearray(raw[p['body_start']:p['body_end']])
    chunks=[list(c) for c in p['chunks']]
    for c in chunks:
        if not c[3]>>8&0xff:c[0]+=body_shift
    chunks+=borrowed;mine=[]
    for dec,(st,codec) in zip(decoded,stored):
        chunks.append([len(head)+len(body),len(st),zlib.crc32(dec),codec]);mine.append((tuple(dec[i:i+PAGE] for i in range(0,len(dec),PAGE)),chunks[-1]))
        body+=st;body+=bytes((-len(body))%64)
    # The trailer names every pack a borrowed record points into (size and CRC set when that pack is final).
    for m in sorted({c[3]>>8&0xff for c in borrowed}):
        assert trailer is not None
        n,=struct.unpack_from('>H',trailer,4)
        if any(struct.unpack_from('>H',trailer,8+12*i)[0]==m for i in range(n)):continue
        assert 8+12*(n+1)<=64,'trailer full';struct.pack_into('>H',trailer,4,n+1)
        struct.pack_into('>HHII',trailer,8+12*n,m,0,*sizes[m])
    base=p['pal_at']+chunk_at
    for i,c in enumerate(chunks):struct.pack_into('>4I',head,base+16*i,*c)
    out=bytes(head)+bytes(body)+(bytes(trailer) if trailer else b'')
    return out,sh,mine,dict(mission=mission,version=version,added=len(added),new_resources=[hex(r) for r,_ in new_res],new_chunks=len(decoded),borrowed_chunks=len(borrowed),
                       rider_images=dict(first=counts[0],counts=counts[1:]),bytes=len(out))

def main():
    a=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('dir',type=Path);a.add_argument('out',type=Path)
    a.add_argument('--add',action='append',default=[],metavar='MISSION:EXPORT[,EXPORT]')
    a.add_argument('--rom',default='/home/user/roms/mslugx.zip')
    a.add_argument('--resources',default='',help='hex palette resources the added frames draw with (from the exports), added where a pack lacks them')
    args=a.parse_args()
    add={int(m):e.split(',') for m,_,e in (x.partition(':') for x in args.add)}
    riders=pack_mission.rider_descriptors(Program(args.rom))
    packs=sorted(args.dir.glob('mission*.pack'),key=lambda q:int(q.stem[7:]))
    args.out.mkdir(parents=True,exist_ok=True);shifts={};sizes={};earlier=[]
    for q in packs:
        p=parse(q.read_bytes());m=p['mission']
        if m in add:
            raw,sh,mine,rep=rewrite(p,add[m],riders,shifts,sizes,{int(x,16) for x in args.resources.split(',') if x},earlier);shifts[m]=sh;earlier.append((m,mine));print(json.dumps(rep))
        else:
            data,trailer=fix_refs(p,shifts,sizes)
            raw=bytearray(p['raw'])
            for i,c in enumerate(p['chunks']):struct.pack_into('>4I',raw,p['chunks_at']+16*i,*c)
            if p['data_length']:raw[p['data_offset']:p['data_offset']+p['data_length']]=data
            if trailer:raw[-64:]=trailer
            raw=bytes(raw)
        (args.out/q.name).write_bytes(raw);sizes[m]=(len(raw),zlib.crc32(raw))
    # Read back as every tool does: each chunk decodes to its CRC, here or in the pack that holds it.
    for q in packs:
        back=pack_store.MissionPack(args.out/q.name)
        for i in range(len(back.chunks)):back.chunk(i)
    print('PASS: every chunk of',len(packs),'packs decodes to its CRC')
if __name__=='__main__':main()
