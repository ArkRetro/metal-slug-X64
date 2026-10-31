#!/usr/bin/env python3
"""Export the sprite descriptors drawn by the native rest-of-mission actors
(programs of tools/extract_soldier_programs.py ROOTS for the robed swordsman
0A8C46: 2A7372, 2A73B4, 2A7434, 2A74E0, 2A759C) from the original P-ROM and
C-ROM, with their logical palettes (0x1B, 0x20B, 0x3BA, allocated by
0A8CA0), and verify them pixel by pixel against MAME VRAM dumps of a family
trace (MSX_REST_FAMILY_ROOTS=a8c46): the sprite-list entries the original
wrote during the actor's last update before each dump (PC 00A078/00A080)
are rasterized with the exported pens and must give the same palette slot
and pen as the VRAM sprite. Output: filesystem/rest-native.json (layout of
filesystem/boss-native.json) and docs/rest-sprites-validation.json."""
import argparse,collections,hashlib,json,struct
from pathlib import Path
from export_marco import ROOT,reconstruct
from export_soldier_sprites import program,descriptor,resources
from mission_tiles import rgba16
from extract_boss_fight import parse
ROOTS=(0x2a7372,0x2a73b4,0x2a7434,0x2a74e0,0x2a759c,
    # Domed building 054904 (rise, ruin, turret, banners, hatch, launcher, ender, missile).
    0x24a334,0x24a33e,0x24a348,0x24a352,0x24a35c,0x24a366,0x24a370,0x24a450,0x24a530,0x24ac42,0x24abb0,
    0x24a610,0x24a6f2,0x24a742,0x24a7ce,0x24a910,0x24aa18,
    # Destructible shop 0B6566 (building stages, ruin, overlay).
    0x2a89b8,0x2a89c4,0x2a89d0,0x2a89dc,0x2a89e8,0x2a89f4,0x2a8a00,
    # Covered wagon 033C98 (drive, stop, open, wreck, canvas 034022, pieces 034094).
    0x239dc8,0x239df6,0x239e24,0x239e30,0x239ee4,0x239f26,0x239f32,
    # Arabian swordsmen 039590/0395B6 and the shared death pieces 0A93CE.
    0x23c8b0,0x23c8f8,0x23c97c,0x23c782,0x23c790,0x23c79e,0x23c7ac,0x23c7ba,0x23c7c8,0x23c7dc,0x23c7ea,
    0x23c7f8,0x23c806,0x23c814,0x23c822,0x23c836,0x23c9ba,0x23ca32,0x2a769c,0x269340,0x2a765a,0x2a78f0,0x2a796e,
    0x2a77f4,0x2a7872,0x2a79ec,0x2a7a6a,0x2a7ae8,0x2a7b2a,0x2a76f8,0x2a7776,
    # Creature 035668.
    0x23b3c6,0x23b5b8,0x23b4e4,0x23b41c,
    # Prisoner 099862 (tied, wriggles, salute, jump, run).
    0x29f6d0,0x29f9ac,0x29fa22,0x29fa40,0x29fa78,0x29f85e,0x29f874,0x29f88a,0x29f8a0,0x29f8b6,0x29f8cc,0x29f8e2,
    0x29f8f8,0x29f90e,0x29f924,0x29f93a,0x29f950,0x29f966,0x29f97c,0x29f992,0x29f99c,
    # Items 08C374/08C37C of the native kinds and the appearance 2861BC.
    0x28652c,0x28745c,0x287516,0x287814,0x2878ec,0x2879c4,0x287b40,0x2864b0,0x28635a,0x28639c,0x2861bc,
    # Armoured vehicle 087E36 (wait, stop, drive, destroyed, aim, fire, flinch, reverse).
    0x2841b4,0x2841de,0x28425e,0x2842de,0x284416,0x2847de,0x284828,0x28454e,0x2845f2,0x2846ba,0x2848aa,
    # Its children: shell 088B52, case 0896A0, smokes 088ED8/0897E2/08982A,
    # pieces 08943C and 088F20.
    0x284aba,0x284b0c,0x284b40,0x284a14,0x236272,0x24855c,0x239bca,0x247da8,
    0x28499c,0x2849a8,0x2849b4,0x2849c0,0x2849cc,0x2849d8,0x2849e4,0x2849f0,0x2849fc,0x284a08,
    # Every item kind with the motion 08B9B2 (36 is dropped by the hidden
    # boxes 09A67E).
    0x28631e,0x286336,0x28632a,0x286342,0x28634e,0x2864e0,0x28652c,0x28657c,0x286216,0x286258,0x28629a,0x2862dc,0x2875ae,0x286fc2,0x286dee,0x2866e6,0x287078,0x286eaa,0x2871c2,0x28741a,0x2876a4,0x28702c,0x28745c,0x287374,0x2867a8,0x287516,0x28756c,0x286c6c,0x286b54,0x287144,0x28724c,0x286840,0x2867ea,0x287814,0x2878ec,0x2879c4,0x287b40,0x287bb4,0x287a9c,0x28775a,0x287bc4,0x287c7e,0x28829a,0x288396,0x2887d2,0x28a720,0x24a200,0x2864a4,0x2864b0,0x2864bc,0x2864c8,0x2864d4,0x28635a,0x2863de,0x28639c,0x286420,0x286462,0x28aa54,0x286fbc,
    # Item sparkle 08C68E; player bullets 01B6DC..01B782 (flight, impact, wall
    # impacts) and grenade 01BF46/01BF10 with its explosion 01C152.
    0x28617e,0x22c1b6,0x22c3be,0x22c12e,0x22e504,0x22c884,0x22c81e,0x22c950,0x22cb60,
    # Swordsmen 09D846/09D862/09D876, their knife 09ECCC and the death splash 01C97A.
    0x2a1200,0x2a1166,0x2a1242,0x2a124c,0x2a12ca,0x2a12ee,0x2a1382,0x2a11d0,0x2a148a,0x2a13ca,0x2a13d4,
    0x2a1430,0x2a1446,0x2a154c,0x2a159e,0x2a15be,0x2a1778,0x2a178e,0x2a17c2,0x2a1892,0x2a1962,0x2a1a32,0x2a1b02,
    0x2a1b7c,0x2a1b54,0x2a1bce,0x2a1b6a,0x2a1be4,0x2a1bf6,0x2a16a6,0x2a1724,0x2a174e,0x2a1c84,0x2a1dbc,0x22e97e,
    # Prisoners 099DCC/099E06 and the rope 09A97C.
    0x29fad8,0x29fb34,0x29fcbe,0x29fde6,0x29fe52,0x29fbec,0x29fed0,0x29feec,0x2a02b6,0x2a0260,0x29ffe4,
    0x29fffa,0x2a0074,0x2a011c,0x2a0128,0x2a01d0,0x2a024a,0x29fe0a,
    # Item kinds with the motions 08BA10, 08BA38 and 08BAB2 (the foods).
    0x2865cc,0x2876e6,0x287d64,0x28851a,0x288af2,0x288c1e,0x288e1c,0x28988e,0x28a94c,0x28a582,0x288d18,0x28a672,0x28aa8c,
    # Rocket launcher 05ED84 (idle, stop, turn, deaths, fire, reload, flinch,
    # walk), its rocket 05FD8A, aim flash 060490 and muzzle 0602E4.
    0x263bc6,0x263bf4,0x263d7a,0x263dcc,0x2640e2,0x264072,0x264b00,0x264bca,0x264c94,0x264e46,
    0x264152,0x264176,0x26419a,0x2641be,0x2641e2,0x264206,0x26422a,0x26424e,
    0x264272,0x264374,0x26443e,0x264508,0x2645d2,0x26469c,0x264766,0x264830,
    0x263fc6,0x263e1e,0x26401c,0x263ef2,0x263c78,0x263d24,0x263c22,0x263cce,0x2648fa,
    0x60370,0x60394,0x603b8,0x603dc,0x60400,0x60424,0x60448,0x6046c,0x601a0,0x60242,
    # Helicopter formation 04857C: helicopter 245178/245188, rotor 24483E,
    # dropper 244F06, bomb 244DBC, impact 245106, shell 244F9E (and the lone
    # helicopter's 24473E/244712/244DD6/244E6E and turn tables).
    0x24473e,0x24483e,0x244dd6,0x244e6e,0x244f06,0x244dbc,0x244f9e,0x244712,0x245106,0x245178,0x245188,
    0x245004,0x245012,0x245020,0x24502e,0x2450a2,0x2450b0,0x2450be,0x2450cc,
    # Camel riders 09F682: camel, rider tables A0B82/A0BBA, head, shot,
    # piece, case and the dropped sword 09EE34.
    0x2a1dd8,0x2a1e56,0x2a1ea6,0x2a1ef6,0x2a1f34,0x2a1f72,0x2a21c4,0x2a222c,0x2a22a0,0x2a2334,0x2a2380,
    0x2a23d8,0x2a2178,0x2a219e,0x2a212c,0x2a2152,0x2a1ffe,0x2a2084,0x2a2430,0x2a2266,0x2a21f8,0x2a22ea,0x2a235a,
    0x2a23b6,0x2a240e,0x2a248e,0x2a24ec,0x2a251e,0x2a2550,0x2a2578,0x2a25a0,0x2a25ce,0x2a263e,0x2a2658,0x2a26fe,0x2a1cee,
    # Village decoration 0B6298 and the market stall 0400AA.
    0x2a89ac,0x2402ca,
    # Village porter 03264E and its walker.
    0x2389ce,0x2386f0,0x237f36,
    # Explosion fragments 01D31C.., debris 01CE08.. (table 01DF20) and the flash 01E058.
    0x22d414,0x22d47e,0x22d4e8,0x22d552,0x22d5bc,0x22d626,0x22d690,0x22d6e6,0x22d73c,0x22d792,0x22d7e8,
    0x22d83e,0x22d894,0x22d8ea,0x22d940,0x22d996,0x22d9ec,0x22da42,0x22da98,0x22daee,0x22db44,0x22dc2c,0x22dc82,
    0x22dcd8,0x22dd42,0x22ddac,0x22de16,0x22de80,0x22deea,0x22ea20,
    # Score label 08D330 digits (0E56EE/0E565A).
    0x285ef2,0x285efe,0x285f0a,0x285f16,0x285f22,0x285f2e,0x285f3a,0x285f46,0x285f52,0x285f5e,0x285f6a,0x285f74,0x285f7e,0x285f88,0x285f92,0x285f9c,0x286078,0x286082,0x28608c,0x286096,0x2860a0,0x2860aa,
    # Village civilians 031D80/031D92 (figures 031F52: stand, gesture, flight).
    0x237d02,0x237d0e,0x237d6e,0x237d7a,0x237cc0,
    # MISSION COMPLETE letters 06090A (60F44 letters, 60F7C mission numbers).
    0x264ff8,0x265002,0x26500c,0x265016,0x265020,0x26502a,0x265034,0x26503e,0x265048,0x265052,0x26505c,0x265066,0x265070,0x26507a,
    0x265084,0x26508e,0x265098,0x2650a2,0x2650ac,0x2650b6,
    # Player's vehicles: Camel Slug 03458E (state programs 034368.., rider
    # graphic 034F1A, effect 034FD2, eject 23A0C6, marker 03523C and its
    # children), vulcan turret 03B4E0 (23CE0C), end piece 03B936 (23D46C),
    # spark 03BA76, bullets 01B6A0/01BE24 and the dust 059E30.
    0x23a1da,0x23a306,0x23a350,0x23a376,0x23a3c0,0x23a3fc,0x23a508,0x23a276,
    0x23a5ae,0x23a6aa,0x23a6ec,0x23a712,0x23a754,0x23a778,0x23a7d0,0x23a62c,
    0x23a88c,0x23a988,0x23a9ca,0x23a9f0,0x23aa32,0x23aa56,0x23aaae,0x23a90a,
    0x23a84e,0x23ab2c,0x23a0c6,0x23b212,0x23b19c,0x23b126,0x23b288,0x23b324,0x23b3ba,
    0x23ce0c,0x23d4e0,0x23d46c,0x22c1b8,0x22c4c6,0x22c040,0x24c49c,
    # The boss turret gun's fire 08FA4E: flash 08EFFA (22D264), projectile 08F058.
    0x22d264,0x28c32c,
    # The player's C weapon (neo_chaser.c): shell 063F32 (267F92), explosion
    # 268622 and its blast 06371E (22CE6C), trail 063D9E (26863A), muzzle
    # flash 0C58D2 (0C586C, 0C5806).
    0x267f92,0x268622,0x22ce6c,0x26863a,0xc586c,0xc5806,
    # The SV-001 092A32 (neo_tank.c; extract_soldier_programs.py TANK_ROOTS).
    0x291ce4,0x291dc2,0x2942d4,0x294c6c,0x29667e,0x2952ec,0x296cfe,0x29589a,0x2972ac,0x295f1a,
    0x29792c,0x298090,0x2984ca,0x298904,0x298d3e,0x29a202,0x2934ec,0x2947a0,0x29282a,0x299180,
    0x299756,0x29c308,0x29c746,0x29b690,0x29b6f2,0x29b734,0x29bcd6,0x2964c8,0x292200,0x290fc0,
    0x2919fe,0x29090e,0x290c9e,0x291af0,
    0x29263a,0x2926b6,0x292732,0x2927ae,0x292678,0x2926f4,0x292770,0x2927ec,
    0x25d20a,0x25dba0,0x25e536,0x25eecc,0xad8ce,0x22cf2c,
    # Prisoner tally 0308C0: the big prisoner 237AEC, the special prisoners
    # 237B84/237BD0 and the parade icon 237C48.
    0x237aec,0x237b84,0x237bd0,0x237c48,
    # The mission actor's CONTINUE? and countdown digit 018120 and GAME OVER
    # 0181E4 (fixed block, neo_continue.c).
    0x18120,0x181e4)
