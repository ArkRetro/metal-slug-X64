-- Observe the history-token writer; no gameplay memory patches.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_first_soldier.lua')
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local dbg=assert(manager.machine.debugger)
dbg:command('temp3=0')
local fmt='printf "HISTORY_TOKEN_WRITE frame=%d pc=%08X address=%08X data=%08X before=%02X\\n",temp9,pc,wpaddr,wpdata,b@(10c33a);'
dbg:command('wpset 10c33a,1,w,temp9>=834 && temp3==0,{'..fmt..'temp3=1;trace '..output..'/history-token.log,maincpu,noloop,{tracelog "frame=%d D0=%08X A5=%08X ",temp9,d0,a5};g}')
dbg:command('wpset 10c33a,1,w,temp9>=834 && temp3==1,{'..fmt..'g}')
local frame=0
emu.register_frame_done(function()
 frame=frame+1
 if frame==2104 then dbg:command('trace off,maincpu') end
end,'frame')
