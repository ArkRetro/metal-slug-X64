-- Instruction-trace window on top of capture_soldier_campaign.lua inputs.
-- Env: MSX_TRACE_FIRST / MSX_TRACE_LAST (video frames), plus the campaign
-- capture variables. Requires -debug -debugger none. No RAM writes.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'capture_soldier_campaign.lua')
local output=os.getenv('MSX_CAPTURE_DIR')
local first=tonumber(os.getenv('MSX_TRACE_FIRST') or '0')
local last=tonumber(os.getenv('MSX_TRACE_LAST') or '0')
local dbg=assert(manager.machine.debugger,'Launch MAME with -debug -debugger none')
local frame=0
emu.register_frame_done(function()
 frame=frame+1
 dbg:command(string.format('temp9=%x',frame))
 if frame==first then
  dbg:command('trace '..output..'/instructions.log,maincpu,noloop,{tracelog "F=%d A6=%06X A0=%08X A1=%08X D0=%08X D1=%08X D7=%08X SR=%04X ",temp9,a6,a0,a1,d0,d1,d7,sr}')
 end
 if frame==last then dbg:command('trace off,maincpu') end
end,'frame')
