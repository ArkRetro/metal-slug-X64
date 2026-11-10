-- MAME 0.264, mslugx euro BIOS. Plays the WHOLE game (Missions 1..6 and the
-- ending) with one reproducible, state-driven input plan. Set MSX_CAPTURE_DIR.
-- Generalizes tools/capture_mission_rest.lua (docs/MISSION1_REST.md); see
-- docs/FULLGAME.md.
--
-- Reference-only RAM overrides (player survival/ammo, identical addresses and
-- values to tools/capture_advance.lua / capture_mission_rest.lua) are never
-- shipped in the N64 game. Nothing here writes level, script, enemy or
-- collision state. The input plan only READS the camera progress, the camera
-- Y, the script position and the player's projected X to choose buttons.
--
-- Video memory is read through the saved item ':spritegen/0/m_videoram'
-- (as tools/mame/capture_intro_audio.lua): no write to the VRAM address
-- register 0x3C0000 is needed, so the snapshots cannot disturb the game.
--
-- Outputs (MSX_CAPTURE_DIR), columns compatible with capture_mission_rest.lua:
--  trace.csv       every frame from 2100 (+ section, cam_y_int, player fields)
--  sections.csv    every change of the script table section (ROM 0x000800)
--  actors.csv, actorstate.csv, script.csv, layers.csv, platforms.csv,
--  lspc.csv, lspc-writes.csv, events.csv (collision-grid writes, all frames
--  unless MSX_FULL_GRID_TAP=0), vram|ram|palette|game<frame> every
--  MSX_FULL_PERIOD (30) frames, bank<frame>.bin when the 0x200000 window changes,
--  prev/vram<f>.bin with MSX_FULL_PREV_DUMPS=1, debug.log with MSX_FULL_DEBUG=1.
-- Stops at MSX_FULL_LAST (default 700000) or after MSX_FULL_STALL (default
-- 36000) frames without camera/script progress (reported as STALL).
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local last_frame=tonumber(os.getenv('MSX_FULL_LAST') or '700000')
local stall_limit=tonumber(os.getenv('MSX_FULL_STALL') or '36000')
local first_dump=tonumber(os.getenv('MSX_FULL_FIRST_DUMP') or '2100')
local period=tonumber(os.getenv('MSX_FULL_PERIOD') or '30')
local frame=0
local machine=manager.machine
local cpu=machine.devices[':maincpu'].spaces['program']
local sprite=assert(machine.devices[':spritegen'])
local video_memory=emu.item(assert(sprite.items['0/m_videoram']))
local ROOT,DEAD,SCRIPT=0x1010a4,0x3f1e,0x101414
-- Script table (ROM 0x000800, 30 long pointers; tools/decode_level_script.py).
local sections={}
for i=0,29 do sections[#sections+1]={index=i,base=cpu:read_u32(0x800+4*i)} end
table.sort(sections,function(a,b) return a.base<b.base end)
local function section_of(pc)
 if pc==0 then return -1 end
 local s=-1
 for _,e in ipairs(sections) do if e.base<=pc then s=e.index end end
 return s
end
local function open(name,header) local f=assert(io.open(output..'/'..name,'w'));f:write(header..'\n');return f end
local trace=open('trace.csv','frame,camera,camera_word,fore_x,fore_y,mid_x,mid_y,sky_x,sky_y,obj4_x,obj4_y,full_speed,scroll_x,scroll_y,lock,lock_params,autoscroll,cam_x,cam_y,shake,bounds,map_cmd,x16,y16,player_routine,script_pc,script_timer,groups,inputs,section,player_flags,player_screen_x,player_screen_y,weapon,plan,palette_hash')
local actors=open('actors.csv','frame,camera,node,event,routine,previous,creator,parent,x,y,group,variant')
local scriptlog=open('script.csv','frame,camera,node,pc')
local seclog=open('sections.csv','frame,camera,pc,section')
local layers=open('layers.csv','frame,object,header')
local platforms=open('platforms.csv','frame,camera,slot,bytes')
local states=open('actorstate.csv','frame,node,routine,anim,x,y,health,creator,palette')
local lspc=open('lspc.csv','frame,lspc,auto_animation')
local lspc_writes=open('lspc-writes.csv','frame,value')
local known={};local runner_pc={};local platform_last={}
local function msx_pc() local st=machine.devices[':maincpu'].state;return (st['PC'] or st['rPC']).value end
-- Collision-grid write tap (both layer rings 0x10C7B6.., 0x10D83E..; the
-- 0x10D7B6 layer header is skipped), as MSX_REST_TRACE_WINDOWS grid rows.
local events=nil
if os.getenv('MSX_FULL_GRID_TAP')~='0' then
 events=open('events.csv','frame,kind,address,old,new,pc,camera')
 grid_tap=cpu:install_write_tap(0x10c7b6,0x10e83d,'full_grid',function(address,data,mask)
  if frame<2000 or (address>=0x10d7b6 and address<0x10d83e) then return end
  local pc=msx_pc()
  if os.getenv('MSX_FULL_GRID_ALL')~='1' and pc~=0xc8982 then return end
  for lane=0,1 do
   local shift=lane==0 and 8 or 0
   if (mask&(0xff<<shift))~=0 then
    local a=address+lane
    events:write(string.format('%d,grid,%x,%d,%d,%x,%d\n',frame,a,cpu:read_u8(a),(data>>shift)&255,pc,cpu:read_u16(0x10e99e)))
   end
  end
 end)
end
-- MSX_FULL_CAMERA=1: camera.csv, one line per camera update (write tap on the
-- last store of ROM 0C7CA6, RAM 10E9D8), as tools/capture_mission_rest.lua
-- MSX_REST_CAMERA, plus the layer objects 10C72E / 10D7B6 / 10E83E / 10E8C6
-- +0..+2B as the previous layer step 0C81C0 left them (tools/extract_camera_fixture.py).
local camera_csv=nil
if os.getenv('MSX_FULL_CAMERA')=='1' then
 camera_csv=assert(io.open(output..'/camera.csv','w'))
 camera_csv:write('frame,p1,p2,mission,flags,block,d8,layers\n')
 local function hex(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
 local function player(b) return string.format('%08x:%s:%02x%02x:%02x%02x',cpu:read_u32(b),hex(b+0x3c,8),cpu:read_u8(b+0x7a),cpu:read_u8(b+0x7b),cpu:read_u8(b+0x5f),cpu:read_u8(b+0x15)) end
 camera_tap=cpu:install_write_tap(0x10e9d8,0x10e9d9,'full_camera',function(address,data,mask)
  camera_csv:write(string.format('%d,%s,%s,%08x:%s,%02x%02x%02x,%s,%04x,%s:%s:%s:%s\n',frame,player(0x1014c4),player(0x101574),
   cpu:read_u32(0x101364),hex(0x101364+0x72,8),cpu:read_u8(0x10c322),cpu:read_u8(0x10f000),cpu:read_u8(0x10c300),hex(0x10e94e,0x8e),data&0xffff,
   hex(0x10c72e,0x2c),hex(0x10d7b6,0x2c),hex(0x10e83e,0x2c),hex(0x10e8c6,0x2c)))
 end)
end
lspc_tap=cpu:install_write_tap(0x3c0006,0x3c0007,'full_lspc',function(address,data,mask)
 lspc_writes:write(string.format('%d,%04x\n',frame,data&0xffff))
end)
local fields={}
for _,port in pairs(machine.ioport.ports) do for name,field in pairs(port.fields) do fields[name]=field end end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0}}
local pressed={}
local function button(name,value)
 pressed[name]=value
 for tag,p in pairs(machine.ioport.ports) do
  if string.find(tag,'JOY1') then for key,f in pairs(p.fields) do
   if key==name or key=='%p '..string.sub(name,4) then f:set_value(value) end
  end end
 end
