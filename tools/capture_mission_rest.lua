-- MAME 0.264, mslugx euro BIOS. Plays Mission 1 to completion. Set MSX_CAPTURE_DIR.
-- Reference-only RAM overrides (player survival/ammo, identical to
-- tools/capture_advance.lua) are never shipped in the N64 game. Nothing here
-- writes level, script, enemy or collision state; the input plan below only
-- READS the camera and the player's projected X to choose buttons.
--
-- Outputs (MSX_CAPTURE_DIR), see docs/MISSION1_REST.md:
--  trace.csv       every frame: progress, layer words, camera, stop, shake,
--                  map command pointer, player, script actor, group counts
--  actors.csv      every actor node birth/routine change seen in the list walk
--  actorstate.csv  every second frame: each live actor's main fields
--  script.csv      every change of the script actor position (+6E) / runners
--  layers.csv      every frame: headers of the four background layer objects
--  platforms.csv   object platform slots when they change
--  lspc.csv, lspc-writes.csv  LSPC mode read at each snapshot / every write
--  vram|ram|palette|game<frame>  snapshots every MSX_REST_PERIOD (30) frames
--  bank<frame>.bin CPU window 0x200000..0x2FFFFF whenever its checksum changes
--  events.csv      MSX_REST_TRACE_WINDOWS: collision and VRAM-port writes
--  prev/vram<f>    MSX_REST_PREV_DUMPS=1: VRAM of the frame before a snapshot
--  debug.log       MSX_REST_DEBUG=1 (-debug): SPAWN/DRAW/BP_ probe lines
-- Stops 60 frames after the Mission 2 script starts (or at MSX_REST_LAST).
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local last_frame=tonumber(os.getenv('MSX_REST_LAST') or '20000')
local end_at=nil -- set when the next mission's script starts
local first_dump=tonumber(os.getenv('MSX_REST_FIRST_DUMP') or '2100')
local period=tonumber(os.getenv('MSX_REST_PERIOD') or '30')
-- Extra snapshot frames (comma separated), e.g. the frame before a snapshot.
local extra={}
for f in string.gmatch(os.getenv('MSX_REST_EXTRA_DUMPS') or '','(%d+)') do extra[tonumber(f)]=true end
local frame=0
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local ROOT,DEAD,SCRIPT=0x1010a4,0x3f1e,0x101414
local trace=assert(io.open(output..'/trace.csv','w'))
trace:write('frame,camera,camera_word,fore_x,fore_y,mid_x,mid_y,sky_x,sky_y,obj4_x,obj4_y,full_speed,scroll_x,scroll_y,lock,lock_params,autoscroll,cam_x,cam_y,shake,bounds,map_cmd,x16,y16,player_routine,script_pc,script_timer,groups,inputs\n')
local actors=assert(io.open(output..'/actors.csv','w'))
actors:write('frame,camera,node,event,routine,previous,creator,parent,x,y,group,variant\n')
local scriptlog=assert(io.open(output..'/script.csv','w'))
scriptlog:write('frame,camera,node,pc\n')
local known={};local runner_pc={}
-- Per-frame headers (first 0x88 bytes) of the four background layer objects.
local layers=assert(io.open(output..'/layers.csv','w'))
layers:write('frame,object,header\n')
-- Object platforms (ROM 0140C0 registers, 013F1E releases): eight 0x40-byte
-- slots at RAM 10C06E..10C26D; logged whenever a slot's bytes change.
local platforms=assert(io.open(output..'/platforms.csv','w'))
platforms:write('frame,camera,slot,bytes\n')
local platform_last={}
-- Optional read-only write taps inside MSX_REST_TRACE_WINDOWS ("lo-hi,lo-hi"):
-- collision rings (as tools/trace_collision_loader.lua) and the VRAM port.
local windows={}
for lo,hi in string.gmatch(os.getenv('MSX_REST_TRACE_WINDOWS') or '','(%d+)-(%d+)') do windows[#windows+1]={tonumber(lo),tonumber(hi)} end
local function traced(f) for _,w in ipairs(windows) do if f>=w[1] and f<=w[2] then return true end end return false end
local dumping=false
local function msx_pc() local st=manager.machine.devices[':maincpu'].state;return (st['PC'] or st['rPC']).value end
local events=nil
if #windows>0 then
 events=assert(io.open(output..'/events.csv','w'))
 events:write('frame,kind,address,old,new,pc,camera\n')
 grid_tap=cpu:install_write_tap(0x10c7b6,0x10e83d,'rest_grid',function(address,data,mask)
  if not traced(frame) or (address>=0x10d7b6 and address<0x10d83e) then return end
  for lane=0,1 do
   local shift=lane==0 and 8 or 0
   if (mask&(0xff<<shift))~=0 then
    local a=address+lane
    events:write(string.format('%d,grid,%x,%d,%d,%x,%d\n',frame,a,cpu:read_u8(a),(data>>shift)&255,msx_pc(),cpu:read_u16(0x10e99e)))
   end
  end
 end)
 vram_tap=cpu:install_write_tap(0x3c0000,0x3c0007,'rest_vram',function(address,data,mask)
  if dumping or not traced(frame) then return end
  events:write(string.format('%d,vram%x,0,0,%x,%x,%d\n',frame,address&7,data&0xffff,msx_pc(),cpu:read_u16(0x10e99e)))
 end)
end
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do
 for name,field in pairs(port.fields) do fields[name]=field end
end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0}}
local pressed={}
local function button(name,value)
 pressed[name]=value
 for tag,p in pairs(manager.machine.ioport.ports) do
  if string.find(tag,'JOY1') then for key,f in pairs(p.fields) do
   if key==name or key=='%p '..string.sub(name,4) then f:set_value(value) end
  end end
 end
