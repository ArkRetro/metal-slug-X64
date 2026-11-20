#!/usr/bin/env python3
"""Tile layers of the ending's two screens with maps, straight from the ROM (docs/MISION6.md "Fondos del final").

The last screen (018B40: layers 017EB0, camera list 017EE6) and the title's logo (0A8096 -> 0A863C: layers 0A7D52,
list 0A7F02 with RAM 100001 != 0, the euro board of every capture; 0A7E5E with 100001 = 0) load their maps with the
same list ops as a stage (0C81A6 = 0C8084 + 0C81C0). The results 0570A0 and the ranking 057E00 have no map: their
tables 056EF4 / 0579AA allot no layer (the blueprint is the dial's sprites and the ranking's picture).

What the tool reads from the ROM, nothing from a screen:
* the layer table of 01AA28 (records [length.w][kind.w]: kind 0 = the mark after which the records are allotted in
  reverse order from sprite 1 up, 1 = that many sprites for the sprite window, 2 = a layer object with 0x20 sprites,
  3 = end): the first hardware sprite of each layer object;
* the list: op 0x0C (009D8C: palette slot <- palette number, 6 bytes each to FFFF), 0x14 (all the layers at the
  root), 0x18 (map into a layer object: block column, top block row, map descriptor), 0x20 (0C741A: the whole map
  into the sprites) or 0x48 (0C73CE: D2 columns x D3 rows from block (D0, D1));
* the blocks of each map (tile word, attribute word: palette slot, flips) and the palettes (16 words at 200000 +
  32 n of bank 1 through the conversion of tools/palette_mix.py).

Model (the cells of tools/pack_mission.py, added to the credits' stage of the pack of mission 0xC, whose own map has
no tile): block (i, j) of a map with column C and top row T at x = PAGE + 16 (C + i), y = 240 - 16 (T + 1) + 16 j
(8 px higher on the model's layer 0, as every pack: src/scene_assets.h scene_layer_rise); layer 0 = 10E83E,
2 = 10D7B6, 3 = 10C72E. Each screen has its own x page (the runtime shows a page with the layer's own offset,
src/actor.c); blank tiles are left out.

Validation against a capture's dumps (--capture: vram<frame>.bin, palette<frame>.bin): in every dump where the
screen's layer sprites are on (SCB3 height 33 on the first sprite of each layer and the first map block in place),
the sprites' tile and attribute words of every map block, the pixels of the visible raster (lines 16..239, palette
slot and pen) of the layer sprites against the model's, and the palette RAM of the slots against the ROM palettes
(a dump during a fade has other colours: counted apart, with its frame). Any different word or pixel stops the tool.

  python3 tools/export_ending_layers.py --map filesystem/mission13/s1/map.json --capture /home/user/captures-m6/fam/T1 \\
      --rest filesystem/mission13/rest-native.json --report build/ending_layers_report.json"""
import argparse,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from extract_map_patches import Rom
from export_stage_rom import map_blocks
from mission_tiles import decode,rgba16
from palette_mix import Mixer
MODEL_LAYER={0x10e83e:0,0x10d7b6:2,0x10c72e:3}
# name, layer table (01AA28), camera list (0C81A6), x page of the model, fix palettes of the screen's texts (slots)
SCREENS=(dict(name='last screen (018B40)',table=0x17eb0,list=0x17ee6,page=0x2000,fix=()),
         dict(name='title logo (0A863C, RAM 100001 != 0)',table=0xa7d52,list=0xa7f02,page=0x4000,fix=(6,7)))
