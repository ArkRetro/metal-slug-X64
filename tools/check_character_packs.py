#!/usr/bin/env python3
"""B87: every character pack (romfs/chars/cN.bin, tools/export_character.py)
keys its programs by the roles of the compiled catalog (build/marco_campaign.h:
MARCO_CAMPAIGN_* = Marco's command index). A pack exported against another
catalog (the 107 roles of before B83/B84) leaves roles without a key: in the
game they are explicit stops and the weapon (the torso) draws nothing.
Fails unless each pack has exactly the catalog's roles.
Usage: check_character_packs.py build/marco_campaign.h PACK..."""
import re,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from lz4_blocks import unblock
def unpack(f):
    assert f[:4]==b'MSXZ','not a character pack'
    raw,block,blocks=struct.unpack('>III',f[8:20]);at=20+4*blocks;out=bytearray()
    for i in range(blocks):
        s=struct.unpack('>I',f[20+4*i:24+4*i])[0];n=s&0x7fffffff;want=min(block,raw-i*block)
        out+=f[at:at+n] if s&0x80000000 else unblock(f[at:at+n],block)[:want];at+=n
    return bytes(out)
names={int(v):k for k,v in re.findall(r'(MARCO_CAMPAIGN_\w+)=(\d+),',Path(sys.argv[1]).read_text())}
bad=0
for path in sys.argv[2:]:
    b=unpack(Path(path).read_bytes());assert b[:4]==b'MSXP'
    nroles=struct.unpack('>H',b[10:12])[0];off=struct.unpack('>I',b[28:32])[0]
    keys={struct.unpack('>H',b[off+4*i:off+4*i+2])[0] for i in range(nroles)}
    missing=sorted(names[r] for r in names if r not in keys);extra=sorted(keys-set(names))
    ok=not missing and not extra
    bad+=not ok
    print('%s %s: %d roles, catalog %d, %d without a key, %d keys outside the catalog%s'%('ok  ' if ok else 'FAIL',path,nroles,len(names),len(missing),len(extra),
        '' if ok else ' (re-export with tools/export_character.py: docs/PERSONAJES.md)'))
sys.exit(1 if bad else 0)