end
-- LSPC mode register read (0x3C0006): raster line (bits 15..7) and the
-- hardware auto-animation counter (bits 2..0), read before the VRAM dump
-- touches the VRAM address register. Logged for every snapshot.
local lspc=assert(io.open(output..'/lspc.csv','w'))
lspc:write('frame,lspc,auto_animation\n')
-- Every write to the LSPC mode register (0x3C0006): bit 3 disables the
-- hardware auto-animation, bits 15..8 set its speed.
lspc_writes=assert(io.open(output..'/lspc-writes.csv','w'))
lspc_writes:write('frame,value\n')
lspc_tap=cpu:install_write_tap(0x3c0006,0x3c0007,'rest_lspc',function(address,data,mask)
 lspc_writes:write(string.format('%d,%04x\n',frame,data&0xffff))
end)
-- MSX_REST_PREV_DUMPS=1: also keep the VRAM of the frame before each snapshot
-- (prev/vram<snapshot>.bin): the screen of a busy frame may still show
-- sprites of the previous list.
local prev_dumps=os.getenv('MSX_REST_PREV_DUMPS')=='1'
if prev_dumps then os.execute('mkdir -p "'..output..'/prev"') end
local function dump_vram(path)
 local v=assert(io.open(path,'wb'))
 for i=0,0x85ff do
  cpu:write_u16(0x3c0000,i)
  v:write(string.pack('>I2',cpu:read_u16(0x3c0002)))
 end
 v:close()
end
local function dump(f)
 local mode=cpu:read_u16(0x3c0006)
 lspc:write(string.format('%d,%04x,%d\n',f,mode,mode&7));lspc:flush()
 dumping=true
 local v=io.open(output..'/vram'..f..'.bin','wb')
 for i=0,0x85ff do
  cpu:write_u16(0x3c0000,i)
  v:write(string.pack('>I2',cpu:read_u16(0x3c0002)))
 end
 v:close()
 local ram=assert(io.open(output..'/ram'..f..'.bin','wb'));ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 local p=io.open(output..'/palette'..f..'.bin','wb')
 for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
 p:close()
 manager.machine.screens[':screen']:snapshot(output..'/game'..f..'.png')
 dumping=false
