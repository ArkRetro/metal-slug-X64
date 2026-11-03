-- MAME 0.264, mslugx euro BIOS. Player weapons and vehicles (docs/ARMAS_VEHICULOS.md).
-- Run with tools/run_capture_weapons.sh (needs -debug for the breakpoint log).
--
-- Reference-only overrides, as tools/capture_mission_rest.lua: mission timer
-- 1013DE, P1 invulnerability bit (101523 |= 0x40), 101549 and 101288. Unlike
-- that script NOTHING writes the weapon block 10C718..10C72F, the bombs or the
-- vehicle slots 10C608/10C62C: weapons, ammunition, bombs and vehicles are
-- the game's own. Nothing writes level, script, enemy or collision state.
--
-- MSX_WEAPONS_ASSIST_CAMERA=<camera>: progression assist, reference-only and
-- identical to tools/capture_mission_rest.lua (weapon 10C71E=3, ammo low byte
-- 10C725=FF, bombs 10C720=0x63) but ONLY while the camera is below <camera>.
-- At the first frame at or past it the ammo word 10C724 is written 0 and the
-- bombs 10C720 the initial 10 (ROM 0C0E80); from then on nothing is written
-- and the game's own expiry (0C3388 -> 0C0EE2) brings back the pistol.
-- With the pistol alone the dome stop at camera 1920 did not end in 12000
-- frames (run pickups), so the vehicles and the H need it.
-- MSX_WEAPONS_PLAN:
--   pickups   ride each vehicle MSX_WEAPONS_RIDE (default 60) updates, then
--             leave it (A+B), so most of the run is on foot: items, the H,
--             grenades and the expiry back to the pistol.
--   vehicles  ride each vehicle MSX_WEAPONS_RIDE (default 720) updates firing
--             A (main gun / vulcan) and C (bombs / cannon), aiming up half of
--             the time, then leave it with A+B.
-- A and B are never pressed together on foot (A+B is mask 0x30, input bit 7,
-- the vehicle exit of ROM 035168), B only when A is released.
--
-- Outputs (MSX_CAPTURE_DIR):
--  weapons.csv   every frame from 2100: camera, inputs, input word 10E9EC,
--                P1 actor (1014C4) routine/+15/+4E/+5F/+6C/+90/+92..+95/+9A,
--                position, weapon block 10C718..10C72F, vehicle slot P1
--                10C608..10C62B, record pointer 10C2E8 (set by ROM 0AA83A),
--                frame counter 10000E, timer 1013DE, score 10C46A, prisoners 10C5FC
--  actors.csv    actor births/routine changes (with +6E: item kind, angle)
--  actorstate.csv every second frame from MSX_WEAPONS_STATE_FROM (6800)
--  trace.csv     the family-pipeline trace columns of capture_mission_rest.lua
--  debug.log     BP lines (MAME debugger printf): pickups, weapon grant,
--                pistol reset, fire, grenades, vehicle bind/unbind/HUD, and
--                allocations (0x411C/0x40BC) by the player or a vehicle
--  vram|ram|palette|game<frame>  every 30 frames from MSX_WEAPONS_FIRST_DUMP
--                (6900) and on weapon/bomb/vehicle-slot changes
--  MSX_WEAPONS_ALLOC_ALL=lo-hi  log every allocation (0x411C/0x40BC) in that window
--  boss-trace.txt MSX_WEAPONS_FAMILY_FROM..TO: family trace (same format as
--                capture_mission_rest.lua) of MSX_WEAPONS_FAMILY_ROOTS plus the
--                fixed nodes MSX_WEAPONS_FAMILY_NODES (default the player 1014c4);
--                a node allocated through ROM 00411C joins with its real creator
--                (the A6 that 00411C saved), so the projectiles are traced
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local plan=os.getenv('MSX_WEAPONS_PLAN') or 'pickups'
local ride_jumps=os.getenv('MSX_WEAPONS_RIDE_JUMPS')
local ride_len=tonumber(os.getenv('MSX_WEAPONS_RIDE') or (plan=='vehicles' and '720' or '60'))
local last_frame=tonumber(os.getenv('MSX_WEAPONS_LAST') or '11000')
local first_dump=tonumber(os.getenv('MSX_WEAPONS_FIRST_DUMP') or '6900')
local state_from=tonumber(os.getenv('MSX_WEAPONS_STATE_FROM') or '6800')
local period=tonumber(os.getenv('MSX_WEAPONS_PERIOD') or '30')
local family_from=tonumber(os.getenv('MSX_WEAPONS_FAMILY_FROM') or '0')
local family_to=tonumber(os.getenv('MSX_WEAPONS_FAMILY_TO') or '0')
local assist_camera=tonumber(os.getenv('MSX_WEAPONS_ASSIST_CAMERA') or '0')
local assisting=assist_camera>0
-- MSX_WEAPONS_ASSIST_ID=<hex id> (default 3): the weapon the assist writes.
-- With it set, the first assist frame also writes the cap 10C71C from ROM
-- table 0C08A8 and the form request +94 from 0C0918 (as 0C0928 does).
local assist_id=tonumber(os.getenv('MSX_WEAPONS_ASSIST_ID') or '3',16)
local assist_first=os.getenv('MSX_WEAPONS_ASSIST_ID')~=nil
-- MSX_WEAPONS_KEEP_TIMER=1: do not hold the mission timer 1013DE (HUD timer
-- digits, docs/ARMAS_VEHICULOS.md §11); everything else as without it.
local keep_timer=os.getenv('MSX_WEAPONS_KEEP_TIMER')=='1'
-- MSX_WEAPONS_MORTAL_CAMERA=<camera>: from the first frame at or past it the
-- P1 invulnerability bit and count (+84) are no longer forced (bit cleared once) so the player
-- can die (lives 101288 stay forced); deaths.csv then logs the player's
-- death/respawn fields every frame (docs/JEFE_MISION1.md, respawn).
local mortal_camera=tonumber(os.getenv('MSX_WEAPONS_MORTAL_CAMERA') or '0')
local mortal=false
-- MSX_WEAPONS_GAUGE=frame:health,..: reference-only HUD gauge check
-- (docs/ARMAS_VEHICULOS.md §11, B41): at each frame the P1 vehicle slot health
-- +18 (10C620) is written, while a vehicle is ridden (slot +00 nonzero); the
-- vehicle copies it to its +4E (0AAA74) and the gauge 0C9B94 shows it.
local gauge_pokes={}
for f,h in (os.getenv('MSX_WEAPONS_GAUGE') or ''):gmatch('(%d+):(%x+)') do gauge_pokes[tonumber(f)]=tonumber(h,16) end
local frame=0
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local mstate=manager.machine.devices[':maincpu'].state
local ROOT,DEAD,SCRIPT,P1=0x1010a4,0x3f1e,0x101414,0x1014c4
local function hexbytes(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
local wcsv=assert(io.open(output..'/weapons.csv','w'))
wcsv:write('frame,camera,inputs,input_word,routine,flags15,health,f5f,f6c,f90,f92,f93,f94,f95,f9a,x16,y16,weapon_block,slot_p1,hud,video,timer,score,rescued\n')
local deaths=nil
local actors=assert(io.open(output..'/actors.csv','w'))
actors:write('frame,camera,node,event,routine,previous,creator,parent,x,y,kind6e,word70\n')
local states=assert(io.open(output..'/actorstate.csv','w'))
states:write('frame,node,routine,anim,x,y,health,creator,palette,w6e\n')
local trace=assert(io.open(output..'/trace.csv','w'))
trace:write('frame,camera,camera_word,x16,y16,player_routine,script_pc,script_timer,groups,inputs\n')
local known={}
local function walk(f,camera)
 local n=ROOT;local seen={};local count=0
 while n~=0 and n>=0x100000 and n<0x110000 and not seen[n] and count<400 do
  seen[n]=true;count=count+1
  local r=cpu:read_u32(n)
  local old=known[n]
  if r~=old then
   local ev=(old==nil or old==DEAD) and 'birth' or (r==DEAD and 'death' or 'state')
   actors:write(string.format('%d,%d,%x,%s,%x,%x,%x,%x,%d,%d,%x,%x\n',f,camera,n,ev,r,old or 0,
    cpu:read_u32(n+0x0c),cpu:read_u32(n+0x38),cpu:read_i16(n+0x28),cpu:read_i16(n+0x2c),cpu:read_u16(n+0x6e),cpu:read_u16(n+0x70)))
   known[n]=r
  end
  if f>=state_from and f%2==0 and r~=DEAD then
   states:write(string.format('%d,%x,%x,%x,%d,%d,%d,%x,%x,%x\n',f,n,r,cpu:read_u32(n+0x20),cpu:read_i16(n+0x28),cpu:read_i16(n+0x2c),cpu:read_u16(n+0x4e),cpu:read_u32(n+0x0c),cpu:read_u8(n+0x19),cpu:read_u16(n+0x6e)))
  end
  n=cpu:read_u32(n+8)
 end
end
local dumping=false
local function dump(f)
 dumping=true
 local v=assert(io.open(output..'/vram'..f..'.bin','wb'))
 for i=0,0x85ff do
  cpu:write_u16(0x3c0000,i)
  v:write(string.pack('>I2',cpu:read_u16(0x3c0002)))
 end
 v:close()
 local ram=assert(io.open(output..'/ram'..f..'.bin','wb'));ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 local p=assert(io.open(output..'/palette'..f..'.bin','wb'))
 for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
 p:close()
 manager.machine.screens[':screen']:snapshot(output..'/game'..f..'.png')
 dumping=false
end
local bank_sum=nil
local function check_bank(f)
 local s=0
 for a=0x200000,0x2fffff,0x1000 do s=(s*31+cpu:read_u16(a))%4294967291 end
 if s~=bank_sum then
  bank_sum=s
  local b=assert(io.open(output..'/bank'..f..'.bin','wb'));b:write(cpu:read_range(0x200000,0x2fffff,8));b:close()
 end
end
-- Breakpoint probes (print only, resume with g). temp9 holds the frame.
local dbg=manager.machine.debugger
local function probes()
 local function bp(addr,cond,text,regs)
  dbg:command(string.format('bpset %x,%s,{printf "BP_%06X frame=%%d %s\\n",temp9,%s;g}',addr,cond,addr,text,regs))
 end
 -- Item pickup routines (+10 of the 0E581A records).
 bp(0x8b5bc,'1','weapon_item d0=%X d1=%X a6=%08X kind=%X','d0,d1,a6,w@(a6+6e)')
 bp(0x8b6d0,'1','bomb_item d0=%X a6=%08X kind=%X','d0,a6,w@(a6+6e)')
 bp(0x8b612,'1','bombtype_item d0=%X d1=%X a6=%08X kind=%X','d0,d1,a6,w@(a6+6e)')
 bp(0x8b7fc,'1','points_item d0=%X a6=%08X kind=%X rec=%08X','d0,a6,w@(a6+6e),a0')
 bp(0x8b896,'1','score_popup d0=%X rec=%08X value=%08X','d0,a0,d@(a0+c)')
 -- Marco catalog mode (MSX_WEAPONS_MARCO_DUMP=lo-hi,..): the player's draws
 -- and program starts in the format of tools/mame/capture_marco_catalog.lua.
 if os.getenv('MSX_WEAPONS_MARCO_DUMP') then
  dbg:command('bpset 4406,a6==1014c4,{printf "MARCO_DRAW frame=%d descriptor=%08X xy=%08X attr=%04X z=%04X\\n",temp9,d7,d0,d1&ffff,d5&ffff;g}')
  dbg:command('bpset 4256,a6==1014c4,{printf "MARCO_INIT frame=%d program=%08X speed=%04X\\n",temp9,a1,d0&ffff;g}')
 end
 -- Extra debugger commands (MSX_WEAPONS_DEBUG_CMDS, separated by '|'):
 -- one-off kernel traces such as the SV-001 tread sweeps.
 for c in string.gmatch(os.getenv('MSX_WEAPONS_DEBUG_CMDS') or '','[^|]+') do dbg:command(c) end
 -- Weapon state.
 bp(0xc0928,'1','give_weapon d0=%X d1=%X old=%X ammo=%X','d0,d1,b@10c71e,w@10c724')
 bp(0xc09c8,'1','give_weapon_done id=%X ammo=%X max=%X','b@10c71e,w@10c724,b@10c71c')
 bp(0xc0b32,'1','give_bombs d0=%X d1=%X bombs=%X','d0,d1,b@10c720')
 bp(0xc0ee2,'1','pistol_reset a6=%08X ret=%08X id=%X ammo=%X','a6,d@(sp),b@10c71e,w@10c724')
 bp(0xc0e86,'1','player_init a6=%08X','a6')
 bp(0xc2d6a,'1','player_death a6=%08X x=%04X y=%04X f15=%X','a6,w@(a6+3c),w@(a6+40),b@(a6+15)')
 bp(0x196e8,'1','respawn a6=%08X rec=%08X c31e=%08X progress=%X','a6,d@10c31a,d@10c31e,w@10e99e')
 -- Fire: 0C438E ammo granted, table in A0; 0C4454/0C447C H; 0C46F4 laser-type.
 bp(0xc438e,'a6==1014c4','fire table=%08X id=%X ammo=%X shots=%X','a0,b@10c71e,w@10c724,b@10c71a')
 bp(0x1b782,'1','bullet_pistol a6=%08X','a6')
 bp(0x1b716,'1','bullet_hA a6=%08X','a6')
 bp(0xc4454,'a6==1014c4','fire_h5 f93=%X','b@(a6+93)')
 bp(0xc447c,'a6==1014c4','fire_hA f93=%X','b@(a6+93)')
 bp(0xc46f4,'a6==1014c4','fire_beam id=%X','b@10c71e')
 -- Grenades: 0C4786 (bomb type table), 0C48C0 (1BE56), 0C0808 (vehicles).
 bp(0xc4786,'1','grenade_typed a6=%08X bombs=%X type=%X f90=%X f93=%X','a6,b@10c720,b@10c722,b@(a6+90),b@(a6+93)')
 bp(0xc48c0,'1','grenade_plain a6=%08X bombs=%X','a6,b@10c720')
 bp(0xc0808,'1','vehicle_bomb a6=%08X d0=%X bombs=%X type=%X','a6,d0,b@10c720,b@10c722')
 bp(0xaa956,'1','vehicle_shell a6=%08X d0=%X shells=%X','a6,d0,b@10c627')
 -- Vehicle system.
 bp(0xaa65a,'1','select_player d0=%X a6=%08X','d0,a6')
 bp(0xaa66a,'1','select_player d0=%X a6=%08X','d0,a6')
 bp(0xaa6a0,'1','select_player_rand d0=%X a6=%08X','d0,a6')
 bp(0xaa6ac,'1','select_player d0=%X a6=%08X','d0,a6')
 bp(0xaa6b4,'1','select_player d0=%X a6=%08X','d0,a6')
 bp(0xaa7a4,'1','bind a6=%08X d2=%X d3=%X slot=%X','a6,d2,d3,w@10c608')
 bp(0xaa81e,'1','unbind a6=%08X kind=%X','a6,d1')
 -- 0AA83A: vehicle stats into the slot (label kept for existing logs).
 bp(0xaa83a,'1','vehicle_hud a6=%08X d0=%X kind=%X ammo=%X type=%X d4=%X health=%X','a6,d0,d1,d2,d3,d4,w@(a6+4e)')
 bp(0xaa8dc,'1','vehicle_item_kind4 d1=%X','d1')
 bp(0xaa93c,'1','vehicle_bombs_added d1=%X','d1')
 bp(0xc0bf4,'1','food_item d0=%X food=%X','d0,w@10c728')
 -- Player side of the binding: 0C30F0 reads the exit kind (+1D) and cause (+1E).
 bp(0xc30fc,'1','player_exit kind=%X cause=%X slot1d=%X','d0,d1,b@10c625')
  -- Camel Slug: grenade/bomb button 035100, exit by A+B 034BA0, by down+B 0351E6, hit 0351BC.
 bp(0x34ba0,'1','camel_eject_AB a6=%08X','a6')
 bp(0x351e6,'1','camel_exit_downB a6=%08X','a6')
 bp(0x351bc,'1','camel_killed a6=%08X cause=%X','a6,b@(a6+5a)')
 bp(0x3514a,'1','camel_fire_nobomb a6=%08X','a6')
 -- SV-001: cannon 092934 (shells), bombs 0929E6, exit 093A08.
 bp(0x9294a,'1','tank_cannon a6=%08X shells=%X type=%X','a6,b@10c627,b@(a6+7c)')
 bp(0x929ee,'1','tank_bomb a6=%08X','a6')
 bp(0x93a08,'1','tank_exit_AB a6=%08X health=%X','a6,w@(a6+4e)')
 -- Targets of the homing projectiles: the 0049E8 query result of the C
 -- (063FDC; A1 the alternative of a negative +8B) and of the rocket (0638B4).
 bp(0x63fe6,'1','chaser_target a6=%08X a0=%08X a1=%08X r8b=%X a0x=%08X a0y=%08X a1x=%08X a1y=%08X','a6,a0,a1,b@(a6+8b),d@(a0+3c),d@(a0+40),d@(a1+3c),d@(a1+40)')
 bp(0x638be,'1','rocket_target a6=%08X a0=%08X a0x=%08X a0y=%08X','a6,a0,d@(a0+3c),d@(a0+40)')
 -- Allocations by the player or by a vehicle (A6 the creator).
 local who='(a6==1014c4)||((d@(a6)>=34000)&&(d@(a6)<36000))||((d@(a6)>=90000)&&(d@(a6)<98000))'
 bp(0x411c,who,'alloc ctor=%08X creator=%08X croutine=%08X','a0,a6,d@(a6)')
 bp(0x40bc,who,'alloc2 ctor=%08X creator=%08X croutine=%08X','a0,a6,d@(a6)')
 -- MSX_WEAPONS_ALLOC_ALL=lo-hi: every allocation in that frame window.
 local lo,hi=string.match(os.getenv('MSX_WEAPONS_ALLOC_ALL') or '','(%d+)-(%d+)')
 if lo then
  local w=string.format('(temp9>=%x)&&(temp9<=%x)',tonumber(lo),tonumber(hi))
  bp(0x411c,w,'any_alloc ctor=%08X creator=%08X croutine=%08X','a0,a6,d@(a6)')
  bp(0x40bc,w,'any_alloc2 ctor=%08X creator=%08X croutine=%08X','a0,a6,d@(a6)')
 end
end
-- Family trace (same format as tools/capture_mission_rest.lua).
local family_roots={}
for h in string.gmatch(os.getenv('MSX_WEAPONS_FAMILY_ROOTS') or '3458e,92a32,8c37c,8c374','(%x+)') do family_roots[tonumber(h,16)]=true end
local family={}
for h in string.gmatch(os.getenv('MSX_WEAPONS_FAMILY_NODES') or '1014c4','(%x+)') do family[tonumber(h,16)]=true end
local boss_log=nil;local in_lua=true;local active=nil;local attacker_dumped={}
local POOL0,POOL1=0x1010a4,0x1010a4+0xb0*100
local function boss_read(address,data,mask)
 if in_lua or dumping then return end
 if address>=POOL0 and address<POOL1 then
  local o=(address-POOL0)%0xb0
  if o==0 then
   if mstate['CURPC'].value==0x4246 then
    local n=address;in_lua=true
    local r=cpu:read_u32(n)
    if family_roots[r] then family[n]=true end
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
  -- ROM 00411C allocates with A6 = the list head (+0C = 101414 in the node);
  -- the real creator is the A6 it saved: (SP+8) when (SP+4) is 004122.
  if a6==0x101414 then
   in_lua=true
   local reg=mstate['SP'] or mstate['A7'] or mstate['SSP']
   if reg then
    local sp=reg.value&0xffffff
    if cpu:read_u32(sp+4)==0x4122 then a6=cpu:read_u32(sp+8) end
   end
   in_lua=false
  end
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
local function family_frame(f)
 if family_from<=0 then return end
 if f==family_from then
  boss_log=assert(io.open(output..'/boss-trace.txt','w'))
  fam_rtap=cpu:install_read_tap(0x100000,0x10ffff,'wpn_r',boss_read)
  fam_wtap=cpu:install_write_tap(0x100000,0x10ffff,'wpn_w',boss_write)
 end
 if boss_log then
  boss_log:write(string.format('F %d\n',f))
  if f%600==0 then boss_log:flush() end
  if f==family_to then fam_rtap:remove();fam_wtap:remove();boss_log:close();boss_log=nil;print('FAMILY END '..f) end
 end
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
-- Marco catalog mode: while the player's routine is in a range (and the 4
-- frames after: the submission delay), the VRAM (video item, no register
-- write), palette and screen of every frame, plus states.jsonl and the
-- program banks, for tools/audit_marco_catalog.py --sparse (plan.json is
-- written by the caller).
local marco_ranges=os.getenv('MSX_WEAPONS_MARCO_DUMP');local marco_until=nil;local marco_states=nil
local function marco_dump(f)
 if not marco_ranges or f<2100 then return end
 if not marco_states then marco_states=assert(io.open(output..'/states.jsonl','w')) end
 local P=0x1014c4;local routine=cpu:read_u32(P)
 marco_states:write(string.format('{"frame":%d,"routine":%d,"cursor":%d,"x":%d,"y":%d,"projected_x":%d,"projected_y":%d,"flags":%d,"f5a":%d}\n',f,routine,cpu:read_u32(P+0x20),cpu:read_u32(P+0x28),cpu:read_u32(P+0x2c),cpu:read_u32(P+0x3c),cpu:read_u32(P+0x40),cpu:read_u16(P+0x14),cpu:read_u8(P+0x5a)))
 if f==2102 then
  for _,r in ipairs({{'program-fixed.bin',0,0xfffff},{'program-bank.bin',0x200000,0x2fffff}}) do
   local file=assert(io.open(output..'/'..r[1],'wb'));file:write(cpu:read_range(r[2],r[3],8));file:close()
  end
 end
 for lo,hi in marco_ranges:gmatch('(%x+)-(%x+)') do if routine>=tonumber(lo,16) and routine<=tonumber(hi,16) then marco_until=f+4 end end
 if not marco_until or f>marco_until then return end
 local video=emu.item(assert(manager.machine.devices[':spritegen'].items['0/m_videoram']))
 local v=assert(io.open(output..'/vram'..f..'.bin','wb'));for i=0,0x85ff do v:write(string.pack('>I2',video:read(i))) end;v:close()
 local pf=assert(io.open(output..'/palette'..f..'.bin','wb'));for i=0,4095 do pf:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end;pf:close()
 manager.machine.screens[':screen']:snapshot(output..'/game'..f..'.png')
