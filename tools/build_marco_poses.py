#!/usr/bin/env python3
"""Convert verified Marco descriptors to anchored CI4 chunks, at most 2 KiB TMEM.

Only local assets are written. Metadata retains source tags as provenance;
none of these tags is ever an executable address in the N64 renderer.
"""
import argparse,hashlib,json,struct,subprocess,zlib
from pathlib import Path
from PIL import Image
from export_vehicle import program
from export_marco import palette_rgb
from mission_tiles import decode
ROOT=Path(__file__).resolve().parents[1]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--audit',type=Path);p.add_argument('--extra-audit',type=Path,action='append',default=[]);p.add_argument('--rom',type=Path,required=True);p.add_argument('--mksprite',type=Path,required=True);a=p.parse_args()
 audit_path=a.audit or a.capture/'audit.json';audit=json.loads(audit_path.read_text());assert audit['status']=='PASS'
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 assert all(sha(a.capture/n)==h for n,h in audit['provenance'].items()),'Preserve audited source capture'
 extra_audits=[]
 for path in a.extra_audit:
  extra=json.loads(path.read_text());assert extra['status']=='PASS'
  assert all(sha(path.parent/n)==h for n,h in extra['provenance'].items()),'Preserve additional audited capture'
  assert all(extra['provenance'][n]==audit['provenance'][n] for n in ('program-fixed.bin','program-bank.bin'))
  extra_audits.append(dict(path=str(path),sha256=sha(path)))
  audit['descriptor_addresses']=sorted(set(audit['descriptor_addresses'])|set(extra['descriptor_addresses']))
  audit.setdefault('palette_profiles',[]).extend(extra.get('palette_profiles',[]))
 data=program(a.rom);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
 pal=struct.unpack('>4096H',(a.capture/'palette2104.bin').read_bytes())[0x630:0x640]
 def convert_palette(words):return [0]+[((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1 for r,g,b in (palette_rgb(v) for v in words[1:])]
 rgba16=convert_palette(pal);palettes=[rgba16]
 for profile in audit.get('palette_profiles',[]):
  converted=convert_palette(profile['words'])
  if converted not in palettes:palettes.append(converted)
 colors=[((v>>11&31)*255//31,(v>>6&31)*255//31,(v>>1&31)*255//31,255 if v&1 else 0) for v in rgba16]
 source=ROOT/'build/marco-png';source.mkdir(parents=True,exist_ok=True);out=ROOT/'romfs/marco';out.mkdir(parents=True,exist_ok=True)
 parts=[];poses=[];paths=[]
 for tag in audit['descriptor_addresses']:
  address=int(tag,16);at=address-0x100000;n,hi=data[at:at+2];at+=2;tiles=[]
  for _ in range(n):
   dx,dy,w,h=struct.unpack_from('>hhBB',data,at);at+=6
   for col in range(w):
    for row in range(h):
     code=struct.unpack_from('>H',data,at)[0]|(hi<<12);at+=2;tiles.append((dx+col*16,-dy+row*16,decode(crom,code,0)))
  opaque={}
  for x,y,tile in tiles:
   for py in range(16):
    for px in range(16):
     if tile[py*16+px]:opaque[(x+px,y+py)]=tile[py*16+px]
  assert opaque
  left=min(x for x,y in opaque);top=min(y for x,y in opaque);right=max(x for x,y in opaque)+1;bottom=max(y for x,y in opaque)+1
  start=len(parts);pixels=bytearray((right-left)*(bottom-top))
  for (x,y),pen in opaque.items():pixels[(y-top)*(right-left)+x-left]=pen
  for y in range(top,bottom,64):
   for x in range(left,right,64):
    w=min(64,right-x);h=min(64,bottom-y)
    pens=[opaque.get((x+px,y+py),0) for py in range(h) for px in range(w)]
    im=Image.new('P',(w,h));im.putpalette([v for color in colors for v in color[:3]]);im.putdata(pens);im.info['transparency']=bytes([0]+[255]*15)
    path=source/f'p{len(parts)}.png';im.save(path,bits=4,optimize=False)
    assert list(Image.open(path).tobytes())==pens
    fingerprint=2166136261
    for pen in pens:fingerprint=((fingerprint^pen)*16777619)&0xffffffff
    paths.append(str(path));parts.append(dict(x=x,y=y,width=w,height=h,indices_fnv=fingerprint))
  union_left=min(left,-right-1);union_right=max(right,-left-1)
  assert union_right-union_left<=320 and bottom-top<=240,(hex(address),(left,top,right,bottom))
  anchor_x=-union_left+(320-(union_right-union_left))//2;anchor_y=-top+(240-(bottom-top))//2
  checks=[]
  for palette in palettes:
   pair=[]
   for mirror in (False,True):
    fb=bytearray(b'\x00\x01'*(320*240))
    for (x,y),pen in opaque.items():
     xx=anchor_x+(-x-2 if mirror else x);yy=anchor_y+y;assert 0<=xx<320 and 0<=yy<240
     struct.pack_into('>H',fb,(yy*320+xx)*2,palette[pen])
    pair.append(zlib.crc32(fb))
   checks.append(pair)
  poses.append(dict(tag=address,first=start,count=len(parts)-start,crc=checks,opaque_pixels=len(opaque),bounds=[left,top,right,bottom],anchor=[anchor_x,anchor_y]))
 subprocess.run([str(a.mksprite),'-f','CI4','-D','NONE','-c','0','-o',str(out),*paths],check=True,stdout=subprocess.DEVNULL)
 lines=['/* Original descriptor geometry; see the local audited capture. */',f'#define MARCO_POSES {len(poses)}u',f'#define MARCO_PARTS {len(parts)}u',f'#define MARCO_PALETTES {len(palettes)}u',
 'static const uint16_t marco_palettes[MARCO_PALETTES][16] __attribute__((aligned(16)))={'+','.join('{'+','.join(str(v) for v in palette)+'}' for palette in palettes)+'};',
 'static const struct {int16_t x,y;uint16_t width,height;uint32_t indices_fnv;} marco_parts[]={']
 lines += ['{'+','.join(str(part[k]) for k in ('x','y','width','height','indices_fnv'))+'},' for part in parts]
 lines += ['};','static const struct {uint32_t source_tag;uint16_t first,count;int16_t anchor_x,anchor_y;uint32_t crc[MARCO_PALETTES][2];} marco_poses[]={']
 for pose in poses:lines.append('{'+str(pose['tag'])+'u,'+str(pose['first'])+','+str(pose['count'])+','+str(pose['anchor'][0])+','+str(pose['anchor'][1])+',{'+','.join('{'+','.join(str(v)+'u' for v in pair)+'}' for pair in pose['crc'])+'}},')
 lines.append('};');(ROOT/'build/marco_pose_meta.h').write_text('\n'.join(lines)+'\n')
 report=dict(source='Verified original descriptors, C-ROM and observed Marco palette',poses=poses,parts=parts,palette_rgba5551=palettes,audit_sha256=sha(audit_path),additional_audits=extra_audits,rom_sha256=sha(a.rom),limits='Observed descriptors only, not complete Marco coverage. Native control/state selection is separate.')
 (ROOT/'build/marco_pose_assets.json').write_text(json.dumps(report,indent=2)+'\n');(ROOT/'build/marco-poses.stamp').write_text(str(len(poses))+' anchored poses; '+str(len(parts))+' CI4 chunks; <=2048 texture bytes per upload\n')
 print('Built',len(poses),'anchored poses,',len(palettes),'observed palette profiles,',len(parts),'CI4 parts,',sum(p.stat().st_size for p in out.glob('*.sprite')),'ROM bytes')
if __name__=='__main__':main()
