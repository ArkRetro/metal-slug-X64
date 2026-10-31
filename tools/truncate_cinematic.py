#!/usr/bin/env python3
"""Cut an audited entry cinematic (filesystem/cinematics/entry.msxc/.json,
tools/pack_cinematic.py) to its frames first..LAST without re-rendering:
the delta packets are sequential, so the first N packets, their offsets and
their audited CRCs stay exactly as packed. The handoff becomes the player's
state of frame LAST+1 from the capture's player.jsonl (the parachute
descent: routine 0C162A at 3860, docs/MISSION_ENTRY.md). The audio is kept
whole (it plays on past the handoff)."""
import argparse,hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--last-frame',type=int,required=True)
    p.add_argument('--name',default='entry');a=p.parse_args()
    out=ROOT/'filesystem/cinematics';meta=json.loads((out/f'{a.name}.json').read_text());movie=(out/f'{a.name}.msxc').read_bytes()
    magic,version,w,h,count,maximum,rate,scale=struct.unpack_from('>4s7I',movie,0)
    assert magic==b'MSXC' and count==meta['frames'] and meta['first_frame']<=a.last_frame<=meta['last_frame']
    n=a.last_frame-meta['first_frame']+1
    offsets=struct.unpack_from('>%dI'%(count+1),movie,32)
    packets=[movie[offsets[i]:offsets[i+1]] for i in range(n)]
    off=32+4*(n+1);new=[off]
    for k in packets:off+=len(k);new.append(off)
    body=struct.pack('>4s7I',magic,version,w,h,n,max(map(len,packets)),rate,scale)+struct.pack('>%dI'%(n+1),*new)+b''.join(packets)
    players={r['frame']:r for l in (a.capture/'player.jsonl').read_text().splitlines() if (r:=json.loads(l))}
    hand=players[a.last_frame+1]
    assert (hand['flags15']&64 and not hand['vx'] and not hand['vy']) or hand['routine']==0xc162a
    (out/f'{a.name}.msxc').write_bytes(body)
    meta.update(last_frame=a.last_frame,frames=n,maximum_packet=max(map(len,packets)),movie_bytes=len(body),
        movie_sha256=hashlib.sha256(body).hexdigest(),frame_crc32=meta['frame_crc32'][:n],visual=meta['visual'][:n],
        handoff=dict(frame=a.last_frame+1,routine=hand['routine'],x=hand['x'],y=hand['y'],vx=hand['vx'],vy=hand['vy'],flags15=hand['flags15']))
    (out/f'{a.name}.json').write_text(json.dumps(meta,indent=2)+'\n')
    print('Cut',a.name,'to',n,'frames,',len(body),'bytes; handoff',meta['handoff'])
if __name__=='__main__':main()