end
local still=0;local last_camera=-1;local sweep_direction=1
local mounted_at=nil;local mounted_slot=0;local leave_until=0
local last_key=nil;local next_event_dump=0
emu.register_frame_done(function()
 in_lua=true
 frame=frame+1
 dbg:command(string.format('temp9=%x',frame))
 if frame==2000 then probes() end
 if actions[frame] then local a=actions[frame]; if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame>=1550 and frame<=1700 then button('P1 A',1) end
 if frame==1710 then button('P1 A',0) end
 if frame>=2000 then
  if not keep_timer then cpu:write_u8(0x1013de,0x5a) end
  if mortal_camera>0 and not mortal and cpu:read_u16(0x102036)>=mortal_camera then
   mortal=true;cpu:write_u8(0x101523,cpu:read_u8(0x101523)&0xbf);print('MORTAL '..frame)
  end
  if not mortal then cpu:write_u8(0x101523,cpu:read_u8(0x101523)|0x40) end -- P1 invulnerability, capture only.
  if not mortal then cpu:write_u8(0x101549,0x99) end -- P1 +84 low byte (invulnerability count), capture only.
  cpu:write_u8(0x101288,2)
  if gauge_pokes[frame] and cpu:read_u16(0x10c608)~=0 then cpu:write_u16(0x10c620,gauge_pokes[frame]);print('GAUGE '..frame..' '..gauge_pokes[frame]) end
 end
 local camera=cpu:read_u16(0x102036)
 if assisting and frame>=2100 then
  if camera<assist_camera then
   if assist_first then
    assist_first=false
    cpu:write_u8(0x10c71c,cpu:read_u8(0x0c08a8+assist_id));cpu:write_u8(P1+0x94,cpu:read_u8(0x0c0918+assist_id))
    print('ASSIST ID '..string.format('%x',assist_id)..' '..frame)
   end
   cpu:write_u8(0x10c71e,assist_id);cpu:write_u8(0x10c725,0xff);cpu:write_u8(0x10c720,0x63)
  else
   cpu:write_u16(0x10c724,0);cpu:write_u8(0x10c720,10);assisting=false
   print('ASSIST END '..frame..' camera '..camera)
  end
 end
 if plan=='hold' and frame>=2190 then
  -- Fire semantics plan (docs/ARMAS_VEHICULOS.md §3): the player stands at
  -- the mission start and HOLDS A (released 20 frames every 300) in the four
  -- stances, with the pistol and then the H (id 0A, 200 rounds, given at
  -- 3400 like a pickup), then taps A every 4 frames; nothing else moves.
  local t=frame-2190
  local phase=t//300
  if frame==3400 then cpu:write_u8(0x10c71e,0x0a);cpu:write_u16(0x10c724,200);cpu:write_u8(0x10c71c,0xff);cpu:write_u8(P1+0x94,1) end
  local a=(t%300<280) and 1 or 0
  if phase>=8 then a=(frame%4<2) and 1 or 0 end
  local up=(phase%4==2) and 1 or 0
  local down=(phase%4==1) and 1 or 0
  local b=(phase%4==3 and frame%60<4) and 1 or 0
  button('P1 Right',0);button('P1 Left',0);button('P1 Up',up);button('P1 Down',down)
  button('P1 A',a);button('P1 B',b);button('P1 C',0)
 elseif plan=='air' and frame>=2190 then
  -- Aiming in the air and H sweeps (docs/ARMAS_VEHICULOS.md, down-aim):
  -- 3 weapons x 960 frames (pistol from 2190; the H (0A, 200 rounds) given
  -- at 3150; the C (0C, 40 rounds, cap 3) at 4110, like a pickup), each in
  -- four 240-frame stances repeated twice facing right then left: (0)
  -- standing jumps (B every 60 frames) holding Down from the take-off,
  -- (1) running jumps holding Down, (2) walking with Up held 24 of every 48
  -- frames (sweeps), (3) standing jumps, Up for the first 16 frames of the
  -- jump then Down. A is tapped every 8 frames (4 down) throughout.
  local t=frame-2190
  local w=t//960;local s=(t%960)//240;local u=t%240;local j=u%60
  if frame==3150 then cpu:write_u8(0x10c71e,0x0a);cpu:write_u16(0x10c724,200);cpu:write_u8(0x10c71c,0xff);cpu:write_u8(P1+0x94,1) end
  if frame==4110 then cpu:write_u8(0x10c71e,0x0c);cpu:write_u16(0x10c724,40);cpu:write_u8(0x10c71c,3);cpu:write_u8(0x10c71a,0);cpu:write_u8(P1+0x94,1) end
  if w>=1 and frame<4110 and cpu:read_u16(0x10c724)<20 then cpu:write_u16(0x10c724,200) end
  if w>=2 and cpu:read_u16(0x10c724)<10 then cpu:write_u16(0x10c724,40) end
  local leftward=(u>=120)
  local right,left,up,down,b=0,0,0,0,0
  local a=(frame%8<4) and 1 or 0
  if s==0 then b=(j<4) and 1 or 0;down=(j>=6 and j<50) and 1 or 0;if j<2 then right=leftward and 0 or 1;left=leftward and 1 or 0 end
  elseif s==1 then b=(j<4) and 1 or 0;down=(j>=6 and j<50) and 1 or 0;right=leftward and 0 or 1;left=leftward and 1 or 0
  elseif s==2 then right=leftward and 0 or 1;left=leftward and 1 or 0;up=((u%48)<24) and 1 or 0
  else b=(j<4) and 1 or 0;up=(j<16) and 1 or 0;down=(j>=16 and j<50) and 1 or 0;if j<2 then right=leftward and 0 or 1;left=leftward and 1 or 0 end end
  button('P1 Right',right);button('P1 Left',left);button('P1 Up',up);button('P1 Down',down)
  button('P1 A',a);button('P1 B',b);button('P1 C',0)
 elseif frame>=2190 then
  if camera==last_camera then still=still+1 else still=0 end
  last_camera=camera
  local slot=cpu:read_u16(0x10c608);local slot_state=cpu:read_u8(0x10c608+0x1c)
  local riding=slot~=0 and slot_state~=2
  if riding and mounted_slot~=slot then mounted_slot=slot;mounted_at=frame;print(string.format('MOUNT %d %x',frame,slot)) end
  if not riding and mounted_slot~=0 then print(string.format('LEFT %d %x',frame,mounted_slot));mounted_slot=0;mounted_at=nil end
  local screen_x=cpu:read_u16(0x101500)&511
  local stopped=still>=240
  local right,left,up,down=1,0,0,0
  if stopped then
   if screen_x<=48 then sweep_direction=1 elseif screen_x>=256 then sweep_direction=-1 end
   right=sweep_direction>0 and 1 or 0;left=sweep_direction<0 and 1 or 0
   up=(frame//180)%2==0 and 1 or 0
  end
  local a=frame%12<6 and 1 or 0
  local b=(frame>3000 and frame%120>=6 and frame%120<12) and 1 or 0
  local c=(frame>2900 and frame%60<8) and 1 or 0
  if riding then
   local t=frame-mounted_at
   b=0
   if plan=='vehicles' then
    up=(t//90)%2==1 and 1 or 0
    c=(t%40<6) and 1 or 0
    -- Hold position for the first half, then advance.
    if t<ride_len//2 and not stopped then right=(t//60)%2==0 and 1 or 0;left=1-right end
    -- MSX_WEAPONS_RIDE_JUMPS=1: jumps (B) and crouches (down; with B: out)
    -- in the SV-001 (its 09333A and 093138).
    local veh=cpu:read_u32(0x100000+mounted_slot)
    if ride_jumps=='1' and veh>=0x92810 and veh<0x95800 then b=(t%100>=50 and t%100<54) and 1 or 0;down=(t%150>=100 and t%150<130) and 1 or 0;if b==1 then a=0 end end -- A+B would leave
    -- 2: long crouches (bombs with C) and separate jumps.
    if ride_jumps=='2' and veh>=0x92810 and veh<0x95800 then b=(t%200>=100 and t%200<104) and 1 or 0;down=(t%200>=20 and t%200<80) and 1 or 0;if b==1 then a=0 end end
   end
   if t>=ride_len and t<ride_len+6 then a=1;b=1;c=0;down=0 end -- leave: A+B (ROM 035168, input bit 7)
  end
  button('P1 Right',right);button('P1 Left',left);button('P1 Up',up)
  if ride_jumps then button('P1 Down',down) end
  button('P1 A',a);button('P1 B',b);button('P1 C',c)
 end
 if mortal then
  if not deaths then deaths=assert(io.open(output..'/deaths.csv','w'));deaths:write('frame,video,camera,progress,routine,f15,f5f,x28,y2c,v30,v34,x3c,y40,p38,f78,f80,f84,rec,table,lock,f5a,f5b,f26\n') end
  deaths:write(string.format('%d,%04x,%d,%04x,%x,%02x,%02x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%02x,%04x,%04x,%08x,%08x,%02x%02x,%02x,%02x,%04x\n',frame,cpu:read_u16(0x10000e),cpu:read_u16(0x102036),
   cpu:read_u16(0x10e99e),cpu:read_u32(P1),cpu:read_u8(P1+0x15),cpu:read_u8(P1+0x5f),cpu:read_u32(P1+0x28),cpu:read_u32(P1+0x2c),cpu:read_u32(P1+0x30),cpu:read_u32(P1+0x34),
   cpu:read_u32(P1+0x3c),cpu:read_u32(P1+0x40),cpu:read_u32(P1+0x38),cpu:read_u8(P1+0x78),cpu:read_u16(P1+0x80),cpu:read_u16(P1+0x84),cpu:read_u32(0x10c31a),cpu:read_u32(0x10c31e),
   cpu:read_u8(0x10c322),cpu:read_u8(0x10c323),cpu:read_u8(P1+0x5a),cpu:read_u8(P1+0x5b),cpu:read_u16(P1+0x26)))
 end
 if frame>=2100 then
  local inputs=(pressed['P1 Left']==1 and 'L' or '')..(pressed['P1 Right']==1 and 'R' or '')..(pressed['P1 Up']==1 and 'U' or '')..(pressed['P1 A']==1 and 'A' or '')..(pressed['P1 B']==1 and 'B' or '')..(pressed['P1 C']==1 and 'C' or '')
  local block=hexbytes(0x10c718,0x18);local slotp=hexbytes(0x10c608,0x24)
  wcsv:write(string.format('%d,%d,%s,%04x%04x,%x,%02x,%d,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%u,%u,%s,%s,%x,%04x,%04x,%08x,%04x\n',frame,camera,inputs,
   cpu:read_u16(0x10e9ec),cpu:read_u16(0x10e9ee),cpu:read_u32(P1),cpu:read_u8(P1+0x15),cpu:read_u16(P1+0x4e),cpu:read_u8(P1+0x5f),cpu:read_u8(P1+0x6c),
   cpu:read_u8(P1+0x90),cpu:read_u8(P1+0x92),cpu:read_u8(P1+0x93),cpu:read_u8(P1+0x94),cpu:read_u8(P1+0x95),cpu:read_u8(P1+0x9a),
   cpu:read_u32(P1+0x28),cpu:read_u32(P1+0x2c),block,slotp,cpu:read_u32(0x10c2e8),cpu:read_u16(0x10000e),cpu:read_u16(0x1013de),cpu:read_u32(0x10c46a),cpu:read_u16(0x10c5fc)))
  local g={}
  for i=0,8 do g[#g+1]=string.format('%d',cpu:read_u8(0x10c326+i)) end
  trace:write(string.format('%d,%d,%d,%u,%u,%x,%x,%d,%s,%s\n',frame,camera,cpu:read_u16(0x10e99e),cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),
   cpu:read_u32(P1),cpu:read_u32(SCRIPT+0x6e),cpu:read_u16(SCRIPT+0x72),table.concat(g,' '),inputs))
  walk(frame,camera)
  -- Event snapshots: weapon id, ammo zero crossing, bombs, bomb type, vehicle slot word/kind/ammo.
  local key=string.format('%02x%02x%02x%04x%02x%02x',cpu:read_u8(0x10c71e),cpu:read_u8(0x10c720),cpu:read_u8(0x10c722),cpu:read_u16(0x10c608),cpu:read_u8(0x10c608+0x21),cpu:read_u8(0x10c608+0x1f))
  local event=key~=last_key
  last_key=key
  if frame%300==0 or frame==2100 then check_bank(frame) end
  if (frame>=first_dump and frame%period==0) or (event and frame>=next_event_dump and frame>2200) then dump(frame);if event then next_event_dump=frame+4;print('EVENT '..frame..' '..key) end end
  if frame%600==0 then print('FRAME '..frame..' camera '..camera);wcsv:flush();actors:flush();trace:flush();if deaths then deaths:flush() end end
 end
 marco_dump(frame)
 family_frame(frame)
 in_lua=false
 if frame==last_frame then
  if boss_log then boss_log:close() end
  wcsv:close();actors:close();states:close();trace:close();if deaths then deaths:close() end;if marco_states then marco_states:close() end;manager.machine:exit()
 end
end,'frame')