RESOURCES=(0x1b,0x20b,0x3ba,0x472,0x475,0x476,0x477,0x478,0x4b9,0x87f,0x883,0x46e,0x121,0x122,0x123,0x17,0x19,0x1a,0x3fd,0x93,0x9b,0x37d,0x3a6,0x380,0x37e,0x3a7,0x3ac,0xd0,0x103,0x24,0x30b,0xd3,0x208,0x113,0x13f,0x37c,0x124,0x37f,0x3c6,0x114,0x133,0x38e,0x823,0x3fd,0x202,0x27c,0x378,0x3fe,0xb8,0x37a,0x16c,0x16d,0x16e,0x381,0x382,0x3fa,0x117,0x9c,0x9d,0x1d,0x15a,0x20,0x21,0x22,0x23,0x178,0x179,0x202,0x15c,0x118,0x442,0x13b,0x92,0x114,
    # Camel Slug, its rider (player 1/2), turret and bullets, marker, dust.
    0x3ff,0x376,0x377,0x3d6,0x3ad,0x884,0x170,
    # Palette of the fire's projectile 08F058.
    0x373,
    # Mission-end letters and other HEAD-only palettes.
    0x3be,0x3aa,0x200,
    # Explosion fragments, debris and flash reached in Mission 1.
    0x277,0x278,0x20a,0x29e,0x143,0x228,
    # Score label 08D330 (player 1; player 2 0x3CD/0x3CE in no capture), item kind 55.
    0x3b8,0x3b9,
    # The C weapon's blast 06371E.
    0x363,
    # The SV-001: body 0x3D3/0x39D/0x203 (exit list 093882: 0x396..0x39A, 0x39C..0x39E),
    # shell 0x258, dust and pieces 0x204, rider entry 0x4/0x5.
    0x3d3,0x39d,0x203,0x258,0x204,0x4,0x5,0x396,0x397,0x398,0x399,0x39a,0x39c,0x39e,
    # Prisoner tally 0308C0: parade icon (0x381, 0x3FA, 0x9B listed above).
    0x2a2,
    # CONTINUE? letters (slot 04) and digit (slot FE) of 018120 (go2 dumps).
    0x63,0x25f)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path)
    p.add_argument('captures',nargs='+',help='capture directory[:node,...] or directory@lo-hi (family trace; nodes, or routine range, validated)')
    p.add_argument('--palette-capture',action='append',type=Path,default=[],help='capture directory read for palettes only (e.g. the BLOOD ON capture of tools/capture_blood.lua)')
    p.add_argument('--report',type=Path,default=ROOT/'docs/rest-sprites-validation.json')
    p.add_argument('--keep',type=Path,default=ROOT/'filesystem/rest-native.json',help='previous export: its palettes stay when no given capture has them (captures no longer at hand)')
    a=p.parse_args()
    data=program(a.zip);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    progs=[q for q in json.loads((ROOT/'data/soldier_programs.json').read_text())['programs'] if q['root'] in ROOTS]
    assets=[]
    for prog in progs:
        for c in prog['commands']:
            if c['kind']=='POSE' and c['asset'] not in assets:assets.append(c['asset'])
    def columns(address):
        at=address-0x100000;groups,attr=data[at:at+2];at+=2;out=[]
        for g in range(groups):
            x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6+2*cols*rows
            out+=[x+16*c for c in range(cols)]
        return out
    descs={}
    for d in assets:
        try:descs[d]=descriptor(data,d,crom)
        except AssertionError:descs[d]=dict(descriptor=hex(d),x=0,y=0,width=0,height=0,pens=b'')
        descs[d]['columns']=columns(d)
    caps=[]
    for c in a.captures:
        if '@' in c:
            path,_,r=c.partition('@');lo,hi=(int(x,16) for x in r.split('-'));caps.append((Path(path),(lo,hi)))
        else:
            path,_,nodes=c.partition(':');caps.append((Path(path),[int(n,16) for n in nodes.split(',')] if nodes else [0x104644]))
    palettes={}
    for pal_file in sorted(f for cap in [c for c,_ in caps]+a.palette_capture for f in cap.glob('palette*.bin')):
        frame=int(pal_file.stem[7:]);ram_file=pal_file.parent/f'ram{frame}.bin'
        if not ram_file.exists():continue
        ram=ram_file.read_bytes();pw=struct.unpack('>4096H',pal_file.read_bytes())
        for slot,rid in resources(ram).items():
            if rid in RESOURCES:palettes.setdefault(rid,collections.Counter())[tuple(rgba16(pw[slot*16:slot*16+16]))]+=1
    pal_out={hex(k):list(c.most_common(1)[0][0]) for k,c in sorted(palettes.items())}
    if a.keep and a.keep.exists():
        for k,v in json.loads(a.keep.read_text()).get('palettes',{}).items():pal_out.setdefault(k,v)
        pal_out=dict(sorted(pal_out.items(),key=lambda kv:int(kv[0],16)))
    stats=collections.Counter();per=[]
    for capture,nodes in caps:validate(capture,nodes,descs,crom,stats,per)
    frames=[dict(descriptor=d['descriptor'],x=d['x'],y=d['y'],width=d['width'],height=d['height'],columns=d['columns'],pens_hex=d['pens'].hex()) for d in descs.values() if d['width']]
    out=dict(source='Original P-ROM descriptors and C-ROM tiles of the rest-of-mission programs; palettes from MAME 0.264 mslugx euro dumps',
        p2_sha256=hashlib.sha256(data).hexdigest(),frames=frames,palettes=pal_out,empty=[hex(d) for d,g in descs.items() if not g['width']],
        missing_palettes=[hex(r) for r in RESOURCES if hex(r) not in pal_out])
    (ROOT/'filesystem/rest-native.json').write_text(json.dumps(out,separators=(',',':'))+'\n')
    report=dict(descriptors=len(frames),palettes=sorted(pal_out),missing_palettes=out['missing_palettes'],stats=stats,frames=per)
    a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps(dict(descriptors=len(frames),palettes=len(pal_out),missing=out['missing_palettes'],stats=stats)))
