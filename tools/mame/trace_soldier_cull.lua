-- Pure culling helper: exact scene bounds/history and returned carry.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_state.lua')
local dbg=assert(manager.machine.debugger)
local fmt=' frame=%d px=%08X py=%08X oldx=%08X oldy=%08X f15=%02X bounds=%08X parent=%08X left=%04X top=%04X right=%04X bottom=%04X parentx=%08X parenty=%08X parentoldx=%08X parentoldy=%08X rect0=%04X rect1=%04X rect2=%04X rect3=%04X'
local args='temp9,d@(a6+3c),d@(a6+40),d@(a6+44),d@(a6+48),b@(a6+15),d@(10c2f0),d@(d@(10c2f0)),w@(d@(10c2f0)+4),w@(d@(10c2f0)+6),w@(d@(10c2f0)+8),w@(d@(10c2f0)+a),d@(d@(d@(10c2f0))+14),d@(d@(d@(10c2f0))+18),d@(d@(d@(10c2f0))+1c),d@(d@(d@(10c2f0))+20),w@(a0),w@(a0+2),w@(a0+4),w@(a0+6)'
dbg:command('bpset 1945c,a6==101f14 && temp1==1,{printf "CULL_IN'..fmt..'\\n",'..args..';g}')
dbg:command('bpset b0546,a6==101f14 && temp1==1,{printf "CULL_OUT frame=%d sr=%04X\\n",temp9,sr;g}')
