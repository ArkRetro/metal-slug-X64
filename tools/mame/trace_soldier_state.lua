-- Trace only dispatches of the constructor-identified first mission soldier.
-- Addresses come from the existing dispatcher/state disassembly; no RAM patch.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_first_soldier.lua')
local dbg=assert(manager.machine.debugger)
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local fmt=' frame=%d routine=%08X f15=%02X f5f=%02X f62=%08X f26=%04X f6c=%04X f70=%04X f74=%04X f78=%04X f7c=%02X f7e=%02X f80=%02X f81=%02X f84=%08X f88=%08X f8c=%04X f92=%04X x=%08X y=%08X px=%08X py=%08X d0=%08X d1=%08X d6=%08X sr=%04X'
local args='temp9,d@(a6),b@(a6+15),b@(a6+5f),d@(a6+62),w@(a6+26),w@(a6+6c),w@(a6+70),w@(a6+74),w@(a6+78),b@(a6+7c),b@(a6+7e),b@(a6+80),b@(a6+81),d@(a6+84),d@(a6+88),w@(a6+8c),w@(a6+92),d@(a6+28),d@(a6+2c),d@(a6+3c),d@(a6+40),d0,d1,d6,sr'
local function probe(site,label,action,condition)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1%s,{%sprintf "%s site=%06X%s\\n",%s;g}',site,condition or '',action or '',label,site,fmt,args))
end
for frame=2351,tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '2460')-1,2 do
 probe(0x4248,'DISPATCH_IN','trace '..output..'/actor-'..frame..'.log,maincpu,noloop,{tracelog "frame=%d A6=%08X D0=%08X D1=%08X D2=%08X D3=%08X D6=%08X SR=%04X ",temp9,a6,d0,d1,d2,d3,d6,sr};',string.format(' && temp9==%x',frame))
end
probe(0x424a,'DISPATCH_OUT','trace off,maincpu;')
-- 004126 is the executed deletion entry, reached through 004188. Stop the
-- identity gate before the allocator can reuse the same address.
probe(0x4126,'LIFECYCLE_DELETE','trace off,maincpu;temp1=2;')
for _,site in ipairs({0xaf6d0,0xaf6f0,0xaf72a,0xaf738,0xaf746,0xaf768,0xaf776,0xaf79a,0xaf79e,0xaf7ac,0xaf8a6,0xaf8c0,0xaf8c6,0xaf8d0,0xaf3ae,0xaf3b4,0xaf41c,0xaf42a,0xaf438,0xaf446,0xaf460,0xaf470,0xaf474,0xaf478,0xaf482,0xaf490,0xb035a,0xb0360,0xb036a,0xb037a,0xb051e,0xb0538,0xb05d8}) do probe(site,'STATE_BOUNDARY') end
probe(0x4256,'STATE_ANIM_START')
probe(0x427a,'STATE_ANIM_IN')
probe(0x4350,'STATE_ANIM_OUT')
probe(0x40200,'STATE_OBJECT_QUERY_IN')
probe(0xaf10e,'STATE_OBJECT_QUERY_OUT')
probe(0x49e8,'STATE_REGION_QUERY_IN')
probe(0xaedc2,'STATE_REGION_QUERY_OUT')
