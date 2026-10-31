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
 local regs='temp9,curpc,a6,a0,a1,a4,d0,d1,d@(sp)'
 local fmt='frame=%d pc=%06X a6=%08X a0=%08X a1=%08X a4=%08X d0=%08X d1=%08X ret=%08X'
 local list={0xc702a,0xc85f0,0xc8646,0xc865a,0xc88b0,0xc8764,0xc8084,0x9d8c,0x1aa28}
 for a in string.gmatch(os.getenv('MSX_FULL_EXTRA_PROBES') or '','(%x+)') do list[#list+1]=tonumber(a,16) end
 for _,a in ipairs(list) do
  dbg:command(string.format('bpset %x,1,{printf "BP_%06X ',a,a)..fmt..'\\n",'..regs..';g}')
 end
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
local function plan_inputs(screen_x)
 local px=cpu:read_i16(0x1014ec);local py=cpu:read_i16(0x1014f0)
 if px==last_px then blocked=blocked+1 else blocked=0 end
 last_px=px
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
-- MSX_FULL_LOAD=<state name> MSX_FULL_LOAD_FRAME=<frame>: resume from a state
-- saved by MSX_FULL_SAVE (iteration only; canonical captures run from boot).
local load_name=os.getenv('MSX_FULL_LOAD')
emu.register_frame_done(function()
 frame=frame+1
 if load_name and frame==1 then machine:load(load_name);frame=tonumber(os.getenv('MSX_FULL_LOAD_FRAME'));last_section=-3 end
 if dbg then dbg:command(string.format('temp9=%x',frame)) if frame==2000 then probes() end end
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
  cpu:write_u8(0x10c621,0x30);cpu:write_u8(0x10c627,0x63)
  -- Progress word 0x10E99E (equal to 0x102036 in Mission 1 only).
  -- Camera Y is left out: it follows the player's jumps and would reset the
  -- count forever; vertical paths advance the progress word itself.
  local key=string.format('%d %x %d %x',cpu:read_u16(0x10e99e),spc,sec,cpu:read_u32(0x10e964))
  if key==last_key then still=still+1 else still=0 end
  last_key=key
  plan_inputs(cpu:read_u16(0x101500)&511)
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
 if frame>=last_frame then
  for _,f in ipairs({trace,actors,scriptlog,seclog,layers,states,platforms,lspc,lspc_writes}) do f:close() end
  if events then events:close() end
  machine:exit()
 end
end,'frame')
