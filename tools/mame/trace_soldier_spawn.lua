-- Locate the level-data record that actually created the identified soldier.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_first_soldier.lua')
local dbg=assert(manager.machine.debugger)
local function probe(address,label,args,fmt)
 dbg:command(string.format('bpset %x,a0==101f14 && temp1==1,{printf "%s frame=%%d a6=%%08X a0=%%08X script=%%08X%s\\n",temp9,a6,a0,d@(sp)%s;g}',address,label,fmt or '',args or ''))
end
probe(0x1a134,'SPAWN_RECORD',',w@(d@(sp)),w@(d@(sp)+2),w@(d@(sp)+4),w@(d@(sp)+6),w@(d@(sp)+8),w@(d@(sp)+a),w@(d@(sp)+c),w@(d@(sp)+e),w@(d@(sp)+10),w@(d@(sp)+12),w@(d@(sp)+14)',
 ' words=%04X%04X%04X%04X%04X%04X%04X%04X%04X%04X%04X')
probe(0x1a152,'SPAWN_COPY_OUT',',w@(a0+6e),w@(a0+70),w@(a0+72),w@(a0+74),w@(a0+76),w@(a0+78)',
 ' f6e=%04X f70=%04X f72=%04X f74=%04X f76=%04X f78=%04X')
