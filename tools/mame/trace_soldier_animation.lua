-- Entry/return and draw probes for the original animation helper, first soldier.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_first_soldier.lua')
local dbg=assert(manager.machine.debugger)
local state_fmt=' frame=%d ptr=%08X timer=%04X speed=%04X f17=%02X f18=%02X field50=%08X'
local state_args='temp9,d@(a6+20),w@(a6+24),w@(a6+26),b@(a6+17),b@(a6+18),d@(a6+50)'
local function probe(site,label,extra_fmt,extra_args)
 local fmt=label..' site='..string.format('%06X',site)..state_fmt..(extra_fmt or '')..'\\n'
 local args=state_args..(extra_args or '')
 -- The constructor watchpoint sets temp1. A6 alone is insufficient: this slot
 -- is also reused during the attract sequence before the mission soldier.
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1,{printf "%s",%s;g}',site,fmt,args))
end
probe(0x4256,'ANIM_INIT_IN',' program=%08X requested_speed=%04X',',a1,d0&ffff')
probe(0x4278,'ANIM_INIT_OUT')
probe(0x427a,'ANIM_IN',' base=%08X f16=%04X palettes=%08X f5f=%02X phase=%02X x=%04X y=%04X correction=%02X z=%04X',
 ',d@(a6+12),w@(a6+16),d@(a6+19),b@(a6+5f),b@(10000f),w@(a6+3c),w@(a6+40),b@(a6+4d),w@(a6+1e)')
probe(0x4350,'ANIM_OUT',' sr=%04X',',sr')
probe(0x4288,'ANIM_NULL_OUT',' sr=%04X',',sr')
probe(0x438e,'ANIM_RAW',' descriptor=%08X flags=%02X dx=%02X dy=%02X',',d7,d5&ff,d0&ff,d6&ff')
probe(0x4406,'ANIM_DRAW',' descriptor=%08X xy=%08X attr=%04X zreg=%04X',',d7,d0,d1&ffff,d5&ffff')
probe(0x44ec,'ANIM_CALL_IN',' target=%08X attr=%04X',',a1,w@(a6+14)')
probe(0x44ee,'ANIM_CALL_OUT',' attr=%04X',',w@(a6+14)')
