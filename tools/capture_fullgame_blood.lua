-- MAME 0.264: tools/capture_fullgame.lua with the soft-DIP "BLOOD" ON (RAM
-- 10FD8F = 1 written every frame and returned to every game read, as
-- tools/capture_blood.lua does for Mission 1). Run with
-- MSX_FULL_SCRIPT=capture_fullgame_blood.lua tools/run_capture_fullgame.sh;
-- a reference capture for the BLOOD ON palettes (Mission 3: 0A7 of the
-- pilot 04EF88, MISSION3_DEBUG.md G12), never shipped.
local here=debug.getinfo(1,'S').source:match('^@(.*/)') or './'
local cpu=manager.machine.devices[':maincpu']
local space=cpu.spaces['program']
blood_tap=space:install_read_tap(0x10fd8e,0x10fd8f,'msx_blood',function(offset,data,mask)
 if (mask&0x00ff)==0 or cpu.state['rPC'].value>=0x100000 then return data end
 return (data&0xff00)|1
end)
emu.register_frame_done(function() space:write_u8(0x10fd8f,1) end,'blood')
dofile(here..'capture_fullgame.lua')
