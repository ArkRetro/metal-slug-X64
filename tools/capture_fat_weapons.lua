-- MAME 0.264, mslugx euro BIOS. Fat Marco (+93 = 2) with every weapon
-- (docs/MISSION3_DEBUG.md G21): from a Mission 3 state where the player is
-- fat, each weapon id of MSX_FAT_IDS (hex list) is given in turn and fired
-- by a fixed input pattern: level, rapid presses, up, crouched, down and
-- level in the air, walking, facing left, a grenade and the knife.
-- Run with tools/run_capture_fat_weapons.sh (debugger on: sound commands).
--
-- Capture-only RAM writes (never in the N64 game): the weapon grant fields
-- as 0C0928 writes them (10C71E id, 10C71C cap, 10C71A = 0, +94, +95 = 0),
-- the rounds 10C724 (FFFF pistol, else 200), bombs 10C720 = 10 with type
-- 10C722 = MSX_FAT_BOMB (1), the fat counter 10C728 = 0x400 (0C33AE never
-- reaches 0), the timer 1013DE and the invulnerability bit 101523 |= 0x40
-- (as capture_fullgame.lua).
--
-- Outputs (MSX_CAPTURE_DIR):
--  fat.txt   P <frame> <inputs> <player 1014C4 bytes +00..+AF>   every frame
--            N <frame> <node> <old routine> <new routine> <bytes +00..+AF>
--                         every node whose routine changed in that frame
--            W <frame> <input words 10E9EC..10E9F1> <weapon block 10C718..10C727>
--                         every frame
--            G <frame> <weapon id>  at each grant
--            E <frame>  the end of the segments: the fat counter is set to
--                       0x20 and left alone (0C33AE: +94 = 0, 0C1D82 and
--                       0C5064), MSX_FAT_TAIL (300) frames without input
--  ram<f>.bin, palette<f>.bin every MSX_FAT_DUMP frames (0: none), for the
--            palettes of tools/export_rest_sprites.py --palette-capture
--  debug.log SND f=<frame> r=<routine of A6> w=<sound word> (bp 0120F8)
--            ALLOC f=<frame> n=<node> r=<routine> c=<creator> ret=<caller>
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local load_name=assert(os.getenv('MSX_FAT_LOAD'),'Set MSX_FAT_LOAD')
local load_frame=tonumber(os.getenv('MSX_FAT_LOAD_FRAME') or '0')
local seg=tonumber(os.getenv('MSX_FAT_SEGMENT') or '480')
local bomb=tonumber(os.getenv('MSX_FAT_BOMB') or '1')
local ids={}
for h in string.gmatch(os.getenv('MSX_FAT_IDS') or '0,a,3,4,2,6,c,d,e','(%x+)') do ids[#ids+1]=tonumber(h,16) end
local machine=manager.machine
local cpu=machine.devices[':maincpu'].spaces['program']
local dbg=machine.debugger
local P,POOL0,NODES=0x1014c4,0x1010a4,100
local out=assert(io.open(output..'/fat.txt','w'))
local pressed={}
local function button(name,value)
 pressed[name]=value
 for tag,p in pairs(machine.ioport.ports) do
  if string.find(tag,'JOY1') then for key,f in pairs(p.fields) do
   if key==name or key=='%p '..string.sub(name,4) then f:set_value(value) end
  end end
 end
end
local function hexbytes(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
local known={}
-- The input pattern of one segment, t = frames since the grant.
local function pad(t)
 local b={L=0,R=0,U=0,D=0,A=0,B=0,C=0}
 local function press(at,len) return t>=at and t<at+(len or 3) end
 if press(10) or press(30) then b.A=1 end
 if t>=50 and t<80 and (t//2)%2==0 then b.A=1 end            -- rapid presses
 if t>=90 and t<150 then b.U=1;if press(100) or press(120) or press(140) then b.A=1 end end
 if t>=160 and t<220 then b.D=1;if press(170) or press(190) or press(210) then b.A=1 end end
 if press(230,4) then b.B=1 end                                 -- jump: down + fire
 if t>=234 and t<270 then b.D=1;if press(240) or press(252) then b.A=1 end end
 if press(290,4) then b.B=1 end                                 -- jump: level fire
 if press(300) or press(312) then b.A=1 end
 if t>=340 and t<370 then b.R=1;if press(345) or press(360) then b.A=1 end end
 if t>=380 and t<384 then b.L=1 end                              -- face left
 if press(395) then b.A=1 end
 if t>=405 and t<425 then b.U=1;if press(410) then b.A=1 end end
 if press(435) then b.C=1 end                                    -- grenade
 if t>=455 and t<459 then b.R=1 end                              -- face right again
 return b
end
local dump_every=tonumber(os.getenv('MSX_FAT_DUMP') or '0')
local function dump(f)
 local ram=assert(io.open(output..'/ram'..f..'.bin','wb'));ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 local parts={}
 for i=0,4095 do parts[#parts+1]=string.pack('>I2',cpu:read_u16(0x400000+i*2)) end
 local p=assert(io.open(output..'/palette'..f..'.bin','wb'));p:write(table.concat(parts));p:close()
end
local tail=tonumber(os.getenv('MSX_FAT_TAIL') or '300')
-- MSX_FAT_THIN=1: the fat counter 10C728 is never written (a state with a
-- thin player: the same pattern for the thin fire programs).
local thin=os.getenv('MSX_FAT_THIN')=='1'
local frame=0;local segment=0;local seg_start=nil;local ended=nil
emu.register_frame_done(function()
 frame=frame+1
 if frame==1 then
  machine:load(load_name);frame=load_frame
  dbg:command('bpset 120f8,1,{printf "SND f=%d r=%08X w=%04X ret=%08X\\n",temp9,d@a6,d0&ffff,d@(sp);g}')
  dbg:command('bpset 410e,1,{printf "ALLOC f=%d n=%08X r=%08X c=%08X ret=%08X\\n",temp9,a0,d@(sp),a6,d@(sp+4);g}')
  for c in string.gmatch(os.getenv('MSX_FAT_DEBUG_CMDS') or '','[^|]+') do dbg:command(c) end
  seg_start=frame+20
  return
 end
 dbg:command(string.format('temp9=%x',frame))
 cpu:write_u8(0x1013de,0x5a);cpu:write_u8(0x101523,cpu:read_u8(0x101523)|0x40)
 cpu:write_u8(0x10c720,10);cpu:write_u8(0x10c722,bomb)
 local t=frame-seg_start
 if t>=seg then segment=segment+1;seg_start=frame;t=0 end
 if segment>=#ids and not ended then ended=frame;if not thin then cpu:write_u16(0x10c728,0x20) end;out:write(string.format('E %d\n',frame)) end
 if ended and frame>=ended+tail then out:close();machine:exit();return end
 if not ended and not thin then cpu:write_u16(0x10c728,0x400) end
 local id=ids[math.min(segment+1,#ids)]
 if t==0 and not ended then
  cpu:write_u8(0x10c71e,id);cpu:write_u8(0x10c71c,cpu:read_u8(0xc08a8+id));cpu:write_u8(0x10c71a,0)
  -- 0C0928: +94 = 0C0918[id]; a 0 there (ids 0/1) would end the fat form
  -- (rule 0 of 0C32E4), so the pistol keeps +94.
  cpu:write_u8(P+0x95,0);if cpu:read_u8(0xc0918+id)~=0 then cpu:write_u8(P+0x94,cpu:read_u8(0xc0918+id)) end
  out:write(string.format('G %d %x\n',frame,id))
 end
 if not ended then cpu:write_u16(0x10c724,id==0 and 0xffff or 200) end
 local b=t>=0 and not ended and pad(t) or {L=0,R=0,U=0,D=0,A=0,B=0,C=0}
 button('P1 Left',b.L);button('P1 Right',b.R);button('P1 Up',b.U);button('P1 Down',b.D)
 button('P1 A',b.A);button('P1 B',b.B);button('P1 C',b.C)
 local keys=(b.L==1 and 'L' or '')..(b.R==1 and 'R' or '')..(b.U==1 and 'U' or '')..(b.D==1 and 'D' or '')..(b.A==1 and 'A' or '')..(b.B==1 and 'B' or '')..(b.C==1 and 'C' or '')
 out:write(string.format('P %d %s %s\n',frame,keys=='' and '-' or keys,hexbytes(P,0xb0)))
 out:write(string.format('W %d %s %s\n',frame,hexbytes(0x10e9ec,6),hexbytes(0x10c718,16)))
 if dump_every>0 and frame%dump_every==0 then dump(frame) end
 for i=0,NODES-1 do
  local n=POOL0+i*0xb0
  if n~=P then
   local r=cpu:read_u32(n)
   if known[n]~=nil and r~=known[n] then out:write(string.format('N %d %x %x %x %s\n',frame,n,known[n],r,hexbytes(n,0xb0))) end
   known[n]=r
  end
 end
end)
