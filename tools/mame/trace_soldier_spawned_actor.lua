-- Follow only the child actually allocated by the observed 03BB8E callback.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_action.lua')
local dbg=assert(manager.machine.debugger)
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
dbg:command('temp6=0;temp7=0')
dbg:command('bpset 3bba4,a6==101f14 && temp1==1,{temp6=a0;temp7=1;printf "CHILD_ALLOC frame=%d parent=%08X child=%08X routine=%08X\\n",temp9,a6,a0,d@(a0);g}')
local fmt=' frame=%d actor=%08X routine=%08X flags=%04X anim=%08X timer=%04X speed=%04X x=%08X y=%08X vx=%08X vy=%08X parent=%08X px=%08X py=%08X f50=%08X f5f=%02X f60=%08X f64=%08X f68=%08X f6c=%08X f70=%08X f74=%08X f78=%08X sr=%04X'
local args='temp9,a6,d@(a6),w@(a6+14),d@(a6+20),w@(a6+24),w@(a6+26),d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+38),d@(a6+3c),d@(a6+40),d@(a6+50),b@(a6+5f),d@(a6+60),d@(a6+64),d@(a6+68),d@(a6+6c),d@(a6+70),d@(a6+74),d@(a6+78),sr'
local function probe(site,label,action,condition)
 dbg:command(string.format('bpset %x,a6==temp6 && temp7==1%s,{%sprintf "%s site=%06X%s\\n",%s;g}',site,condition or '',action or '',label,site,fmt,args))
end
for frame=2557,2699 do
 probe(0x4248,'CHILD_IN','trace '..output..'/child-'..frame..'.log,maincpu,noloop,{tracelog "frame=%d A6=%08X D0=%08X D1=%08X D2=%08X D3=%08X D6=%08X D7=%08X SR=%04X ",temp9,a6,d0,d1,d2,d3,d6,d7,sr};',string.format(' && temp9==%x',frame))
end
probe(0x424a,'CHILD_OUT','trace off,maincpu;')
probe(0x4126,'CHILD_DELETE','trace off,maincpu;temp7=2;')
probe(0x4256,'CHILD_ANIM_START')
probe(0x427a,'CHILD_ANIM_IN')
probe(0x4350,'CHILD_ANIM_OUT')
probe(0x143e4,'CHILD_AIR_IN')
probe(0x1456a,'CHILD_GROUND_IN')
