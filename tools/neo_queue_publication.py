"""Decode observed dual-buffer publication; never choose a batch by image fit."""
from extract_marco_fields import fields

def published_batches(text):
 batches={};visible={};pending=[];flag=None;closed=[];ordinal=0;swaps=0
 for line in text.splitlines():
  if not line.startswith(('MARCO_QUEUE_SUBMIT ','MARCO_QUEUE_SWAP ','MARCO_VIDEO_STATE ')):continue
  r=fields(line);tag=line.split()[0]
  assert r['a5']==0x108000 and r['flag'] in (0,255)
  if tag=='MARCO_QUEUE_SUBMIT':
   assert r['callback']==0xa036,'Unknown transform callback before queue write'
   if flag is not None and r['flag']!=flag:
    # The watchpoint starts after the ROM snapshot. Discard incomplete startup
    # history rather than infer the missing publication from pixels.
    assert swaps==0,'Missing a publication after watchpoint installation'
    pending=[];closed=[]
   flag=r['flag'];pending.append(dict(descriptor=r['descriptor'],xy=r['xy'],attr=r['attr'],source_frame=r['frame']))
  elif tag=='MARCO_QUEUE_SWAP':
   assert r['pc']==0x11e36 and r['data']&255==r['flag']^255
   assert flag is None or flag==r['flag']
   ordinal+=1;batches[ordinal]=pending;closed.append((ordinal,r['phase']));pending=[]
   flag=r['data']&255;swaps+=1
  else:
   if flag is not None and r['flag']!=flag:
    assert swaps==0,'Unexpected write-buffer flag at video boundary'
    pending=[];closed=[];flag=r['flag']
   # The phase distinguishes a just-closed queue from the following update
   # consuming it. This rule is checked against every SCB snapshot, never
   # selected by comparing candidate images. Empty batches stay empty.
   if len(closed)>=2:
    delta=(r['phase']-closed[-1][1])&255
    assert delta in (0,1),('Unrecovered publication phase',r,closed[-1])
    visible[r['frame']]=closed[-2 if delta==0 else -1][0]
 assert swaps>0 and visible,'No complete publication history'
 return batches,visible,swaps
