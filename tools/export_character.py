#!/usr/bin/env python3
"""Character pack (romfs/chars/cN.bin) of Tarma (1), Eri (2) or Fio (3) from
the ROM (docs/PERSONAJES.md): the same roles as the campaign Marco
(build/marco_campaign.h) taken from the other columns of the same tables.

  programs  0C0F32/0C0F62/0C0FB6/0C0FCE index every body/weapon table by
            form * 0x10 + 4 * character (+6E). Each Marco role is the long
            of one table row at column 0; the character's program is the
            long of the same row at column +6E. Converted with the formats
            of convert_marco_programs.py. A callback site whose routine and
            payload equal a site measured for Marco is that EVENT; a sound
            request 0120F8 (command 0x30) is an EVENT as every measured one;
            the cut 0C419E of Eri and Fio (row 2D0B04) is Marco's with
            another attack row; anything else stays UNRECOVERED (the
            program stops there).
  deaths    the five programs of src/marco_death.c from tables 0BF298,
            0BF3D8, 0BF358, 0BF318 and 0BF498, same column.
  poses     every descriptor those programs can draw, decoded from the
            program ROM and the C-ROM exactly as build_marco_poses.py
            (anchored CI4 parts of at most 64x64). A pose Marco's catalog
            already has (shared legs) points at Marco's parts in
            romfs/marco instead of carrying pixels.
  palettes  009B84/005978: palette n = 16 words at 200000 + n * 32, each
            the signed-doubled index of a colour word in the table 224000.
            Numbers from the per-character tables 0BEDD0 (normal), 0BEDE8
            (burnt death) and 0BEE00 (mummy); the mummy's second palette
            (slot 0x64, no ROM palette) is the one an audited capture of the
            character as a mummy shows in the same frames (--mummy-capture)
            or is left as the mummy's first.

With --character 0 the pack is Marco's own (tests only: tools/test_character.c
compares it with the built-in catalog). Evidence: --verify CAPTURE (several)
checks every descriptor and palette profile of the audited captures of that
character (tools/mame/run_marco_catalog.py --character N) against the pack.
"""
import argparse,hashlib,json,re,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from export_marco import palette_rgb
from export_vehicle import program
from mission_tiles import decode
import lz4_blocks
KINDS=['WAIT','POSE','JUMP','FIELD50','FINISH','FLIP_FACING','UNRECOVERED','REPLACE_PALETTE','SPAWN_THROW','SELECT_POSE',
       'WRITE_BYTE','WRITE_WORD','WRITE_LONG','DECREMENT_BYTE','BRANCH_FIELD','SELECT_FIELD_POSE','EVENT']
DEATH_TABLES=(0xBF298,0xBF3D8,0xBF358,0xBF318,0xBF498)   # ground, kind 2 on the ground, kinds 7/0x10, in the air, entrance
PALETTE_TABLES=dict(NORMAL=0xBEDD0,BURNT=0xBEDE8,MUMMY=0xBEE00)
# Callback sites that are the same request as a site measured for Marco with
# another ROM row as payload: 0C419E, the cut of the knife programs, with
# the attack row 2D0B04 of Eri and Fio (kind 3) where Marco and Tarma pass
# 2D0AF4 (kind 2). gameplay.c applies the row of the character.
SAME_REQUEST={(0xC419E,'002d0b04'):(0xC419E,'002d0af4')}
TABLE_FIRST,TABLE_LAST=0xBEE18,0xC0600   # the 4-character tables of the player (rows of 0x10 bytes at ...8)

class Rom:
    def __init__(self,zip_path,fixed):
        self.p2=bytes(program(zip_path));self.fixed=fixed.read_bytes()
    def _at(self,a):
        return (self.fixed,a) if a<0x100000 else (self.p2,a-0x100000)   # bank 1: 200000 = P2 + 0x100000
    def w(self,a):b,o=self._at(a);return struct.unpack_from('>H',b,o)[0]
    def l(self,a):b,o=self._at(a);return struct.unpack_from('>I',b,o)[0]
    def bytes(self,a,n):b,o=self._at(a);return b[o:o+n]
    def palette(self,n):
        return [0]+[self.w(0x224000+((2*self.w(0x200000+n*32+2*k)+0x8000)&0xffff)-0x8000) for k in range(1,16)]

