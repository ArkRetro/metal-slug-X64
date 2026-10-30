-- MAME 0.264, mslugx euro BIOS. Mission 1 with the soft-DIP "BLOOD" ON.
-- Set MSX_CAPTURE_DIR; run with tools/run_capture_blood.sh.
--
-- The arcade keeps soft-DIP option 6 (BLOOD) in RAM 10FD8F; the euro and US
-- headers default it to OFF (0), the Japanese header to ON. This wrapper
-- writes 10FD8F = 1 every frame and, through a read tap, also returns 1 to
-- every read of that byte, then plays the whole mission with
-- tools/capture_mission_rest.lua (same input plan, same reference-only aids;
-- nothing else is written). It is a reference capture, never shipped.
--
-- Extra output (MSX_CAPTURE_DIR):
--  blood.csv   every read of 10FD8F: frame, PC of the reading routine
--              (01B2BE or 032B94), the caller (return address - 6 for a
--              jsr 01B2BE), the value returned, the actor (A6) and its routine
--  debug.log   MSX_BLOOD_PALREQ=1 (-debug, see run_capture_blood.sh):
--              PAL frame (hex) caller d0 a6 routine at every 009EA8 entry, the
--              logical palette resource each call site requests
--
-- The BIOS RAM test of the first seconds reads and writes 10FD8E, so the
-- byte is only forced from frame 600 on (the game starts at 700) and the tap
-- only changes reads made by game code (PC below 0x100000).
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local here=debug.getinfo(1,'S').source:match('^@(.*/)') or './'
local cpu=manager.machine.devices[':maincpu']
local space=cpu.spaces['program']
local frame=0
local function reg(name) local s=cpu.state[name];return s and s.value or 0 end
local log=assert(io.open(output..'/blood.csv','w'))
log:write('frame,pc,caller,value,actor,routine\n')
blood_tap=space:install_read_tap(0x10fd8e,0x10fd8f,'msx_blood',function(offset,data,mask)
 if (mask&0x00ff)==0 or frame<600 or reg('rPC')>=0x100000 then return data end
 local ret=space:read_u32(reg('SP'))
 local a6=reg('A6')
 log:write(string.format('%d,%06x,%06x,%d,%06x,%06x\n',frame,reg('rPC'),(ret-6)&0xffffff,1,a6,space:read_u32(a6)&0xffffff))
 return (data&0xff00)|1
end)
local dbg=os.getenv('MSX_BLOOD_PALREQ')=='1' and manager.machine.debugger or nil
if dbg then
 dbg:command('bpset 9ea8,1,{printf "PAL frame=%X caller=%08X d0=%04X a6=%08X routine=%08X\\n",temp8,d@(sp)-6,d0&ffff,a6,d@(a6);g}')
end
emu.register_frame_done(function()
 frame=frame+1
 if frame>=600 then space:write_u8(0x10fd8f,1) end
 if dbg then dbg:command(string.format('temp8=%x',frame)) end
 if frame%600==0 then log:flush() end
end,'frame')
dofile(here..'capture_mission_rest.lua')