def zoomed_slots(vram):
    """Palette slots of the shrunk sprites of a VRAM dump (the rasterizer of
    export_marco.reconstruct draws only full-size sprites)."""
    out=set();zx=zy=rows=0
    for slot in range(1,381):
        control,zoom=vram[0x8200+slot],vram[0x8000+slot]
        if control&64:zx=(zoom>>8)&15
        else:rows,zy,zx=control&63,zoom&255,(zoom>>8)&15
        if not 0<rows<=32 or (zx==15 and zy==255):continue
        for row in range(rows):out.add(vram[slot*64+row*2+1]>>8)
    return out
def validate(capture,nodes,descs,crom,stats,per):
    # Sprite-list entries written during each update of the nodes; at a dump
    # every node contributes its last (or, second try, its previous) update.
    by=collections.defaultdict(list)
    for u in parse(capture/'boss-trace.txt'):
        if isinstance(nodes,tuple):
            if not nodes[0]<=u['routine']<=nodes[1]:continue
        elif u['node'] not in nodes:continue
        if u['post'] is None or u['post'][:4]==bytes.fromhex('00003f1e'):continue   # deleting update: nothing drawn after it
        dl=[];cur=None
        for k,ad,m,d,pc in u['recs']:
            if k=='W' and pc==0xa078 and ad<0x10f000:
                if cur is None or len(cur['w'])>=6:cur=dict(w=[]);dl.append(cur)
                cur['w'].append(d)
        by[u['node']].append((u['frame'],[dict(desc=c['w'][4]<<16|c['w'][5],x=c['w'][0],y=c['w'][1],attr=c['w'][3]) for c in dl if len(c['w'])==6]))
    for vf in sorted(capture.glob('vram*.bin'),key=lambda f:int(f.stem[4:])):
        frame=int(vf.stem[4:])
        if not (capture/f'palette{frame}.bin').exists():continue
        if not any(x[0]<=frame and x[1] for ups in by.values() for x in ups):continue
        v=struct.unpack('>34304H',vf.read_bytes());pw=struct.unpack('>4096H',(capture/f'palette{frame}.bin').read_bytes())
        best=None
        for rank in (1,2):
            dl=[];f=0
            for ups in by.values():
                # Updates up to the dump; a node deleted since then draws nothing.
                done=[x for x in ups if x[0]<=frame]
                if len(done)>=rank and done[-1][0]>=frame-2:
                    f=max(f,done[-rank][0]);dl+=done[-rank][1]
            if not dl:continue
            zoomed=zoomed_slots(v)
            stats['zoomed_entries_skipped']+=sum(1 for d in dl if d['attr']>>8 in zoomed)
            dl=[d for d in dl if d['attr']>>8 not in zoomed]
            slots={d['attr']>>8 for d in dl}
            im,_=reconstruct(v,pw,crom,slots)
            mame=struct.unpack(f'<{320*256}H',im.info['neo_palette_indices'])
            native=[0]*(320*256)
            for d in dl:
                g=descs.get(d['desc'])
                if g is None:stats['missing_descriptor']+=1;continue
                if not g['width']:continue
                mirror=d['attr']&1;w,h=g['width'],g['height'];pens=g['pens']
                left=d['x']+(-(g['x']+w)-1 if mirror else g['x']);top=((512-d['y'])&511)+g['y']
                shown=set()
                for cx in g['columns']:
                    sx0=d['x']-cx-16-1 if mirror else d['x']+cx
                    if -8<=sx0<=311:shown.update(range(sx0,sx0+16))
                for yy in range(h):
                    sy=top+yy
                    if not 0<=sy<256:continue
                    for xx in range(w):
                        pen=pens[yy*w+(w-1-xx if mirror else xx)];sx=left+xx
                        if pen and 0<=sx<320 and sx in shown:
                            # Two sprites of the family on one pixel: the
                            # hardware order decides; not compared.
                            if native[sy*320+sx]:native[sy*320+sx]=-1
                            else:native[sy*320+sx]=(d['attr']>>8)*16+pen
            same=sum(1 for i in range(320*256) if native[i]>0 and native[i]==mame[i])
            diff=sum(1 for i in range(320*256) if native[i]>0 and native[i]!=mame[i])
            overlap=sum(1 for i in range(320*256) if native[i]<0)
            if best is None or diff<best[1]:best=(same,diff,rank,f,overlap)
        if best is None:continue
        stats['frames']+=1;stats['pixels_equal']+=best[0];stats['pixels_different']+=best[1];stats['pixels_overlapping']+=best[4];stats['rank_%d'%best[2]]+=1
        per.append(dict(capture=capture.name,frame=frame,update_frame=best[3],equal=best[0],different=best[1]))
if __name__=='__main__':main()
