#!/usr/bin/env python3
"""Synthetic publication ordering/empty-buffer tests, not MAME execution cases."""
from neo_queue_publication import published_batches

def submit(frame,flag,descriptor):
 return f'MARCO_QUEUE_SUBMIT frame={frame} a5=00108000 flag={flag:02X} phase=01 descriptor={descriptor:08X} xy=0078015E attr=6340 z=86D6 callback=0000A036'
def swap(frame,flag):
 return f'MARCO_QUEUE_SWAP frame={frame} pc=00011E36 a5=00108000 data={flag^255:08X} flag={flag:02X} phase=01'
def video(frame,flag):
 return f'MARCO_VIDEO_STATE frame={frame} a5=00108000 flag={flag:02X} phase=01 head0=00109948 head1=0010A94E'
text='\n'.join([submit(1,255,7),swap(1,255),video(2,0),submit(2,0,8),submit(3,0,9),swap(3,0),video(4,255),swap(4,255),video(5,0),swap(5,0),video(6,255)])
batches,visible,count=published_batches(text)
assert count==4 and 2 not in visible
assert [d['descriptor'] for d in batches[visible[4]]]==[7]
assert [d['descriptor'] for d in batches[visible[5]]]==[8,9]
assert batches[visible[6]]==[]
# The second write is missing: refuse to invent a generation from a new flag.
bad=text+'\n'+submit(6,0,10)
try:published_batches(bad)
except AssertionError:pass
else:raise AssertionError('Missing swap was accepted')

# Once the next update advances phase, the latest closed buffer is visible.
advanced=text+'\n'+video(7,255).replace('phase=01','phase=02')
b,v,_=published_batches(advanced)
assert v[7]==4 and b[v[7]]==[]

print('PASS: synthetic sprite publication ordering, cross-video submissions and explicit empty buffers')
