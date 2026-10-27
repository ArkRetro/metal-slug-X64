-- Complete drawing/control lifecycle of the actually allocated throw actor.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_attack.lua')
local dbg=assert(manager.machine.debugger)
local cpu_debug=manager.machine.devices[':maincpu'].debug
for index,bp in pairs(cpu_debug:bplist()) do
 if (bp.address==0x4256 or bp.address==0x427a or bp.address==0x4350) and
    bp.action:find('CHILD_ANIM_',1,true) then cpu_debug:bpclear(index) end
end
local sf=' frame=%d ptr=%08X timer=%04X speed=%04X f17=%02X f18=%02X field50=%08X'
local sa='temp9,d@(a6+20),w@(a6+24),w@(a6+26),b@(a6+17),b@(a6+18),d@(a6+50)'
local function anim(site,label,extra,args)
 dbg:command(string.format('bpset %x,a6==temp6 && temp7==1,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,sf,extra or '',sa,args or ''))
end
anim(0x4256,'OBJECT_INIT_IN',' program=%08X requested_speed=%04X',',a1,d0&ffff')
anim(0x4278,'OBJECT_INIT_OUT')
anim(0x427a,'OBJECT_IN',' base=%08X f16=%04X palettes=%08X f5f=%02X phase=%02X x=%04X y=%04X correction=%02X z=%04X routine=%08X oldx=%08X oldy=%08X mark=%02X epoch=%02X f66=%04X',
 ',d@(a6+12),w@(a6+16),d@(a6+19),b@(a6+5f),b@(10000f),w@(a6+3c),w@(a6+40),b@(a6+4d),w@(a6+1e),d@(a6),d@(a6+44),d@(a6+48),b@(a6+4c),b@(10c33a),w@(a6+66)')
anim(0x4350,'OBJECT_OUT',' sr=%04X',',sr')
anim(0x438e,'OBJECT_RAW',' descriptor=%08X flags=%02X dx=%02X dy=%02X',',d7,d5&ff,d0&ff,d6&ff')
anim(0x4406,'OBJECT_DRAW',' descriptor=%08X xy=%08X attr=%04X zreg=%04X',',d7,d0,d1&ffff,d5&ffff')
anim(0x3bd98,'OBJECT_CYCLE_IN',' phase16=%04X palettes=%08X',',w@(10000e),d@(a6+19)')
anim(0x3bdb4,'OBJECT_CYCLE_OUT',' attr=%04X',',w@(a6+14)')
anim(0x3bd7c,'OBJECT_HIT_OUT',' result=%04X',',d0&ffff')
anim(0x3bd8e,'OBJECT_OTHER_OUT',' sr=%04X',',sr')
anim(0x3be6c,'OBJECT_BURST_IN',' f66=%04X f1e=%04X',',w@(a6+66),w@(a6+1e)')
anim(0x3bdbe,'OBJECT_CULL_OUT',' sr=%04X',',sr')
anim(0x1ca00,'OBJECT_CULL_OUT',' sr=%04X',',sr')
local bf=' frame=%d left=%04X top=%04X right=%04X bottom=%04X parent=%08X parentx=%08X parenty=%08X parentoldx=%08X parentoldy=%08X rect0=%04X rect1=%04X rect2=%04X rect3=%04X'
local ba='temp9,w@(d@(10c2f0)+4),w@(d@(10c2f0)+6),w@(d@(10c2f0)+8),w@(d@(10c2f0)+a),d@(d@(10c2f0)),d@(d@(d@(10c2f0))+14),d@(d@(d@(10c2f0))+18),d@(d@(d@(10c2f0))+1c),d@(d@(d@(10c2f0))+20),w@(a0),w@(a0+2),w@(a0+4),w@(a0+6)'
dbg:command('bpset 19504,a6==temp6 && temp7==1,{printf "OBJECT_CULL_IN'..bf..'\\n",'..ba..';g}')