JAPAN_LIST=0xa7e5e   # 0A863C with RAM 100001 = 0: reported, not exported
def allot(rom,table):
    """01AA28: first hardware sprite of each layer object and the sprite window's (first, count)."""
    a=table;low=0;high=0x17c;out={};window=None;after=[];marked=False
    for _ in range(64):
        n,kind=rom.u16(a),rom.u16(a+2)
        if kind==3:break
        if kind==0:
            assert not marked;marked=True;a+=rom.u16(a);continue
        assert kind in (1,2),('layer table',hex(a),kind)
        if not marked:
            # before the mark: from the top down (01AB1A)
            size=rom.u16(a+4) if kind==1 else 0x20;high-=size
            if kind==2:out[rom.u32(a+6)]=high+1
            else:window=(high+1,size)
        else:after.append(a)
        a+=n
    for a in after[::-1]:   # 01AA9E: the records after the mark, last first, from sprite 1 up (01AAFC)
        kind=rom.u16(a+2);size=rom.u16(a+4) if kind==1 else 0x20
        if kind==2:
            assert rom.u16(a+4)==0,('layer record +4',hex(a))
            out[rom.u32(a+6)]=low+1
        else:window=(low+1,size)
        low+=size
    assert low<=high
    return out,window
def run_list(rom,address):
    """The list's ops in order: palettes {slot: number}, maps per layer object, what each object's sprites show."""
    a=address;pal={};maps={};shown={};s16=lambda v:v-65536 if v>=32768 else v
    for _ in range(200):
        op,n=rom.u16(a),rom.u16(a+2)
        if op==8:break
        if op==0x0c:
            p=rom.u32(a+4)
            while rom.u16(p)!=0xffff:
                assert rom.u16(p+4)==0,('palette record +4',hex(p))
                pal[rom.u16(p)]=rom.u16(p+2);p+=6
        elif op==0x14:assert n==8 and (rom.u16(a+4),rom.u16(a+6))==(0,0),('rebase',hex(a))
        elif op==0x18:
            o=rom.u32(a+4);assert o not in maps and rom.u32(a+16)==0,('one map per object, no column table',hex(a))
            maps[o]=dict(column=s16(rom.u16(a+8)),top=s16(rom.u16(a+10)),map=rom.u32(a+12))
        elif op==0x1c:
            # 0C86C8: the object's factors +26 / +28; both 0 here: the layer does not move with the root.
            assert (rom.u16(a+8),rom.u16(a+10))==(0,0),('layer factors',hex(a))
        elif op==0x20:shown[rom.u32(a+4)]=None                      # 0C741A: the whole map
        elif op==0x48:shown[rom.u32(a+4)]=tuple(rom.u16(a+8+2*k) for k in range(4))   # 0C73CE: D0, D1, D2, D3
        elif op==0x24:pass                                             # 0C6CFC: the object's sprites on
        else:raise SystemExit(('list op not known here',hex(a),hex(op)))
        a+=n
    assert set(shown)==set(maps),('a layer without 0C741A / 0C73CE',hex(address))
    return pal,maps,shown
def build(rom,crom,mixer,sc,address=None):
    first,window=allot(rom,sc['table']);pal,maps,shown=run_list(rom,address or sc['list'])
    layers=[]
    for o,m in sorted(maps.items(),key=lambda x:first[x[0]]):
        t,c,w,h,tiles,coll=map_blocks(rom,m['map']);assert not coll,('a collision part',hex(m['map']))
        blocks={}
        for (i,j),(code,attr) in tiles.items():
            sh=shown[o]
            if sh is not None and not (sh[0]<=i<sh[0]+sh[2] and sh[1]<=j<sh[1]+sh[3]):continue
            assert not attr&12,('auto-animated block',hex(m['map']),i,j)
            blocks[(i,j)]=(code,attr)
        layers.append(dict(object=o,first=first[o],map=m['map'],column=m['column'],top=m['top'],width=w,height=h,blocks=blocks,shown=shown[o]))
    return dict(layers=layers,palettes=pal,window=window)
def cells_of(sc,b,crom,palette_index):
    out=[]
    for l in b['layers']:
        layer=MODEL_LAYER[l['object']];y0=240-16*(l['top']+1)-(8 if layer<2 else 0)
        for (i,j),(code,attr) in sorted(l['blocks'].items()):
            if not any(decode(crom,code,attr&3)):continue
            out.append(dict(layer=layer,x=sc['page']+16*(l['column']+i),y=y0+16*j,code=code,flip=attr&3,animation_mask=0,palette=palette_index(attr>>8),observations=0,slot=attr>>8))
    return out
