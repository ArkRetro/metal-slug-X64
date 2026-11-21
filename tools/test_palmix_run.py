#!/usr/bin/env python3
"""The console's run-time palette mix (src/neo_palmix_run.h) against the ROM.

1. The table 224000 (the conversion of 005978 / 005EA0): for each of the 32768 words the Neo Geo colour has the
   word's own channels (red bits 10..14, green 5..9, blue 0..4, each as 4 bits and the shared low bit) and no dark bit.
2. neo_palmix_dac is the console colour (5 bits) of each 5-bit Neo Geo channel, as tools/mission_tiles.py rgba16.
3. neo_palmix_mix (compiled for the host, tools/test_palmix_run.c) gives the colours of tools/palette_mix.py Mixer.mix
   (005EA0, checked against MAME's palette RAM by its --verify) for every weight 1..30 of: the 288 page palettes of ROM
   0E1446 and the pictures' 845 / 125 towards palette 1, and the title pieces' 145 towards 159.

usage: test_palmix_run.py mslugx.zip   (SKIP without the ROM)"""
import re,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'tools'))
def main():
    rom=Path(sys.argv[1]) if len(sys.argv)>1 else Path('mslugx.zip')
    if not rom.exists():print(f'SKIP: run-time palette mix (no {rom})');return 0
    from palette_mix import Mixer
    from mission_tiles import rgba16
    from export_rest_sprites import ending_pages
    m=Mixer(rom);bad=0
    for w in range(32768):
        hw=m.convert(w)
        ch=(((hw>>14)&1)|((hw>>7)&30),((hw>>13)&1)|((hw>>3)&30),((hw>>12)&1)|((hw<<1)&30))
        if hw&0x8000 or ch!=((w>>10)&31,(w>>5)&31,w&31):bad+=1
    assert not bad,('table 224000 entries that are not the identity of the channels',bad)
    dac=[int(x) for x in re.search(r'neo_palmix_dac\[32\]=\{([^}]*)\}',(ROOT/'src/neo_palmix_run.h').read_text())[1].split(',')]
    for v in range(32):
        r=rgba16([0,(v&1)<<14|(v>>1)<<8])[1]>>11&31;g=rgba16([0,(v&1)<<13|(v>>1)<<4])[1]>>6&31;b=rgba16([0,(v&1)<<12|(v>>1)])[1]>>1&31
        assert r==g==b==dac[v],('neo_palmix_dac',v,r,g,b,dac[v])
    pairs=[(n,1) for n in sorted({n for n,_ in ending_pages(rom)}|{0x845,0x125})]+[(0x145,0x159)]
    raw=lambda n:[0]+[m.word(n,k) for k in range(1,16)]
    blob=bytes(m.p.u8(0x51d8+i) for i in range(0x800));cases=[]
    for a,c in pairs:
        for w in range(1,31):cases.append(struct.pack('>16H16HH16H',*raw(a),*raw(c),w,*rgba16(m.mix(a,c,w))))
    with tempfile.TemporaryDirectory() as tmp:
        tmp=Path(tmp);(tmp/'v.bin').write_bytes(blob+struct.pack('>I',len(cases))+b''.join(cases))
        subprocess.check_call(['cc','-std=c11','-Wall','-Wextra','-Werror','-O1',str(ROOT/'tools/test_palmix_run.c'),'-o',str(tmp/'t')])
        out=subprocess.run([str(tmp/'t'),str(tmp/'v.bin')],stdout=subprocess.PIPE,text=True)
    if out.returncode:print('FAIL: run-time palette mix:',out.stdout.strip());return 1
    print(f'PASS: run-time palette mix: table 224000 is the channels\' identity (32768 words), neo_palmix_dac (32 values), neo_palmix_mix = 005EA0 for {len(pairs)} pairs x 30 weights ({out.stdout.strip()})')
    return 0
if __name__=='__main__':sys.exit(main())