end
local prev_dumps=os.getenv('MSX_FULL_PREV_DUMPS')=='1'
if prev_dumps then os.execute('mkdir -p "'..output..'/prev"') end
local function dump_vram(path)
 local v=assert(io.open(path,'wb'))
 local parts={}
 for i=0,0x85ff do parts[#parts+1]=string.pack('>I2',video_memory:read(i)) end
 v:write(table.concat(parts));v:close()
end
local function dump(f)
 local mode=cpu:read_u16(0x3c0006)
 lspc:write(string.format('%d,%04x,%d\n',f,mode,mode&7));lspc:flush()
 dump_vram(output..'/vram'..f..'.bin')
 local ram=assert(io.open(output..'/ram'..f..'.bin','wb'));ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 local p=assert(io.open(output..'/palette'..f..'.bin','wb'))
 local parts={}
 for i=0,4095 do parts[#parts+1]=string.pack('>I2',cpu:read_u16(0x400000+i*2)) end
 p:write(table.concat(parts));p:close()
 machine.screens[':screen']:snapshot(output..'/game'..f..'.png')
end
-- Hash of the palette RAM (every word): a change between consecutive frames
-- marks fades and palette effects (excluded/recorded by the map tools).
local function palette_hash()
 local h=0
 for i=0,4095,1 do h=(h*31+cpu:read_u16(0x400000+i*2))%4294967291 end
 return string.format('%x',h)
end
local bank_sum=nil
local function check_bank(f)
 local s=0
 for a=0x200000,0x2fffff,0x1000 do s=(s*31+cpu:read_u16(a))%4294967291 end
 if s~=bank_sum then
  bank_sum=s
  local b=assert(io.open(output..'/bank'..f..'.bin','wb'));b:write(cpu:read_range(0x200000,0x2fffff,8));b:close()
  print('BANK '..f..' '..s)
 end
end
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
-- Debugger probes (MSX_FULL_DEBUG=1, -debug -debugger none): print only.
local dbg=os.getenv('MSX_FULL_DEBUG')=='1' and machine.debugger or nil
local draw_every=tonumber(os.getenv('MSX_FULL_DRAW_EVERY') or '1')
local function probes()
 -- MSX_FULL_NO_PROBES=1: only MSX_FULL_DEBUG_CMDS (no default probes, spawn or draw lines).
 if os.getenv('MSX_FULL_NO_PROBES')=='1' then
  for c in string.gmatch(os.getenv('MSX_FULL_DEBUG_CMDS') or '','[^|]+') do dbg:command(c) end
  return
 end
 local regs='temp9,curpc,a6,a0,a1,a4,d0,d1,d@(sp)'
 local fmt='frame=%d pc=%06X a6=%08X a0=%08X a1=%08X a4=%08X d0=%08X d1=%08X ret=%08X'
 local list={0xc702a,0xc85f0,0xc8646,0xc865a,0xc88b0,0xc8764,0xc8084,0x9d8c,0x1aa28}
 for a in string.gmatch(os.getenv('MSX_FULL_EXTRA_PROBES') or '','(%x+)') do list[#list+1]=tonumber(a,16) end
 for _,a in ipairs(list) do
  dbg:command(string.format('bpset %x,1,{printf "BP_%06X ',a,a)..fmt..'\\n",'..regs..';g}')
 end
 -- MSX_FULL_DEBUG_CMDS: extra debugger commands separated by '|'.
 for c in string.gmatch(os.getenv('MSX_FULL_DEBUG_CMDS') or '','[^|]+') do dbg:command(c) end
 dbg:command('bpset 1a132,1,{printf "SPAWN frame=%d node=%08X entry=%08X record=%08X creator=%08X\\n",temp9,a0,d@(sp),d@(sp+4),a6;g}')
 dbg:command(string.format('bpset 4406,(temp9%%%x)==0,{printf "DRAW frame=%%d a6=%%08X routine=%%08X desc=%%08X xy=%%08X attr=%%04X\\n",temp9,a6,d@(a6),d7,d0,d1&ffff;g}',draw_every))
end
-- Input plan. Progress key: camera progress word, script position, section
-- and map command pointer. `still` counts frames without any change. While
-- moving: run right firing. After 4 s still: sweep the screen; then cycle
-- through fallback patterns every 6 s (aim up, jump-spam, run left, drop
-- through with down+jump). Buttons pulse on frame parity only.
local sweep_direction=1;local last_script=-1;local last_section=-2
local seen_ending=false;local still=0;local last_key=nil;local section_start=0;local plan='run'
-- Deterministic pseudo-random sequence (LCG on the frame number) for the
-- random-walk fallback: same inputs for the same game state history.
local function noise(f,salt) return ((f//30)*1103515245+12345+salt*2654435761)%2147483648 end
-- Read-only terrain probe: shape nibble of the collision cell under world
-- point (x, y) (Y up), in whichever layer ring (0x10C72E / 0x10D7B6 + 0x88)
-- holds a nonzero byte there (docs/COLLISION.md index formula).
local function cell(x,y)
 local i=((x&504)<<3)|(((~y)&504)>>3)
 local a=cpu:read_u8(0x10c7b6+i)&15
 if a~=0 then return a end
 return cpu:read_u8(0x10d83e+i)&15
end
local last_px,blocked=nil,0
-- Edge jump: on the ground and no ground ahead within 24 px below the feet.
local function edge_ahead(px,py,dir)
 if cell(px,py-2)==0 and cell(px,py-6)==0 then return false end
 for dy=2,26,4 do if cell(px+dir*14,py-dy)~=0 then return false end end
 return true
end
local fixed_plan=os.getenv('MSX_FULL_PLAN')
local function plan_inputs(screen_x)
 local px=cpu:read_i16(0x1014ec);local py=cpu:read_i16(0x1014f0)
 if px==last_px then blocked=blocked+1 else blocked=0 end
 last_px=px
 -- MSX_FULL_PLAN=hop: the pad of the port's walk scripts (right, fire,
 -- a jump every 3 s), to compare a spot with ares (B48).
 if fixed_plan=='hop' then
  plan='hop'
  button('P1 Right',1);button('P1 Left',0);button('P1 Up',0);button('P1 Down',0)
  button('P1 B',frame%180<12 and 1 or 0);button('P1 A',frame%6<3 and 1 or 0)
  return
 end
 -- MSX_FULL_PLAN=to:<hex x>: walk right to the world x and stand still there
 -- (the birds 03EE8A take off with a player within 60 px: their flight poses).
 if fixed_plan and fixed_plan:sub(1,3)=='to:' then
  plan='to';local tx=tonumber(fixed_plan:sub(4),16) or 0
  button('P1 Right',px<tx and 1 or 0);button('P1 Left',0);button('P1 Up',0);button('P1 Down',0);button('P1 B',0);button('P1 A',0)
  return
 end
 -- MSX_FULL_PLAN=dive:<hex x>: walk right to the world x, then jump every
 -- 40 frames holding Down and firing (shots straight down: the splash
 -- 03C962 of the bullets 01B8AE over the river, G6).
 if fixed_plan and fixed_plan:sub(1,5)=='dive:' then
  plan='dive';local tx=tonumber(fixed_plan:sub(6),16) or 0
  local walk=px<tx
  button('P1 Right',walk and 1 or 0);button('P1 Left',0);button('P1 Up',0);button('P1 Down',walk and 0 or 1)
  button('P1 B',(not walk and frame%40<4) and 1 or 0);button('P1 A',(not walk and frame%6<3) and 1 or 0)
  return
 end
 -- Map command list waiting for a player inside a rectangle (ROM 0C84BE,
 -- op 4: [layer.l][x][y][w][h], layer-relative = world coordinates, Y up as
 -- player +2C): steer towards it, jumping when it is higher.
 local mc=cpu:read_u32(0x10e964)
 -- Used one 20 s period in three; the generic patterns run in between.
 if still>=240 and ((still-240)//1200)%3==1 and mc>0 and mc<0x100000 and cpu:read_u16(mc)==4 then
  local x0,y0,w,h=cpu:read_i16(mc+8),cpu:read_i16(mc+10),cpu:read_i16(mc+12),cpu:read_i16(mc+14)
  -- 0C8524 compares +40 (= Y + layer Y) with y0 + layer Y: same frame as +2C.
  plan='rect'
  local explore=((still-240)//600)%2
  local dir
  if px<x0+12 then dir=1 elseif px>x0+w-12 then dir=-1 else dir=sweep_direction end
  if explore==1 then dir=-dir end
  if blocked>=60 then sweep_direction=-dir;blocked=0 else sweep_direction=dir end
  button('P1 Right',sweep_direction>0 and 1 or 0);button('P1 Left',sweep_direction<0 and 1 or 0)
  button('P1 Up',0);button('P1 Down',0)
  local higher=py<y0
  button('P1 B',(higher and (edge_ahead(px,py,sweep_direction) or frame%20<5)) and 1 or 0)
  button('P1 A',frame%12<6 and 1 or 0)
  button('P1 C',frame%60<8 and 1 or 0)
  return
 end
 if still<240 then
  plan='run'
  button('P1 Right',1);button('P1 Left',0);button('P1 Up',0);button('P1 Down',0)
  -- Blocked by a wall for 1/3 s: jump repeatedly (climb steps, vehicles).
  button('P1 B',(frame%120<8 or (blocked>=20 and frame%16<6)) and 1 or 0)
 else
  local phase=((still-240)//360)%6
  local lo,hi=48,256
  if phase==1 or phase==5 then lo,hi=16,296 end
  if screen_x<=lo then sweep_direction=1 elseif screen_x>=hi then sweep_direction=-1 end
  -- Pushing against a bound or wall for 1 s: turn around (sweep phases).
  if blocked>=60 and (phase==1 or phase==5) then sweep_direction=-sweep_direction;blocked=0 end
  if phase==0 or phase==1 then
   plan=phase==0 and 'sweep_up' or 'sweep_jump'
   button('P1 Right',sweep_direction>0 and 1 or 0);button('P1 Left',sweep_direction<0 and 1 or 0)
   button('P1 Up',phase==0 and (frame//180)%2==0 and 1 or 0);button('P1 Down',0)
   button('P1 B',((phase==1 and edge_ahead(px,py,sweep_direction)) or frame%120<8 or (blocked>=20 and frame%16<6)) and 1 or 0)
  elseif phase==2 then
   plan='left';button('P1 Right',0);button('P1 Left',1);button('P1 Up',0);button('P1 Down',0)
   button('P1 B',frame%60<8 and 1 or 0)
  elseif phase==3 then
   plan='right_jump';button('P1 Right',1);button('P1 Left',0);button('P1 Up',frame%90<45 and 1 or 0);button('P1 Down',0)
   button('P1 B',frame%30<8 and 1 or 0)
  elseif phase==4 then
   plan='drop';button('P1 Right',sweep_direction>0 and 1 or 0);button('P1 Left',sweep_direction<0 and 1 or 0)
   button('P1 Up',0);button('P1 Down',frame%60<40 and 1 or 0);button('P1 B',frame%60>=20 and frame%60<28 and 1 or 0)
  else
   -- Random walk inside the screen with random jumps (stairs, ledges).
   plan='random'
   local n=noise(frame,1)
   if n%4==0 then sweep_direction=-sweep_direction end
   if screen_x<=lo then sweep_direction=1 elseif screen_x>=hi then sweep_direction=-1 end
   button('P1 Right',sweep_direction>0 and 1 or 0);button('P1 Left',sweep_direction<0 and 1 or 0)
   button('P1 Up',(n//7)%3==0 and 1 or 0);button('P1 Down',0)
   button('P1 B',(edge_ahead(px,py,sweep_direction) or (frame%30<6 and (n//3)%2==0)) and 1 or 0)
  end
 end
 button('P1 A',frame%12<6 and 1 or 0)
 button('P1 C',frame%60<8 and 1 or 0)
end
-- MSX_FULL_RIDE=<plan> (capture only, with MSX_FULL_LOAD): from the load
-- frame on, the inputs come from this controller instead of plan_inputs: walk
-- to the Slugnoid 09692C (the node whose routine lies in 096A10..09782F,
-- creator the script actor), jump onto it and run the plan's timed segments
-- while the vehicle slot 10C608 is in use. Plans: 'tour' (drive, vulcan with
-- the stick, jumps, cannon, crouch, down+B exit, board again, A+B launch),
-- 'fall' (drive off the ledge), 'launch' (crouch, rise, A+B).
local ride_plan=os.getenv('MSX_FULL_RIDE')
local ride_mounts,ride_from,ride_was=0,0,false
local ride_px,ride_blocked,ride_detour,ride_dir=nil,0,0,1
-- MSX_FULL_RIDE_RANGE=<lo>-<hi> (hex): the vehicle's routine interval
-- (default the Slugnoid's; the Mission 3 SV-001 093C9A: 93afa-94e2a).
local ride_lo,ride_hi=string.match(os.getenv('MSX_FULL_RIDE_RANGE') or '96a10-97830','(%x+)-(%x+)')
ride_lo,ride_hi=tonumber(ride_lo,16),tonumber(ride_hi,16)
local saved_progress=false
local function slugnoid_node()
 local n=ROOT;local count=0
 while n~=0 and n>=0x100000 and n<0x110000 and count<400 do
  count=count+1
  local r=cpu:read_u32(n)
  if r>=ride_lo and r<ride_hi and cpu:read_u32(n+0x0c)==SCRIPT then return n end
  n=cpu:read_u32(n+8)
 end
 return nil
end
local function ride_inputs()
 local b={L=0,R=0,U=0,D=0,A=0,B=0,C=0}
 -- Until the vehicle exists the usual plan runs (a mount of another kind,
 -- e.g. the Mission 3 mid-boss hold, is not the ride).
 if ride_mounts==0 and not slugnoid_node() then plan_inputs(cpu:read_u16(0x101500)&511);return end
 local mounted=cpu:read_u16(0x10c608)~=0
 if mounted and not ride_was then ride_mounts=ride_mounts+1;ride_from=frame;print('MOUNT '..ride_mounts..' '..frame) end
 if not mounted and ride_was then print('UNMOUNT '..frame) end
 ride_was=mounted
 local t=frame-ride_from
 if not mounted then
  local n=slugnoid_node()
  -- Not born yet (the script waits for the camera progress 3391): the
  -- usual plan until it is.
  if not n and ride_mounts==0 then plan_inputs(cpu:read_u16(0x101500)&511);return end
  if n and ride_mounts<2 then
   -- Tower ledges (Y up): the usual plan until the player stands level
   -- with it (run4 reaches its ledge at 91340); then walk to it and jump.
   local px=cpu:read_i16(0x1014ec)
   local vx=cpu:read_i16(n+0x28);local dy=cpu:read_i16(n+0x2c)-cpu:read_i16(0x1014f0)
   -- MSX_FULL_RIDE_SCREEN=<dx>: steer by the projected words +3C (the
   -- mount rectangle's frame, 0AA6F4) to the vehicle's +3C + dx (0AC866:
   -- its +28 is ~40 px left of its sprite origin).
   local rs=tonumber(os.getenv('MSX_FULL_RIDE_SCREEN') or '')
   if rs then px=cpu:read_i16(0x101500);vx=cpu:read_i16(n+0x3c)+rs end
   local ground=(cpu:read_u8(0x1014d9)&0x40)~=0
   if not rs and (dy>12 or dy<-40) then plan_inputs(cpu:read_u16(0x101500)&511);return end
   local d=vx-px
   if rs and frame%60==0 then print(string.format('RIDE f=%d node=%x px=%d vx=%d d=%d dy=%d ground=%s',frame,n,px,vx,d,dy,tostring(ground))) end
   if d>3 then b.R=1 elseif d<-3 then b.L=1 end
   -- With MSX_FULL_RIDE_SCREEN the vehicle's solid body stops the walk
   -- beside it: jump from up to 48 px away and keep steering in the air.
   local reach=rs and 48 or 10
   if d>=-reach and d<=reach and ground and frame%30<6 then b.B=1 end
  end
 elseif ride_plan=='flyer' then
  -- 'flyer' (the Mission 3 vehicle 0AC866): first ride: fly right and
  -- left with C (bombs), hold down to land (0ACD76), down+B out (0ACE44);
  -- second ride: fly, then D (0AD01A: the seat thrown off).
  if ride_mounts==1 then
   if t<60 then
   elseif t<140 then b.R=1;b.C=frame%20<4 and 1 or 0
   elseif t<200 then b.L=1;b.U=1;b.C=frame%20<4 and 1 or 0
   elseif t<330 then b.D=1
   elseif t<336 then b.D=1;b.B=1
   end
  else
   if t<60 then
   elseif t<150 then b.R=frame%60<30 and 1 or 0;b.L=frame%60>=30 and 1 or 0;b.C=frame%30<4 and 1 or 0
   elseif t<156 then b.Dbtn=1
   end
  end
 elseif ride_plan=='tank' then
  -- 'tank' (the Mission 3 SV-001 093C9A, docs/PLAN_MISION3.md step 5):
  -- first ride: drive both ways with the vulcan, aim up, jumps, the cannon,
  -- crouch, A+B out; second ride: drive, cannon, A+B out.
  local n=slugnoid_node();local wx=n and cpu:read_i16(n+0x28) or 420
  if wx>=5400 then ride_dir=-1 elseif wx<=5200 then ride_dir=1 end
  local function walk() if ride_dir>0 then b.R=1 else b.L=1 end end
  if ride_mounts==1 then
   if t<60 then
   elseif t<360 then walk();b.A=frame%10<5 and 1 or 0;b.U=t>=240 and 1 or 0
   elseif t<600 then walk();b.B=frame%60<6 and 1 or 0;b.A=(frame%10<5 and frame%60>=10) and 1 or 0;b.C=(t>=450 and frame%40<4) and 1 or 0
   elseif t<780 then b.C=frame%30<4 and 1 or 0
   elseif t<900 then b.D=1;b.A=frame%10<5 and 1 or 0
   elseif t<906 then b.A=1;b.B=1
   end
  else
   if t<60 then
   elseif t<300 then walk();b.A=(frame%10<5 and frame%60>=10) and 1 or 0;b.C=frame%45<4 and 1 or 0
   elseif t<306 then b.A=1;b.B=1
   end
  end
 elseif ride_plan=='sit' then
  -- 'sit' (the Mission 1 SV-001 092A32, B52): drive right for 2 s, then
  -- wait still without firing so the enemies' shots reach the tank (the
  -- damage reaction 09351A; with MSX_FULL_VEHICLE_CHEAT=0 the destruction
  -- 0938DA); every 8 s a short drive right.
  if t>=60 and t<180 then b.R=1
  elseif t>=180 and (t%480)<40 then b.R=1 end
 elseif ride_plan=='boss' then
  -- 'boss' (the Mission 3 SV-001 093C9A into the final boss 04A2C2, G15):
  -- drive right until the camera progress 10E99E reaches the arena's 6494,
  -- firing, then hold the vehicle at the screen x MSX_FULL_RIDE_HOLD (default 96:
  -- farther than 0x40 from the boss, so 04DA38 draws from its vehicle
  -- tables 2C1556/2C15A4) without firing, never leaving the seat.
  local hold=tonumber(os.getenv('MSX_FULL_RIDE_HOLD') or '96')
  local sx=cpu:read_i16(0x101500)
  if t>=60 then
   if cpu:read_u16(0x10e99e)<6494 then
    -- On the way (the camera waits 2DB140.. for the soldiers): vulcan and
    -- cannon as the 'tank' plan's.
    b.R=1;b.A=frame%10<5 and 1 or 0;b.C=frame%40<4 and 1 or 0
   elseif sx<hold-4 then b.R=1 elseif sx>hold+4 then b.L=1 end
  end
 elseif ride_plan=='dmg' then
  -- 'dmg' (the Mission 1 SV-001, B59): drive right 90 frames, stop 60,
  -- a jump every 75 frames (hits in the air 0935A8), never firing, so the
  -- enemies' bullets and rockets take the tank down to its destruction.
  if t>=60 then
   local c=(t-60)%150
   if c<90 then b.R=1 end
   if (t-60)%75<4 then b.B=1 end
  end
 elseif ride_plan=='launch' then
  -- 'launch': crouch with the cannon, rise, then A+B on the ground (the
  -- walker flies up and blows: 0971F8, 0972A6, 09749A).
  if t>=60 and t<200 then b.D=1;b.C=frame%30<4 and 1 or 0
  elseif t>=300 and t<306 then b.A=1;b.B=1 end
 elseif ride_plan=='fall' then
  -- 'fall': stand, then drive right off the ledge (the walker falls below
  -- the screen: 0975C6, 09756A with the rider).
  if t>=120 then b.R=1;b.A=frame%10<5 and 1 or 0 end
 elseif ride_mounts==1 then
  -- 'tour': walk between x 400 and 440 on its ledge (the walker's +28),
  -- the turrets' aim with Up, jumps, the cannon, crouch, down+B out (A
  -- is released while B is new: A held + B new reads as A+B).
  local n=slugnoid_node();local wx=n and cpu:read_i16(n+0x28) or 420
  if wx>=440 then ride_dir=-1 elseif wx<=400 then ride_dir=1 end
  local function walk() if ride_dir>0 then b.R=1 else b.L=1 end end
  if t<120 then
  elseif t<400 then walk();b.A=frame%10<5 and 1 or 0;b.U=t>=260 and 1 or 0
  elseif t<640 then walk();b.B=frame%60<6 and 1 or 0;b.A=(frame%10<5 and frame%60>=10) and 1 or 0;b.C=(t>=500 and frame%40<4) and 1 or 0
  elseif t<840 then b.C=frame%30<4 and 1 or 0
  elseif t<980 then b.D=1;b.A=frame%10<5 and 1 or 0;b.C=frame%30<4 and 1 or 0
  elseif t<1040 then b.D=1;b.B=(t>=1000 and t<1006) and 1 or 0
  end
 else
  -- Second ride: walk and jump, then A+B (the walker flies up and blows).
  local n=slugnoid_node();local wx=n and cpu:read_i16(n+0x28) or 420
  if wx>=440 then ride_dir=-1 elseif wx<=400 then ride_dir=1 end
  if t<60 then
  elseif t<200 then if ride_dir>0 then b.R=1 else b.L=1 end;b.A=(frame%10<5 and frame%60>=10) and 1 or 0;b.B=frame%60<6 and 1 or 0
  elseif t<206 then b.A=1;b.B=1
  end
 end
 for k,name in pairs({L='P1 Left',R='P1 Right',U='P1 Up',D='P1 Down',A='P1 A',B='P1 B',C='P1 C',Dbtn='P1 D'}) do button(name,b[k] or 0) end
end
-- MSX_FULL_LOAD=<state name> MSX_FULL_LOAD_FRAME=<frame>: resume from a state
-- saved by MSX_FULL_SAVE (iteration only; canonical captures run from boot).
local load_name=os.getenv('MSX_FULL_LOAD')
-- MSX_FULL_FAMILY_ROOTS=<hex list> MSX_FULL_FAMILY_FROM=<frame>: family
-- trace (boss-trace.txt, same format as capture_mission_rest.lua's
-- MSX_REST_FAMILY_ROOTS: U/V/R/W/X/A/B/F lines; Z vram writes with
-- MSX_FULL_VRAM_TRACE=1). Read-only taps; usually with MSX_FULL_LOAD.
local fam_from=tonumber(os.getenv('MSX_FULL_FAMILY_FROM') or '0')
local family_roots={}
for h in string.gmatch(os.getenv('MSX_FULL_FAMILY_ROOTS') or '','(%x+)') do family_roots[tonumber(h,16)]=true end
-- MSX_FULL_FAMILY_NODES=<hex list>: nodes in the family from the start (the player's node 1014C0 for a vehicle's rider).
local family_nodes={}
for h in string.gmatch(os.getenv('MSX_FULL_FAMILY_NODES') or '','(%x+)') do family_nodes[tonumber(h,16)]=true end
-- MSX_FULL_FAMILY_RANGE=<lo>-<hi> (hex): also every node dispatched with a
-- routine in that interval (a piece of a long fight started mid-way).
local frange_lo,frange_hi=string.match(os.getenv('MSX_FULL_FAMILY_RANGE') or '','(%x+)-(%x+)')
if frange_lo then frange_lo,frange_hi=tonumber(frange_lo,16),tonumber(frange_hi,16) end
local fam_log=nil;local in_lua=true;local family={};local factive=nil;local attacker_dumped={}
local POOL0,POOL1=0x1010a4,0x1010a4+0xb0*100
local mstate=machine.devices[':maincpu'].state
local function hexbytes(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
local function fam_read(address,data,mask)
 if in_lua then return end
 if address>=POOL0 and address<POOL1 then
  local o=(address-POOL0)%0xb0
  if o==0 and mstate['CURPC'].value==0x4246 then
   local n=address;in_lua=true
   local r=cpu:read_u32(n)
   if family_roots[r] or (frange_lo and r>=frange_lo and r<=frange_hi) then family[n]=true end
   if family[n] then factive=n;fam_log:write(string.format('U %x %x %s\n',n,r,hexbytes(n,0xb0))) else factive=nil end
   attacker_dumped={};in_lua=false;return
  elseif o==8 and factive==address-8 and mstate['CURPC'].value==0x424a then
   in_lua=true;fam_log:write(string.format('V %x %s\n',factive,hexbytes(factive,0xb0)));in_lua=false
   factive=nil;return
  end
 end
 if factive then fam_log:write(string.format('R %x %x %x %x\n',address,mask,data,mstate['CURPC'].value)) end
end
local function fam_write(address,data,mask)
 if in_lua then return end
 local pc=mstate['CURPC'].value
 if pc==0x40ec and address>=POOL0 and address<POOL1 and (address-POOL0)%0xb0==0xc then
  local n=address-0xc;local a6=mstate['A6'].value
  if family[a6] then family[n]=true;fam_log:write(string.format('B %x %x\n',n,a6)) else family[n]=nil end
 end
 if factive then fam_log:write(string.format('W %x %x %x %x\n',address,mask,data,pc));return end
 if address>=POOL0 and address<POOL1 then
  local n=address-(address-POOL0)%0xb0
  if family[n] then
   local a6=mstate['A6'].value
   fam_log:write(string.format('X %x %x %x %x %x %x %x\n',address,mask,data,pc,a6,mstate['A0'].value,mstate['D3'].value))
   if not attacker_dumped[a6] and a6>=0x100000 and a6<0x110000 then
    attacker_dumped[a6]=true;in_lua=true;fam_log:write(string.format('A %x %s\n',a6,hexbytes(a6,0xb0)));in_lua=false
   end
  end
 end
end
local function fam_frame(f)
 if fam_from<=0 then return end
 if f==fam_from then
  fam_log=assert(io.open(output..'/boss-trace.txt','w'))
  for n,_ in pairs(family_nodes) do family[n]=true end
  fam_rtap=cpu:install_read_tap(0x100000,0x10ffff,'fam_r',fam_read)
  fam_wtap=cpu:install_write_tap(0x100000,0x10ffff,'fam_w',fam_write)
  if os.getenv('MSX_FULL_VRAM_TRACE')=='1' then
   local vaddr,vmod=0,1
   fam_vtap=cpu:install_write_tap(0x3c0000,0x3c0005,'fam_v',function(address,data,mask)
    if in_lua then return end
    if address==0x3c0000 then vaddr=data&0xffff
    elseif address==0x3c0004 then vmod=data&0xffff
    elseif address==0x3c0002 then
     if factive then fam_log:write(string.format('Z %x %x\n',vaddr,data&0xffff)) end
     vaddr=(vaddr&0x8000)|((vaddr+vmod)&0x7fff)
    end
   end)
  end
 end
 if fam_log then fam_log:write(string.format('F %d\n',f));if f%600==0 then fam_log:flush() end end
end
-- MSX_FULL_CELL_TAP=<lo>-<hi>: every collision cell the kernel reads with
-- 0C79FE (the byte read at PC 0C7A18 from a layer ring, 10C72E/10D7B6 +88)
-- in those frames, aggregated: cells.txt lines
--   C <a0> <x> <y> <value> <first frame> <last frame> <reads> <player reads>
-- with x/y the local pixel words D0/D1 of 0C79FE (column x>>3, row y>>3)
-- and player reads those made while A6 = 1014C4 (docs/MISION3.md
-- "Reconocimiento del mapa").
local cell_lo,cell_hi=string.match(os.getenv('MSX_FULL_CELL_TAP') or '','(%d+)-(%d+)')
local cell_seen,cell_order,cell_tap,cell_frame=nil,{},nil,0
local function cell_read(address,data,mask)
 if in_lua or mstate['CURPC'].value~=0xc7a18 then return end
 local a0=mstate['A0'].value
 local v=(address&1)==1 and (data&0xff) or ((data>>8)&0xff)
 if mask==0x00ff then v=data&0xff elseif mask==0xff00 then v=(data>>8)&0xff end
 local key=string.format('%x %d %d %d',a0,mstate['D0'].value&0xffff,mstate['D1'].value&0xffff,v)
 local e=cell_seen[key]
 if not e then e={cell_frame,cell_frame,0,0};cell_seen[key]=e;cell_order[#cell_order+1]=key end
 e[2]=cell_frame;e[3]=e[3]+1
 if mstate['A6'].value==0x1014c4 then e[4]=e[4]+1 end
end
local function cell_frame_done(f)
 if not cell_lo then return end
 if f==tonumber(cell_lo) then cell_seen={};cell_tap=cpu:install_read_tap(0x10c7b6,0x10e83d,'cell_r',cell_read) end
 cell_frame=f   -- same convention as temp9 (last completed frame)
 if cell_tap and f>=tonumber(cell_hi) then
  cell_tap:remove();cell_tap=nil
  local out=assert(io.open(output..'/cells.txt','w'))
  for _,k in ipairs(cell_order) do local e=cell_seen[k];out:write(string.format('C %s %d %d %d %d\n',k,e[1],e[2],e[3],e[4])) end
  out:close()
 end
end
-- MSX_FULL_FLOOR_TAP=<lo>-<hi>: floor-query trace (floor.txt): every read
-- by the collision kernel (PC 012800..013A00) with frame, PC, address, data;
-- one P line per frame with the player node (1014C4) +28/+2C/+38.
local floor_lo,floor_hi=string.match(os.getenv('MSX_FULL_FLOOR_TAP') or '','(%d+)-(%d+)')
local floor_log=nil
local function floor_frame(f)
 if not floor_lo then return end
 if f==tonumber(floor_lo) then
  floor_log=assert(io.open(output..'/floor.txt','w'))
  floor_tap=cpu:install_read_tap(0x000000,0x2fffff,'floor_r',function(address,data,mask)
   if in_lua then return end
   local pc=mstate['CURPC'].value
   if (address>=0x10c7b6 and address<0x10e83e) or (pc>=0x12800 and pc<0x13a00) then floor_log:write(string.format('R %x %x %x %x %x %x %x %x\n',pc,address,mask,data,mstate['A0'].value,mstate['D0'].value,mstate['D1'].value,mstate['D2'].value)) end
  end)
 end
 if floor_log then
  floor_log:write(string.format('P %d %d %d %x %d\n',f,cpu:read_i16(0x1014c4+0x28),cpu:read_i16(0x1014c4+0x2c),cpu:read_u32(0x1014c4+0x38),cpu:read_i16(0x10c72e+0x18)))
  if f>=tonumber(floor_hi) then floor_log:close();floor_log=nil;floor_tap:remove() end
 end
end
emu.register_frame_done(function()
 in_lua=true
 frame=frame+1
 if load_name and frame==1 then machine:load(load_name);frame=tonumber(os.getenv('MSX_FULL_LOAD_FRAME'));last_section=-3 end
 if dbg then dbg:command(string.format('temp9=%x',frame)) if frame==2000 or (load_name and frame==tonumber(os.getenv('MSX_FULL_LOAD_FRAME'))+1) then probes() end end
 if actions[frame] then local a=actions[frame]; if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame>=1550 and frame<=1700 then button('P1 A',1) end
 if frame==1710 then button('P1 A',0) end
 -- Reference-only overrides (capture_advance.lua addresses and values).
 if frame>=2000 then
  cpu:write_u8(0x1013de,0x5a)
  cpu:write_u8(0x101523,cpu:read_u8(0x101523)|0x40)
  cpu:write_u8(0x101549,0x99)
  cpu:write_u8(0x101288,2)
 end
 if frame==2100 then cpu:write_u8(0x10c71e,3) end
 local camera=cpu:read_u16(0x102036)
 local spc=cpu:read_u32(SCRIPT+0x6e)
 local sec=section_of(spc)
 if frame>=2190 then
  cpu:write_u8(0x10c725,0xff);cpu:write_u8(0x10c720,0x63)
  -- The vehicle slot's health and shells kept full (the traversal); off
  -- with MSX_FULL_VEHICLE_CHEAT=0 (a vehicle family trace: the slot bytes
  -- are the game's own, shells spent and damage taken).
  if os.getenv('MSX_FULL_VEHICLE_CHEAT')~='0' then cpu:write_u8(0x10c621,0x30);cpu:write_u8(0x10c627,0x63) end
  -- Capture only (MSX_FULL_VEHICLE_HEALTH=<frame>:<hex>): the mounted
  -- slot's health 10C621 (+18, copied to the vehicle's +4E) written once,
  -- as the gauge writes of capture_weapons.lua (B52: the next hit destroys).
  local hf,hv=string.match(os.getenv('MSX_FULL_VEHICLE_HEALTH') or '','(%d+):(%x+)')
  if hf and frame==tonumber(hf) then cpu:write_u8(0x10c621,tonumber(hv,16)) end
  -- Progress word 0x10E99E (equal to 0x102036 in Mission 1 only).
  -- Camera Y is left out: it follows the player's jumps and would reset the
  -- count forever; vertical paths advance the progress word itself.
  local key=string.format('%d %x %d %x',cpu:read_u16(0x10e99e),spc,sec,cpu:read_u32(0x10e964))
  if key==last_key then still=still+1 else still=0 end
  last_key=key
  if ride_plan and load_name and frame>tonumber(os.getenv('MSX_FULL_LOAD_FRAME')) then plan='ride';ride_inputs() else plan_inputs(cpu:read_u16(0x101500)&511) end
  -- MSX_FULL_IDLE="lo-hi": capture-only, no input in those frames (an idle
  -- player for the GO arrow 05287C).
  local il,ih=string.match(os.getenv('MSX_FULL_IDLE') or '','(%d+)-(%d+)')
  if il and frame>=tonumber(il) and frame<=tonumber(ih) then
   for _,b in ipairs({'P1 Left','P1 Right','P1 Up','P1 Down','P1 A','P1 B','P1 C'}) do button(b,0) end
  end
  -- MSX_FULL_NOFIRE="lo-hi": capture-only, the plan's pad without A (fire)
  -- and C (grenade) in those frames (enemies the plan kills at once: the
  -- Mission 4 flyers 083C8A land, drop their rider and crash).
  local nl,nh=string.match(os.getenv('MSX_FULL_NOFIRE') or '','(%d+)-(%d+)')
  if nl and frame>=tonumber(nl) and frame<=tonumber(nh) then button('P1 A',0);button('P1 C',0) end
  if still>=stall_limit then print('STALL '..frame);last_frame=frame end
 end
 if frame>=2100 then
  if sec~=last_section and spc~=0 and (not load_name or frame>tonumber(os.getenv('MSX_FULL_LOAD_FRAME'))+2) then
   seclog:write(string.format('%d,%d,%x,%d\n',frame,camera,spc,sec));seclog:flush()
   print(string.format('SECTION %d frame %d pc %x',sec,frame,spc))
   -- The ending script (table entry 22) followed by an attract-mode demo
   -- script (20/21): the game is over; keep 600 frames and stop.
   if (sec==20 or sec==21) and seen_ending and last_frame>frame+600 then last_frame=frame+600 end
   if sec==22 then seen_ending=true end
   last_section=sec;section_start=frame
   -- Iteration aid only (MSX_FULL_SAVE=1): a MAME state per section start.
   if os.getenv('MSX_FULL_SAVE')=='1' and sec>=0 and sec<30 and frame>(tonumber(os.getenv('MSX_FULL_LOAD_FRAME') or '0'))+10 then machine:save(string.format('sec%02d_%d',sec,frame)) end
  end
  -- Iteration aid only (MSX_FULL_SAVE_PROGRESS=<n>): one MAME state
  -- prog_<frame> the first frame the camera progress 10E99E reaches n.
  local sp=tonumber(os.getenv('MSX_FULL_SAVE_PROGRESS') or '')
  if sp and not saved_progress and cpu:read_u16(0x10e99e)>=sp then saved_progress=true;machine:save(string.format('prog_%d',frame));print('SAVED prog_'..frame) end
  -- Iteration aid only (MSX_FULL_SAVE_AT=<frame>[,<frame>...]): a MAME state at each frame.
  for h in string.gmatch(os.getenv('MSX_FULL_SAVE_AT') or '','(%d+)') do
   if frame==tonumber(h) then machine:save(string.format('at_%d',frame)) end
  end
  -- Capture only (MSX_FULL_GIVE_MUMMY=<frame>): the form request +94 = 3 at
  -- that frame, as a contact of kind 0x10 (the mummy's breath) does in
  -- 0C36C0 (tools/mame/capture_marco_catalog.lua MSX_GIVE_MUMMY); the
  -- original forms check 0C3428 then runs the transformation 0C17A0.
  if frame==tonumber(os.getenv('MSX_FULL_GIVE_MUMMY') or '-1') then cpu:write_u8(0x1014c4+0x94,3) end
  -- Capture only (MSX_FULL_GIVE_WEAPON=<frame>:<hex id>): the weapon id
  -- 10C71E at that frame (as the reference weapon at 2100); the ammo is the
  -- reference's 10C725 = FF of every frame.
  local gf,gid=string.match(os.getenv('MSX_FULL_GIVE_WEAPON') or '','(%d+):(%x+)')
  if gf and frame==tonumber(gf) then
   cpu:write_u8(0x10c71e,tonumber(gid,16))
   -- The pistol (id 0): no rounds left of the previous weapon (10C724).
   if tonumber(gid,16)==0 then cpu:write_u16(0x10c724,0) end
  end
  -- Capture only (MSX_FULL_GIVE_FORM=<frame>:<form>): the form request +94
  -- (0 leaves the fat form 2: 0C1D82 and 0C5064 of 0C359A, G7).
  local ff,fv=string.match(os.getenv('MSX_FULL_GIVE_FORM') or '','(%d+):(%x+)')
  if ff and frame==tonumber(ff) then cpu:write_u8(0x1014c4+0x94,tonumber(fv,16)) end
  -- Capture only (MSX_FULL_GIVE_BOMB=<frame>:<type>): player 1's bomb type
  -- 10C722 (0C4786: 1 grenade, 2 molotov, 3 stone; as an item of kind 09
  -- gives 2) and 10 bombs 10C720 at that frame (the fat molotov 01E680).
  local bf,bt=string.match(os.getenv('MSX_FULL_GIVE_BOMB') or '','(%d+):(%x+)')
  if bf and frame==tonumber(bf) then cpu:write_u8(0x10c722,tonumber(bt,16));cpu:write_u8(0x10c720,10) end
  -- Pistol again after a new section (reference-only weapon, as at 2100).
  if frame==section_start+300 and frame>2400 and cpu:read_u8(0x10c71e)==0 then cpu:write_u8(0x10c71e,3) end
  local b=0x10c72e;local m=0x10d7b6;local sky=0x10e83e
  local g={}
  for i=0,8 do g[#g+1]=string.format('%d',cpu:read_u8(0x10c326+i)) end
  local inputs=(pressed['P1 Left']==1 and 'L' or '')..(pressed['P1 Right']==1 and 'R' or '')..(pressed['P1 Up']==1 and 'U' or '')..(pressed['P1 Down']==1 and 'D' or '')..(pressed['P1 A']==1 and 'A' or '')..(pressed['P1 B']==1 and 'B' or '')..(pressed['P1 C']==1 and 'C' or '')
  local lp=string.format('%d %d %d %d',cpu:read_i16(0x10e9c0),cpu:read_i16(0x10e9c2),cpu:read_i16(0x10e9c4),cpu:read_i16(0x10e9c6))
  local auto=string.format('%d %d %d %d',cpu:read_u8(0x10e95a),cpu:read_u8(0x10e95b),cpu:read_i32(0x10e95c),cpu:read_i32(0x10e960))
  local bounds=string.format('%d %d %d %d',cpu:read_i16(0x10e9d2),cpu:read_i16(0x10e9d4),cpu:read_i16(0x10e9d6),cpu:read_i16(0x10e9d8))
  trace:write(string.format('%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%d,%d,%d,%s,%x,%u,%u,%x,%x,%d,%s,%s,%d,%x,%d,%d,%d,%s,%s\n',frame,camera,cpu:read_u16(0x10e99e),
   cpu:read_i16(b+0x84),cpu:read_i16(b+0x86),cpu:read_i16(m+0x84),cpu:read_i16(m+0x86),cpu:read_i16(sky+0x84),cpu:read_i16(sky+0x86),
   cpu:read_i16(0x10e8c6+0x84),cpu:read_i16(0x10e8c6+0x86),
   cpu:read_i32(b+0x14)==cpu:read_i32(m+0x14) and 1 or 0,cpu:read_i32(0x10e97c),cpu:read_i32(0x10e980),cpu:read_i32(0x10e9bc),lp,auto,cpu:read_i32(0x10e98e),cpu:read_i32(0x10e992),cpu:read_i32(0x10e9cc),bounds,cpu:read_u32(0x10e964),
   cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x1014c4),spc,cpu:read_u16(SCRIPT+0x72),table.concat(g,' '),inputs,
   sec,cpu:read_u8(0x101523),cpu:read_i16(0x101500),cpu:read_i16(0x101502),cpu:read_u8(0x10c71e),plan,palette_hash()))
  if spc~=last_script then last_script=spc;scriptlog:write(string.format('%d,%d,%x,%x\n',frame,camera,SCRIPT,spc)) end
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
  -- While the progress key is unchanged for 20 s, snapshot only every
  -- 10 periods (the view repeats; keeps long stalls from filling the disk).
  local sparse=still>=1200
  if frame>=first_dump and (frame==2100 or (frame%period==0 and (not sparse or frame%(period*10)==0))) then dump(frame) end
  if prev_dumps and frame>=first_dump and (frame+1)%period==0 and (still<1199 or (frame+1)%(period*10)==0) then dump_vram(output..'/prev/vram'..(frame+1)..'.bin') end
  if frame%600==0 then print('FRAME '..frame..' camera '..camera..' section '..sec..' plan '..plan..' still '..still..string.format(' script %x',spc));trace:flush();actors:flush();scriptlog:flush();if events then events:flush() end end
 end
 fam_frame(frame)
 cell_frame_done(frame)
 floor_frame(frame)
 in_lua=false
 if frame>=last_frame then
  if fam_log then fam_log:close() end
  for _,f in ipairs({trace,actors,scriptlog,seclog,layers,states,platforms,lspc,lspc_writes}) do f:close() end
  if events then events:close() end
  machine:exit()
 end
end,'frame')
