import subprocess,time,hashlib
from pathlib import Path
from PIL import ImageGrab,Image,ImageChops
out=Path('/workspace/emulator/v4-tests');out.mkdir(exist_ok=True)
xd='/workspace/emulator/root/usr/bin/xdotool'
def snap(name):
 im=ImageGrab.grab();im.save(out/(name+'.png'));return im
def held(key,seconds):
 subprocess.run([xd,'keydown',key],check=True);time.sleep(seconds)
 subprocess.run([xd,'keyup',key],check=True);time.sleep(.15)
def body(im):
 crop=im.crop((180,300,843,571));bg=im.getpixel((200,310))
 box=ImageChops.difference(crop,Image.new('RGB',crop.size,bg)).getbbox()
 assert box,'Character absent'
 return box,crop
idle=[]
for i in range(4):
 im=snap('idle'+str(i));idle.append(body(im)[1].tobytes());time.sleep(.11)
assert len(set(idle))>1,'Idle animation not advancing'
held('Right',2);r,_=body(snap('right-limit'))
held('Left',2);l,_=body(snap('left-limit'))
assert r[0]>l[0]+200,(r,l)
assert 0<=l[0] and r[2]<663,(r,l)
subprocess.run([xd,'keydown','Right'],check=True);time.sleep(.15)
snap('running-right');subprocess.run([xd,'keyup','Right'],check=True)
subprocess.run([xd,'keydown','Left'],check=True);time.sleep(.15)
snap('running-left');subprocess.run([xd,'keyup','Left'],check=True)
held('Return',.1);snap('lab-c')
held('a',.1);held('Right',.1);held('Right',.1);held('Right',.1);snap('lab-fix')
held('Return',.1);snap('back-to-marco')
log=Path('/workspace/emulator/run-v4-final.log').read_text()
assert 'MSX actor loaded: idle=30 run=30 interval=4' in log
assert 'ERROR' not in log and 'ASSERTION FAILED' not in log
print('PASS: idle animates, both movement limits visible, controller responds; START/lab captures saved')