def rgba(words):return [0]+[((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1 for r,g,b in (palette_rgb(v) for v in words[1:])]

def table_rows(rom,address):
    """Every row of the player's tables whose Marco column is this program."""
    rows=[]
    for a in range(TABLE_FIRST,TABLE_LAST,0x10):
        if rom.l(a)==address:rows.append(a)
    return rows

def walk(rom,roots,by_call,service_measured):
    """convert_marco_programs.py's walk over bank 1, from static roots."""
    nodes={};unrecovered=[];equivalent=0
    def go(pc):
        nonlocal equivalent
        while pc not in nodes:
            assert 0x200000<=pc<0x300000,hex(pc)
            op,arg=rom.bytes(pc,2);c=dict(address=pc,kind='',asset=0,target=0,operand=0,flags=0,dx=0,dy=0);nodes[pc]=c
            sb=lambda a:struct.unpack('b',rom.bytes(a,1))[0]
            if op==0:c.update(kind='WAIT',operand=arg);size=2
            elif op==4:c.update(kind='POSE',asset=rom.l(pc+4),flags=arg,dx=sb(pc+2),dy=sb(pc+3));size=8
            elif op==8:
                assert arg==0;dest=rom.l(pc+2);c.update(kind='JUMP',target_address=dest);go(dest);return
            elif op==0xc:
                dest=rom.l(pc+4);c.update(kind='BRANCH_FIELD',asset=rom.w(pc+2),operand=arg,target_address=dest);go(dest);size=8
            elif op==0x10:
                length=rom.w(pc+2);assert length and length%8==0 and length//8<=255
                c.update(kind='SELECT_FIELD_POSE',asset=arg,operand=length//8,target_address=pc+4)
                for offset in range(0,length,8):
                    where=pc+4+offset;assert rom.bytes(where,1)[0]==4
                    nodes[where]=dict(address=where,kind='POSE',asset=rom.l(where+4),flags=rom.bytes(where+1,1)[0],dx=sb(where+2),dy=sb(where+3),target=0,operand=0)
                size=4+length
            elif op==0x14:
                assert arg>=4;callback=rom.l(pc+2);payload=rom.bytes(pc+6,arg-4).hex()
                if SAME_REQUEST.get((callback,payload),(callback,payload)) in by_call:c.update(kind='EVENT',asset=0);equivalent+=1
                else:c.update(kind='UNRECOVERED',asset=callback);unrecovered.append(dict(command=pc,callback=callback,payload=payload))
                size=2+arg
            elif op==0x18:c.update(kind='WRITE_BYTE',asset=arg,target=rom.w(pc+2));size=4
            elif op==0x1c:c.update(kind='WRITE_WORD',target=arg,asset=rom.w(pc+2));size=4
            elif op==0x20:c.update(kind='FIELD50' if arg==0x50 else 'WRITE_LONG',target=arg,asset=rom.l(pc+2));size=6
            elif op==0x24:c.update(kind='DECREMENT_BYTE',target=arg);size=2
            elif op==0x28:c.update(kind='FINISH');return
            elif op==0x30:
                token=rom.w(pc+2)
                if service_measured:c.update(kind='EVENT',asset=0)
                else:c.update(kind='UNRECOVERED',asset=token);unrecovered.append(dict(command=pc,service='original_0x0120F8',token=token))
                size=4
            else:raise AssertionError(('Unrecovered script command',hex(pc),hex(op)))
            pc+=size
    for r in roots:go(r)
    ordered=sorted(nodes);index={pc:i for i,pc in enumerate(ordered)}
    for c in nodes.values():
        if 'target_address' in c:c['target']=index[c.pop('target_address')]
    return [nodes[pc] for pc in ordered],index,unrecovered

def pose_parts(rom,crom,address):
    """build_marco_poses.py: the descriptor's opaque pixels cut in 64x64 parts."""
    at=address;n,hi=rom.bytes(at,2);at+=2;opaque={}
    assert 0<n<=32,hex(address)
    for _ in range(n):
        dx,dy,w,h=struct.unpack('>hhBB',rom.bytes(at,6));at+=6
        for col in range(w):
            for row in range(h):
                code=rom.w(at)|(hi<<12);at+=2;tile=decode(crom,code,0)
                for py in range(16):
                    for px in range(16):
                        if tile[py*16+px]:opaque[(dx+col*16+px,-dy+row*16+py)]=tile[py*16+px]
    assert opaque,hex(address)
    left=min(x for x,y in opaque);top=min(y for x,y in opaque);right=max(x for x,y in opaque)+1;bottom=max(y for x,y in opaque)+1
    parts=[]
    for y in range(top,bottom,64):
        for x in range(left,right,64):
            w=min(64,right-x);h=min(64,bottom-y)
            pens=[opaque.get((x+px,y+py),0) for py in range(h) for px in range(w)]
            fnv=2166136261
            for pen in pens:fnv=((fnv^pen)*16777619)&0xffffffff
            parts.append(dict(x=x,y=y,width=w,height=h,fnv=fnv,pens=pens))
    union_left=min(left,-right-1);union_right=max(right,-left-1)
    assert union_right-union_left<=320 and bottom-top<=240,hex(address)
    return parts,(-union_left+(320-(union_right-union_left))//2,-top+(240-(bottom-top))//2)

def marco_catalog(meta):
    text=meta.read_text()
    parts=[tuple(int(v) for v in m) for m in re.findall(r'^\{(-?\d+),(-?\d+),(\d+),(\d+),(\d+)\},$',text[text.index('marco_parts[]'):text.index('marco_poses[]')],re.M)]
    poses={int(t):(int(f),int(c),int(ax),int(ay)) for t,f,c,ax,ay in re.findall(r'^\{(\d+)u,(\d+),(\d+),(-?\d+),(-?\d+)',text[text.index('marco_poses[]'):],re.M)}
    count=lambda n:int(re.search(r'#define MARCO_%s (\d+)u'%n,text).group(1))
    assert len(parts)==count('PARTS') and len(poses)==count('POSES')
    pal=text[text.index('marco_palettes['):];pal=pal[pal.index('={')+2:pal.index('};')]
    palettes=[[int(v) for v in row.split(',')] for row in re.findall(r'\{([\d,]+)\}',pal)]
    assert len(palettes)==count('PALETTES')
    named={n:int(v) for n,v in re.findall(r'#define MARCO_PALETTE_(\w+) (\d+)u',text)}
    return parts,poses,palettes,named

def main():
    p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('--character',type=int,required=True,choices=(0,1,2,3))
    p.add_argument('--rom',type=Path,required=True,help='mslugx.zip')
    p.add_argument('--fixed',type=Path,required=True,help='program-fixed.bin of an audited capture (P1)')
    p.add_argument('--graph',type=Path,required=True,help="Marco's converted graph.json (its measured event sites)")
    p.add_argument('--campaign',type=Path,default=ROOT/'build/marco_campaign.h')
    p.add_argument('--meta',type=Path,default=ROOT/'build/marco_pose_meta.h')
    p.add_argument('--mummy-capture',type=Path,help="audited --give-mummy capture of this character: its second mummy palette (slot 0x64, not a ROM palette)")
    p.add_argument('--verify',type=Path,action='append',default=[],help='audited capture of this character')
    p.add_argument('--output',type=Path);p.add_argument('--report',type=Path)
    a=p.parse_args();c=a.character
    rom=Rom(a.rom,a.fixed);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    graph=json.loads(a.graph.read_text())
    by_call={(e['target'],e['payload']) for e in graph['events'] if e['kind']=='CALLBACK'}
    service_measured=any(e['kind']=='SERVICE' and e['target']==0x120f8 for e in graph['events'])
    roles=[(n,int(i),int(r,16)) for n,i,r in re.findall(r'MARCO_CAMPAIGN_(\w+)=(\d+), /\* ROM ([0-9A-F]+)',a.campaign.read_text())]
    assert roles
    # Each role: the rows of the tables with Marco's program at column 0.
    # All of them must give the same program for this character.
    mapped=[];report_roles=[]
    for name,index,address in roles:
        rows=table_rows(rom,address);assert rows,(name,hex(address))
        targets={rom.l(r+4*c) for r in rows};assert len(targets)==1,(name,[hex(r) for r in rows],[hex(t) for t in targets])
        target=targets.pop();assert 0x200000<=target<0x300000,(name,hex(target))
        mapped.append((name,index,target));report_roles.append(dict(role=name,marco=address,tables=rows,program=target))
    deaths=[rom.l(t+4*c) for t in DEATH_TABLES]
    assert all(0x200000<=d<0x300000 for d in deaths)
    commands,index,unrecovered=walk(rom,[t for _,_,t in mapped],by_call,service_measured)
    assert len(commands)<65536
    death_programs=[]
    for d in deaths:
        cmds,idx,unrec=walk(rom,[d],by_call,service_measured)
        assert idx[d]==0,'a death program starts at its first command';death_programs.append(cmds);unrecovered+=unrec
    for cmd in commands+[x for d in death_programs for x in d]:
        v=cmd['asset']&0xffffffff;assert v<0x800000 or v>=0xff800000,'asset %08x does not fit the 24-bit command field'%v
    # Poses.
    marco_parts,marco_poses,marco_palettes,named=marco_catalog(a.meta)
    tags=sorted({x['asset'] for x in commands+[y for d in death_programs for y in d] if x['kind']=='POSE'})
    poses=[];parts=[];pixels=bytearray();shared=0
    for tag in tags:
        own,anchor=pose_parts(rom,crom,tag);first=len(parts)
        if tag in marco_poses and c:
            mf,mc,max_,may=marco_poses[tag];assert mc==len(own) and (max_,may)==anchor,hex(tag)
            for k,part in enumerate(own):
                assert marco_parts[mf+k]==(part['x'],part['y'],part['width'],part['height'],part['fnv']),hex(tag)
                parts.append(dict(part,data=0x80000000|(mf+k)))
            shared+=1
        else:
            if tag in marco_poses:   # character 0: the built-in catalog must be what this tool decodes
                mf,mc,max_,may=marco_poses[tag];assert mc==len(own) and (max_,may)==anchor,hex(tag)
                assert all(marco_parts[mf+k]==(q['x'],q['y'],q['width'],q['height'],q['fnv']) for k,q in enumerate(own)),hex(tag)
            for part in own:
                while len(pixels)%8:pixels.append(0)
                stride=(part['width']+1)//2;data=bytearray(stride*part['height'])
                for i,pen in enumerate(part['pens']):
                    y,x=divmod(i,part['width']);data[y*stride+x//2]|=pen<<(0 if x&1 else 4)
                parts.append(dict(part,data=len(pixels)));pixels+=data
        poses.append(dict(tag=tag,first=first,count=len(parts)-first,anchor=anchor))
    assert len(parts)<65536
    # Palettes: Marco's catalog order (0 normal, BURNT, MUMMY, MUMMY2); the
    # other profiles of that catalog (flashes seen in captures, not used by
    # the game) repeat the normal one here.
    numbers={k:rom.w(t+2*c) for k,t in PALETTE_TABLES.items()}
    normal=rgba(rom.palette(numbers['NORMAL']));palettes=[list(normal) for _ in marco_palettes]
    palettes[named['BURNT']]=rgba(rom.palette(numbers['BURNT']));palettes[named['MUMMY']]=rgba(rom.palette(numbers['MUMMY']))
    palettes[named['MUMMY2']]=list(palettes[named['MUMMY']]);mummy2='the first mummy palette (no capture)'
    if a.mummy_capture:
        # The mummy draws with two palettes in every frame: the ROM's
        # (0BEE00) and the one of slot 0x64, which no ROM palette equals.
        audit=json.loads((a.mummy_capture/'audit.json').read_text());assert audit['status']=='PASS'
        profiles=audit['palette_profiles'];first=[x for x in profiles if x['words'][1:]==rom.palette(numbers['MUMMY'])[1:]]
        assert len(first)==1,'the ROM mummy palette is in the capture'
        rom_words={tuple(rom.palette(n)[1:]) for n in range(0x1000)}
        second=[x for x in profiles if x['frame_uses']==first[0]['frame_uses'] and tuple(x['words'][1:]) not in rom_words]
        assert len(second)==1,second
        palettes[named['MUMMY2']]=rgba(second[0]['words']);mummy2='capture %s: %s'%(a.mummy_capture.name,','.join('%04X'%w for w in second[0]['words'][1:]))
    if c==0:
        for k in ('NORMAL','BURNT','MUMMY')+(('MUMMY2',) if a.mummy_capture else ()):
            i=0 if k=='NORMAL' else named[k];assert palettes[i]==marco_palettes[i],('palette differs from the built-in catalog',k)
    # Evidence from audited captures of this character.
    verified=[]
    for capture in a.verify:
        audit=json.loads((capture/'audit.json').read_text());assert audit['status']=='PASS'
        text=(capture/'debug.log').read_text()
        seen={int(v,16) for v in re.findall(r'character=([0-9A-F]{4})',text)};assert seen=={c},(capture,seen)
        have={x['tag'] for x in poses};drawn={int(t,16) for t in audit['descriptor_addresses']}
        known=[rgba(rom.palette(n)) for n in numbers.values()]
        profiles=[rgba(x['words']) in known+[palettes[named['MUMMY2']]] for x in audit.get('palette_profiles',[])]
        chosen={int(v,16) for v in re.findall(r'MARCO_PROGRAM_CHOICE .*?program=([0-9A-F]+)',text)}-{0}
        verified.append(dict(capture=capture.name,frames=audit['verified_frames'],matched_pixels=audit['matched_pixels'],descriptors=len(drawn),
            descriptors_in_pack=len(drawn&have),missing=sorted(drawn-have),palette_profiles=len(profiles),palette_profiles_from_rom=sum(profiles),
            programs_chosen=len(chosen),programs_in_pack=len(chosen&set(index)|chosen&set(deaths))))
    blob=bytearray()
    def section(data):
        while len(blob)%8:blob.append(0)
        at=len(blob);blob.extend(data);return at
    def cmd_bytes(cmds):
        out=bytearray()
        for x in cmds:out+=bytes([KINDS.index(x['kind'])])+(x['asset']&0xffffff).to_bytes(3,'big')+struct.pack('>HBBbb',x['target'],x['operand'],x['flags'],x['dx'],x['dy'])
        return out
    blob+=bytes(64)
    off_commands=section(cmd_bytes(commands))
    role_rows=sorted({(i,index[t]) for _,i,t in mapped});assert len({i for i,_ in role_rows})==len(role_rows)
    off_roles=section(b''.join(struct.pack('>HH',*r) for r in role_rows))
    death_dir=[];death_blob=bytearray()
    for d in death_programs:death_dir.append((len(death_blob)//10,len(d)));death_blob+=cmd_bytes(d)
    off_death=section(b''.join(struct.pack('>HH',*d) for d in death_dir)+death_blob)
    off_poses=section(b''.join(struct.pack('>IHHhh',x['tag'],x['first'],x['count'],*x['anchor']) for x in poses))
    off_parts=section(b''.join(struct.pack('>hhHHII',x['x'],x['y'],x['width'],x['height'],x['fnv'],x['data']) for x in parts))
    off_palettes=section(b''.join(struct.pack('>16H',*q) for q in palettes))
    off_pixels=section(pixels)
    struct.pack_into('>4sHHHHHHHHHH8I',blob,0,b'MSXP',1,c,len(commands),len(role_rows),len(poses),len(parts),len(palettes),len(death_dir),len(death_blob)//10,0,
        off_commands,off_roles,off_death,off_poses,off_parts,off_palettes,off_pixels,len(blob))
    report=dict(character=c,name=('Marco','Tarma','Eri','Fio')[c],palette_numbers=numbers,mummy2=mummy2,roles=report_roles,deaths=deaths,
        commands=len(commands),death_commands=[len(d) for d in death_programs],unrecovered=unrecovered,
        poses=len(poses),poses_shared_with_marco=shared,parts=len(parts),own_parts=sum(1 for x in parts if not x['data']&0x80000000),
        pixel_bytes=len(pixels),pack_bytes=len(blob),pack_sha256=hashlib.sha256(blob).hexdigest(),verified=verified)
    # The file: the pack in LZ4 blocks of 64 KiB (tools/lz4_blocks.py, the
    # decoder of src/scene_codec.h; msx_character_unpack), a block that
    # does not shrink stored raw (bit 31 of its size).
    BLOCK=65536;raw=[bytes(blob[i:i+BLOCK]) for i in range(0,len(blob),BLOCK)]
    packed=lz4_blocks.compress([b+bytes(BLOCK-len(b)) for b in raw]);stored=[]   # the last block padded with zeros
    for b,z in zip(raw,packed):
        if len(z)<len(b):stored.append((len(z),z))
        else:stored.append((0x80000000|len(b),b))   # a block that does not shrink
    file=struct.pack('>4sHHIII',b'MSXZ',1,c,len(blob),BLOCK,len(stored))+b''.join(struct.pack('>I',n) for n,_ in stored)+b''.join(d for _,d in stored)
    report['file_bytes']=len(file);report['file_sha256']=hashlib.sha256(file).hexdigest()
    if a.output:a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(file)
    if a.report:a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=1)+'\n')
    print('%s: %d roles, %d commands, deaths %s, %d unrecovered; %d poses (%d shared with Marco), %d parts (%d own, %d pixel bytes); pack %d bytes, file %d'%(
        report['name'],len(mapped),len(commands),report['death_commands'],len(unrecovered),len(poses),shared,len(parts),report['own_parts'],len(pixels),len(blob),report['file_bytes']))
    for v in verified:
        print('  %s: %d frames, %d pixels equal in MAME; descriptors %d/%d in the pack; palette profiles %d/%d from the ROM; programs chosen %d/%d in the pack'%(
            v['capture'],v['frames'],v['matched_pixels'],v['descriptors_in_pack'],v['descriptors'],v['palette_profiles_from_rom'],v['palette_profiles'],v['programs_in_pack'],v['programs_chosen']))
        if v['missing']:print('   missing:',' '.join('%06X'%t for t in v['missing']))
if __name__=='__main__':main()