def raster_model(sc,cells,crom):
    img={}
    for c in cells:
        pens=decode(crom,c['code'],c['flip']);x0=c['x']-sc['page'];y0=c['y']+(8 if c['layer']<2 else 0)
        for k,p in enumerate(pens):
            if p:
                key=(c['layer'],x0+k%16,y0+k//16)
                assert key not in img;img[key]=(c['slot'],p)
    return img
def raster_dump(v,l,crom):
    """Pixels of the layer's 32 sprites in the visible raster (full size, each sprite on its own)."""
    img={}
    for k in range(32):
        s=l['first']+k;c3,z,c4=v[0x8200+s],v[0x8000+s],v[0x8400+s]
        rows=c3&63
        if not rows:continue
        assert not c3&64 and z==0xfff,('layer sprite',s,hex(c3),hex(z))
        x=c4>>7;x=x-512 if x>=480 else x;top=(496-(c3>>7))&511
        for r in range(min(rows,32)):
            code=v[s*64+2*r];attr=v[s*64+2*r+1];code|=(attr&0xf0)<<12
            if attr&12:raise SystemExit(('auto-animated tile in a layer sprite',s,r))
            pens=decode(crom,code,attr&3)
            for q,p in enumerate(pens):
                if not p:continue
                yy=(top+16*r+q//16)&511;xx=x+q%16
                if 16<=yy<240 and 0<=xx<320:img[(MODEL_LAYER[l['object']],xx,yy)]=(attr>>8,p)
    return img
def validate(sc,b,cells,crom,mixer,capture,rep):
    model={k:v for k,v in raster_model(sc,cells,crom).items() if 16<=k[2]<240 and 0<=k[1]<320}
    frames=sorted(int(f.stem[4:]) for f in capture.glob('vram*.bin'))
    on=[];words=px=0;pal_equal=[];pal_other=[];fix_equal=[]
    for f in frames:
        v=struct.unpack('>34304H',(capture/f'vram{f}.bin').read_bytes())
        def present(l):
            (i,j),(code,attr)=next(iter(sorted(l['blocks'].items())))
            s=l['first']+l['column']+i;r=(j+l['top']+1)&31
            return (v[0x8200+s]&63)==33 and (v[s*64+2*r],v[s*64+2*r+1])==(code&0xffff,attr) and all((v[0x8200+l['first']+k]&63)==33 for k in range(l['width']))
        if not all(present(l) for l in b['layers']):continue
        # the screen's own allotment and no other: the sprites past the layers' columns are off or hold no pixel
        img={}
        for l in b['layers']:
            for (i,j),(code,attr) in l['blocks'].items():
                s=l['first']+l['column']+i;r=(j+l['top']+1)&31
                got=(v[s*64+2*r],v[s*64+2*r+1])
                if got!=(code&0xffff,attr):raise SystemExit(('block word differs',sc['name'],f,hex(l['map']),i,j,[hex(x) for x in got],hex(code),hex(attr)))
                words+=1
            img.update(raster_dump(v,l,crom))
        if img!=model:
            bad=[k for k in set(img)|set(model) if img.get(k)!=model.get(k)]
            raise SystemExit(('pixels differ',sc['name'],f,len(bad),sorted(bad)[:8]))
        px+=len(model);on.append(f)
        p=struct.unpack('>4096H',(capture/f'palette{f}.bin').read_bytes())
        used=sorted({c['slot'] for c in cells})
        same=all(list(p[16*s+1:16*s+16])==mixer.plain(b['palettes'][s])[1:] for s in used)
        (pal_equal if same else pal_other).append(f)
        if sc['fix'] and all(list(p[16*s+1:16*s+16])==mixer.plain(b['palettes'][s])[1:] for s in sc['fix']):fix_equal.append(f)
    assert on,('no dump shows the screen',sc['name'])
    assert pal_equal,('no dump with the ROM colours',sc['name'])
    rep.append(dict(screen=sc['name'],dumps=on,block_words_equal=words,pixels_equal=px,pixels_per_dump=len(model),different=0,
                    dumps_with_rom_colours=pal_equal,dumps_in_a_fade=pal_other,dumps_with_rom_fix_colours=fix_equal))
    print('%s: %d dumps (%d..%d): %d block words and %d pixels equal, 0 different; ROM colours in %d dumps, %d in a fade %s'%(
        sc['name'],len(on),on[0],on[-1],words,px,len(pal_equal),len(pal_other),pal_other))
def main():
    ap=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'));ap.add_argument('--map',type=Path,required=True,help='the credits\' stage model (tools/export_stage_rom.py --rom-model), rewritten with the screens\' cells')
    ap.add_argument('--capture',type=Path,required=True);ap.add_argument('--rest',type=Path,help='the ending\'s rest-native.json: the fix palettes of the title\'s texts are added to its palettes')
    ap.add_argument('--report',type=Path)
    a=ap.parse_args();rom=Rom(a.rom);crom=(ROOT/'filesystem/sprites.bin').read_bytes();mixer=Mixer(a.rom)
    assert not any(decode(crom,0,0)),'tile 0 is not blank'
    m=json.loads(a.map.read_text())
    m['cells']=[c for c in m['cells'] if c['x']<SCREENS[0]['page']]
    assert not m['cells'],'the credits\' stage has tiles of its own'
    def palette_of(colours):
        if colours not in m['palettes']:m['palettes'].append(colours)
        return m['palettes'].index(colours)
    rep=[];fix={}
    for sc in SCREENS:
        b=build(rom,crom,mixer,sc)
        cells=cells_of(sc,b,crom,lambda slot:palette_of(mixer.plain(b['palettes'][slot])))
        validate(sc,b,cells,crom,mixer,a.capture,rep)
        rep[-1].update(layers=[dict(object='%06X'%l['object'],first_sprite=l['first'],map='%06X'%l['map'],column=l['column'],top=l['top'],blocks=[l['width'],l['height']],
            shown=l['shown'],model_layer=MODEL_LAYER[l['object']],cells=sum(1 for c in cells if c['layer']==MODEL_LAYER[l['object']])) for l in b['layers']],
            sprite_window=b['window'],palettes={'%02X'%s:'%03X'%n for s,n in sorted(b['palettes'].items())},x_page=sc['page'])
        for c in cells:del c['slot']
        m['cells'].extend(cells)
        for s in sc['fix']:fix[b['palettes'][s]]=s
    # The title of a board with RAM 100001 = 0 (0A7E5E): the same maps, every layer whole (no fix text).
    j=build(rom,crom,mixer,SCREENS[1],JAPAN_LIST);e=build(rom,crom,mixer,SCREENS[1])
    rep.append(dict(list='%06X'%JAPAN_LIST,note='RAM 100001 = 0, not exported',same_maps=[l['map'] for l in j['layers']]==[l['map'] for l in e['layers']],same_palettes=j['palettes']==e['palettes'],
        blocks_more={'%06X'%l['object']:len(l['blocks'])-len(k['blocks']) for l,k in zip(j['layers'],e['layers'])}))
    m['width']=320
    m['source']=m['source'].split('; ending layers')[0]+'; ending layers: tools/export_ending_layers.py (ROM maps and palettes of the lists 017EE6 and 0A7F02)'
    a.map.write_text(json.dumps(m)+'\n')
    if a.rest:
        r=json.loads(a.rest.read_text())
        for n in sorted(fix):r['palettes'][hex(n)]=rgba16(mixer.plain(n))
        a.rest.write_text(json.dumps(r)+'\n')
        print('fix palettes of the title in %s: %s'%(a.rest,', '.join('%03X (slot %d)'%(n,s) for n,s in sorted(fix.items()))))
    if a.report:a.report.write_text(json.dumps(rep,indent=1)+'\n')
    print('%d cells, %d palettes -> %s'%(len(m['cells']),len(m['palettes']),a.map))
if __name__=='__main__':main()