end
local bank_sum=nil
local function check_bank(f)
 -- Sample words spread over the banked window; dump it when they change.
 local s=0
 for a=0x200000,0x2fffff,0x1000 do s=(s*31+cpu:read_u16(a))%4294967291 end
 if s~=bank_sum then
  bank_sum=s
  local b=assert(io.open(output..'/bank'..f..'.bin','wb'));b:write(cpu:read_range(0x200000,0x2fffff,8));b:close()
  print('BANK '..f..' '..s)
 end
end
-- Every second frame: each live actor's routine, animation pointer (+20),
-- position words, health (+4E), creator (+0C), palette byte (+19).
local states=assert(io.open(output..'/actorstate.csv','w'))
states:write('frame,node,routine,anim,x,y,health,creator,palette\n')
local function walk(f,camera)
 local n=ROOT;local seen={};local count=0
 while n~=0 and n>=0x100000 and n<0x110000 and not seen[n] and count<400 do
  seen[n]=true;count=count+1
  local r=cpu:read_u32(n)
  local old=known[n]
  if r~=old then
   local ev=(old==nil or old==DEAD) and 'birth' or (r==DEAD and 'death' or 'state')
   actors:write(string.format('%d,%d,%x,%s,%x,%x,%x,%x,%d,%d,%d,%d\n',f,camera,n,ev,r,old or 0,
    cpu:read_u32(n+0x0c),cpu:read_u32(n+0x38),cpu:read_i16(n+0x28),cpu:read_i16(n+0x2c),cpu:read_u8(n+0x69),cpu:read_u8(n+0x68)))
   known[n]=r
  end
  if f%2==0 and r~=DEAD then
   states:write(string.format('%d,%x,%x,%x,%d,%d,%d,%x,%x\n',f,n,r,cpu:read_u32(n+0x20),cpu:read_i16(n+0x28),cpu:read_i16(n+0x2c),cpu:read_u16(n+0x4e),cpu:read_u32(n+0x0c),cpu:read_u8(n+0x19)))
  end
  if r==0x1a438 then
   local pc=cpu:read_u32(n+0x6e)
   if runner_pc[n]~=pc then runner_pc[n]=pc;scriptlog:write(string.format('%d,%d,%x,%x\n',f,camera,n,pc)) end
  end
  n=cpu:read_u32(n+8)
 end
