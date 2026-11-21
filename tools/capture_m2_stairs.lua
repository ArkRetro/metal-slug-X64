-- MAME 0.264, mslugx euro BIOS. Mission 2 stage 2: jumps on the 45-degree
-- staircase (cells of shape 5, x 976..1031, y 168..224) while the camera
-- rises (docs/MISION2.md, "Fase 2i"). Resumes from a state saved by
-- tools/capture_fullgame.lua (MSX_FULL_SAVE_AT) and plays a scripted input
-- plan; reads RAM only, apart from the same reference-only overrides as
-- capture_fullgame.lua (survival, ammo).
--
-- MSX_CAPTURE_DIR   output directory (stairs.csv)
-- MSX_STAIRS_LOAD   state name (e.g. at_22370), MSX_STAIRS_FRAME its frame
-- MSX_STAIRS_PLAN   "frame:buttons,frame:buttons,..." (buttons from LRUDABC,
--                   held from that frame on; '-' = none)
-- MSX_STAIRS_LAST   last frame
--
-- stairs.csv: every frame, the player node 1014C4: routine, +28/+2C (Q16
-- world position, Y up), +30/+34 (velocity), +15 flags, +40 projected y,
-- the scene root 10E97C/10E980 and the terrain layer 10C72E +14/+18 (+1C/+20
-- previous), the parent +38, the kernel's screen box RAM 10C01C..10C023,
-- RAM 10C00C and the shake 10E9CC (0138A0).
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local load_name=assert(os.getenv('MSX_STAIRS_LOAD'))
local first=tonumber(assert(os.getenv('MSX_STAIRS_FRAME')))
local last=tonumber(os.getenv('MSX_STAIRS_LAST') or tostring(first+600))
local plan={}
for f,b in string.gmatch(os.getenv('MSX_STAIRS_PLAN') or '','(%d+):([%-LRUDABC]+)') do plan[#plan+1]={tonumber(f),b} end
table.sort(plan,function(a,b) return a[1]<b[1] end)
local machine=manager.machine
local cpu=machine.devices[':maincpu'].spaces['program']
local out=assert(io.open(output..'/stairs.csv','w'))
out:write('frame,inputs,routine,x,y,vx,vy,flags15,py40,root_x,root_y,terrain_x,terrain_y,flags5f,parent38,box,b400c,shake,prev_terrain_x,prev_terrain_y,prev_root_x,prev_root_y\n')
local names={L='P1 Left',R='P1 Right',U='P1 Up',D='P1 Down',A='P1 A',B='P1 B',C='P1 C'}
local function button(name,value)
 for tag,p in pairs(machine.ioport.ports) do
  if string.find(tag,'JOY1') then for key,f in pairs(p.fields) do
   if key==name or key=='%p '..string.sub(name,4) then f:set_value(value) end
  end end
 end
end
-- MSX_STAIRS_DEBUG=1 (MAME -debug -debugger none -debuglog): the player's
-- air sweep 0143EE -> 013968 (A6 = 1014C4): entry registers at 01448C and
-- RAM 10C004/10C008..10C00D, exit at 014492 (D0..D3, A0, D4, D6, D7), and
-- the ground sweep 014594's call at the same exit address range.
local dbg=os.getenv('MSX_STAIRS_DEBUG')=='1' and machine.debugger or nil
if dbg then
 dbg:command('bpset 1448c,a6==1014c4,{printf "IN f=%d d0=%08X d1=%08X d2=%08X d3=%08X a0=%08X m4004=%08X b4008=%02X b4009=%02X b400a=%02X b400b=%02X b400c=%02X b400d=%02X w4068=%04X w4018=%04X\\n",temp9,d0,d1,d2,d3,a0,d@10c004,b@10c008,b@10c009,b@10c00a,b@10c00b,b@10c00c,b@10c00d,w@10c068,w@10c018;g}')
 dbg:command('bpset 14492,a6==1014c4,{printf "OUT f=%d d0=%08X d1=%08X d2=%08X d3=%08X a0=%08X d4=%08X d6=%08X d7=%08X\\n",temp9,d0,d1,d2,d3,a0,d4,d6,d7;g}')
end
local frame=0
emu.register_frame_done(function()
 frame=frame+1
 if frame==1 then machine:load(load_name);frame=first;return end
 if frame<first then return end
 if dbg then dbg:command(string.format('temp9=%x',frame)) end
 -- Reference-only overrides of capture_fullgame.lua (frame >= 2190).
 cpu:write_u8(0x1013de,0x5a)
 cpu:write_u8(0x101523,cpu:read_u8(0x101523)|0x40)
 cpu:write_u8(0x101549,0x99)
 cpu:write_u8(0x101288,2)
 cpu:write_u8(0x10c725,0xff);cpu:write_u8(0x10c720,0x63)
 cpu:write_u8(0x10c621,0x30);cpu:write_u8(0x10c627,0x63)
 local held='-'
 for _,p in ipairs(plan) do if p[1]<=frame then held=p[2] end end
 for k,n in pairs(names) do button(n,string.find(held,k,1,true) and 1 or 0) end
 local m=0x1014c4
 out:write(string.format('%d,%s,%x,%d,%d,%d,%d,%x,%d,%d,%d,%d,%d,%x\n',frame,held,cpu:read_u32(m),
  cpu:read_i32(m+0x28),cpu:read_i32(m+0x2c),cpu:read_i32(m+0x30),cpu:read_i32(m+0x34),cpu:read_u8(m+0x15),cpu:read_i32(m+0x40),
  cpu:read_i32(0x10e97c),cpu:read_i32(0x10e980),cpu:read_i32(0x10c72e+0x14),cpu:read_i32(0x10c72e+0x18),cpu:read_u8(m+0x5f)):sub(1,-2)..string.format(',%x,%d %d %d %d,%x,%d,%d,%d,%d,%d\n',
  cpu:read_u32(m+0x38),cpu:read_i16(0x10c01c),cpu:read_i16(0x10c01e),cpu:read_i16(0x10c020),cpu:read_i16(0x10c022),cpu:read_u8(0x10c00c),cpu:read_i32(0x10e9cc),
  cpu:read_i32(0x10c72e+0x1c),cpu:read_i32(0x10c72e+0x20),cpu:read_i32(0x10e968+0x1c),cpu:read_i32(0x10e968+0x20)))
 if frame>=last then out:close();machine:exit() end
end)