end
-- Optional MAME debugger probes (launch with -debug -debugger none, see
-- tools/run_capture_rest.sh MSX_REST_DEBUG=1). They only print; the
-- breakpoint actions resume immediately ('g').
local dbg=os.getenv('MSX_REST_DEBUG')=='1' and manager.machine.debugger or nil
local draw_from=tonumber(os.getenv('MSX_REST_DRAW_FROM') or '5400')
local probe_list={0xc702a,0xc85f0,0xc8646,0xc865a,0xc88b0,0xc8764,0xc8084,0x9d8c,0x1aa28}
for a in string.gmatch(os.getenv('MSX_REST_EXTRA_PROBES') or '','(%x+)') do probe_list[#probe_list+1]=tonumber(a,16) end
local function probes()
 local regs='temp9,curpc,a6,a0,a1,a4,d0,d1,d@(sp)'
 local fmt='frame=%d pc=%06X a6=%08X a0=%08X a1=%08X a4=%08X d0=%08X d1=%08X ret=%08X'
 -- Map/layer service routines found in the camera/map command interpreter
 -- (ROM 0C83E6): grid clear 0C702A, rebase all layers 0C85F0, rebase one
 -- 0C8646, layer map load 0C865A, collision stamp 0C88B0, palette 009D8C,
 -- function-call command 0C8764, interpreter pointer write 0C8084.
 for _,a in ipairs(probe_list) do
  dbg:command(string.format('bpset %x,temp9>=834,{printf "BP_%06X ',a,a)..fmt..'\\n",'..regs..';g}')
 end
 -- Script spawn (ROM 01A10A): after the allocator 0040BC returns, A0 is the
 -- new actor, (SP) the spawn-table entry and 4(SP) the op 7 record.
 dbg:command('bpset 1a132,1,{printf "SPAWN frame=%d node=%08X entry=%08X record=%08X creator=%08X\\n",temp9,a0,d@(sp),d@(sp+4),a6;g}')
 -- Animation draw (ROM 004406, see tools/mame/trace_soldier_animation.lua).
 dbg:command(string.format('bpset 4406,temp9>=%x,{printf "DRAW frame=%%d a6=%%08X routine=%%08X desc=%%08X xy=%%08X attr=%%04X\\n",temp9,a6,d@(a6),d7,d0,d1&ffff;g}',draw_from))
end;local sweep_direction=1;local last_script=-1
-- MSX_REST_BOSS_FROM=<frame>: from that frame, trace every update of the
-- Mission 1 boss family (docs/JEFE_MISION1.md) into boss-trace.txt with
-- read/write taps on work RAM (read-only: the taps never change data).
--   F frame                       frame marker (written at frame end)
--   U node routine hex(0xB0)      dispatch (ROM 004246) of a family node: bytes before
--   V node hex(0xB0)              end of that update (ROM 00424A): bytes after
--   R addr mask data pc           RAM read during a family update
--   W addr mask data pc           RAM write during a family update
--   X addr mask data pc a6 a0 d3  write into a family node by another actor
--                                 (A0/D3: attack row and damage of 0049E8)
--   A a6 hex(0xB0)                that actor's bytes (first X of each update)
--   B node creator                a family node allocated (ROM 0040EC)
-- The family is the node that dispatches the constructor 0D5976 plus every
-- node allocated while a family node is the creator (A6 at 0040EC).
local boss_from=tonumber(os.getenv('MSX_REST_BOSS_FROM') or '0')
local boss_log=nil
local in_lua=true
local family={};local active=nil;local attacker_dumped={}
local POOL0,POOL1=0x1010a4,0x1010a4+0xb0*100
local mstate=manager.machine.devices[':maincpu'].state
local function hexbytes(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
local function boss_read(address,data,mask)
 if in_lua or dumping then return end
 if address>=POOL0 and address<POOL1 then
  local o=(address-POOL0)%0xb0
  if o==0 then
   if mstate['CURPC'].value==0x4246 then
    local n=address;in_lua=true
    local r=cpu:read_u32(n)
    if r==0xd5976 then family[n]=true end
    if family[n] then active=n;boss_log:write(string.format('U %x %x %s\n',n,r,hexbytes(n,0xb0))) else active=nil end
    attacker_dumped={}
    in_lua=false
    return
   end
  elseif o==8 and active==address-8 then
   if mstate['CURPC'].value==0x424a then
    in_lua=true;boss_log:write(string.format('V %x %s\n',active,hexbytes(active,0xb0)));in_lua=false
    active=nil;return
   end
  end
 end
 if active then boss_log:write(string.format('R %x %x %x %x\n',address,mask,data,mstate['CURPC'].value)) end
end
local function boss_write(address,data,mask)
 if in_lua or dumping then return end
 local pc=mstate['CURPC'].value
 if pc==0x40ec and address>=POOL0 and address<POOL1 and (address-POOL0)%0xb0==0xc then
  local n=address-0xc;local a6=mstate['A6'].value
  if family[a6] then family[n]=true;boss_log:write(string.format('B %x %x\n',n,a6)) else family[n]=nil end
 end
 if active then boss_log:write(string.format('W %x %x %x %x\n',address,mask,data,pc));return end
 if address>=POOL0 and address<POOL1 then
  local n=address-(address-POOL0)%0xb0
  if family[n] then
   local a6=mstate['A6'].value
   boss_log:write(string.format('X %x %x %x %x %x %x %x\n',address,mask,data,pc,a6,mstate['A0'].value,mstate['D3'].value))
   if not attacker_dumped[a6] and a6>=0x100000 and a6<0x110000 then
    attacker_dumped[a6]=true;in_lua=true;boss_log:write(string.format('A %x %s\n',a6,hexbytes(a6,0xb0)));in_lua=false
   end
  end
 end
end
local function boss_frame(f)
 if boss_from<=0 then return end
 if f==boss_from then
  boss_log=assert(io.open(output..'/boss-trace.txt','w'))
  boss_rtap=cpu:install_read_tap(0x100000,0x10ffff,'boss_r',boss_read)
  boss_wtap=cpu:install_write_tap(0x100000,0x10ffff,'boss_w',boss_write)
 end
 if boss_log then
  for n,_ in pairs(family) do if cpu:read_u32(n)==DEAD then family[n]=nil end end
  boss_log:write(string.format('F %d\n',f))
  if f%600==0 then boss_log:flush() end
 end
end
emu.register_frame_done(function()
 in_lua=true
 frame=frame+1
 if dbg then dbg:command(string.format('temp9=%x',frame)) if frame==2000 then probes() end end
 if actions[frame] then local a=actions[frame]; if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame>=1550 and frame<=1700 then button('P1 A',1) end
 if frame==1710 then button('P1 A',0) end
 -- Reference-only overrides, same addresses/values as capture_advance.lua:
 -- mission timer, P1 invulnerability bit (OR), lives/credits words, weapon
 -- (3) and its ammo/bomb counters. Never level, script, enemy or collision.
 if frame>=2000 then
  cpu:write_u8(0x1013de,0x5a)
  cpu:write_u8(0x101523,cpu:read_u8(0x101523)|0x40)
  cpu:write_u8(0x101549,0x99)
  cpu:write_u8(0x101288,2)
 end
 if frame==2100 then cpu:write_u8(0x10c71e,3) end
 local camera=cpu:read_u16(0x102036)
 if frame>=2190 then
  cpu:write_u8(0x10c725,0xff);cpu:write_u8(0x10c720,0x63)
  cpu:write_u8(0x10c621,0x30);cpu:write_u8(0x10c627,0x63)
  if camera==last_camera then still=still+1 else still=0 end
  last_camera=camera
  -- Control plan: run right firing (A), jump (B) every 2 s, grenade (C) every
  -- second. When the camera has not moved for 4 s (a scroll stop waiting for
  -- an encounter), sweep the viewport left/right aiming up, as in the
  -- MSX_ADVANCE_SWEEP experiment; the projected X (+3C) only bounds the sweep.
  local screen_x=cpu:read_u16(0x101500)&511
  local stopped=still>=240
  if stopped then
   if screen_x<=48 then sweep_direction=1 elseif screen_x>=256 then sweep_direction=-1 end
   button('P1 Right',sweep_direction>0 and 1 or 0)
   button('P1 Left',sweep_direction<0 and 1 or 0)
   button('P1 Up',(frame//180)%2==0 and 1 or 0)
  else
   button('P1 Right',1);button('P1 Left',0);button('P1 Up',0)
  end
  button('P1 A',frame%12<6 and 1 or 0)
  button('P1 B',frame>3000 and frame%120<8 and 1 or 0)
  button('P1 C',frame>2900 and frame%60<8 and 1 or 0)
 end
 if frame>=2100 then
  local b=0x10c72e;local m=0x10d7b6;local sky=0x10e83e
  local g={}
  for i=0,8 do g[#g+1]=string.format('%d',cpu:read_u8(0x10c326+i)) end
  local inputs=(pressed['P1 Left']==1 and 'L' or '')..(pressed['P1 Right']==1 and 'R' or '')..(pressed['P1 Up']==1 and 'U' or '')..(pressed['P1 A']==1 and 'A' or '')..(pressed['P1 B']==1 and 'B' or '')..(pressed['P1 C']==1 and 'C' or '')
  -- Camera stop (ROM 0C7B0C: 10E9BC long, 10E9C0..C6 operands) and the
  -- autoscroll state of ROM 0C7B4E (10E95A mode, 10E95C/10E960 velocities).
  local lp=string.format('%d %d %d %d',cpu:read_i16(0x10e9c0),cpu:read_i16(0x10e9c2),cpu:read_i16(0x10e9c4),cpu:read_i16(0x10e9c6))
  local auto=string.format('%d %d %d %d',cpu:read_u8(0x10e95a),cpu:read_u8(0x10e95b),cpu:read_i32(0x10e95c),cpu:read_i32(0x10e960))
  -- Camera routine ROM 0C7CA6: position 10E98E/10E992 (Q16.16), shake added
  -- to the layers' Y (10E9CC), clamped bounds 10E9D2..10E9D8 (words).
  local bounds=string.format('%d %d %d %d',cpu:read_i16(0x10e9d2),cpu:read_i16(0x10e9d4),cpu:read_i16(0x10e9d6),cpu:read_i16(0x10e9d8))
  trace:write(string.format('%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%d,%d,%d,%s,%x,%u,%u,%x,%x,%d,%s,%s\n',frame,camera,cpu:read_u16(0x10e99e),
   cpu:read_i16(b+0x84),cpu:read_i16(b+0x86),cpu:read_i16(m+0x84),cpu:read_i16(m+0x86),cpu:read_i16(sky+0x84),cpu:read_i16(sky+0x86),
   cpu:read_i16(0x10e8c6+0x84),cpu:read_i16(0x10e8c6+0x86),
   cpu:read_i32(b+0x14)==cpu:read_i32(m+0x14) and 1 or 0,cpu:read_i32(0x10e97c),cpu:read_i32(0x10e980),cpu:read_i32(0x10e9bc),lp,auto,cpu:read_i32(0x10e98e),cpu:read_i32(0x10e992),cpu:read_i32(0x10e9cc),bounds,cpu:read_u32(0x10e964),
   cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x1014c4),cpu:read_u32(SCRIPT+0x6e),cpu:read_u16(SCRIPT+0x72),table.concat(g,' '),inputs))
  local spc=cpu:read_u32(SCRIPT+0x6e)
  if spc~=last_script then last_script=spc;scriptlog:write(string.format('%d,%d,%x,%x\n',frame,camera,SCRIPT,spc)) end
  -- Mission 1's script occupies ROM 2D6314..2D7132 (tools/decode_level_script.py).
  -- A script position outside it after the mission started is the next
  -- mission: keep 60 more frames and stop.
  if not end_at and frame>3000 and spc~=0 and (spc<0x2d6314 or spc>0x2d7132) then end_at=frame+60;print('NEXT SCRIPT '..frame..string.format(' %x',spc)) end
  walk(frame,camera)
  for i=0,7 do
   local h=cpu:read_range(0x10c06e+i*0x40,0x10c06e+i*0x40+0x3f,8)
   if h~=platform_last[i] then
    platform_last[i]=h
    platforms:write(string.format('%d,%d,%d,%s\n',frame,camera,i,(h:gsub('.',function(c) return string.format('%02x',string.byte(c)) end))))
   end
  end
  for _,o in ipairs({0x10c72e,0x10d7b6,0x10e83e,0x10e8c6}) do
   local h=cpu:read_range(o,o+0x87,8)
   layers:write(string.format('%d,%x,%s\n',frame,o,(h:gsub('.',function(c) return string.format('%02x',string.byte(c)) end))))
  end
  if frame%300==0 or frame==2100 then check_bank(frame) end
  if (frame>=first_dump and (frame==2100 or frame%period==0)) or extra[frame] then dump(frame) end
  if prev_dumps and frame>=first_dump and (frame+1)%period==0 then dumping=true;dump_vram(output..'/prev/vram'..(frame+1)..'.bin');dumping=false end
  if frame%600==0 then print('FRAME '..frame..' camera '..camera..' script '..string.format('%x',spc));trace:flush();actors:flush();scriptlog:flush() end
 end
 boss_frame(frame);in_lua=false
 if frame==last_frame or frame==end_at then if boss_log then boss_log:close() end;trace:close();actors:close();scriptlog:close();layers:close();states:close();platforms:close();lspc:close();lspc_writes:close();if events then events:close() end;manager.machine:exit() end
end,'frame')
