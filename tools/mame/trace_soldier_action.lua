-- Read-only boundaries of the original weighted action selector.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_state.lua')
local dbg=assert(manager.machine.debugger)
local pool={};for a=0x100016,0x100082,2 do pool[#pool+1]=string.format('w@(%x)',a) end
local fmt=' frame=%d f15=%02X f7c=%02X f80=%02X f81=%02X f70=%04X f92=%04X f8e=%04X f90=%04X px=%08X py=%08X active=%02X p1x=%04X p1y=%04X p2x=%04X p2y=%04X originx=%04X originy=%04X result=%04X cursor=%08X pool='..string.rep('%04X',55)
local args='temp9,b@(a6+15),b@(a6+7c),b@(a6+80),b@(a6+81),w@(a6+70),w@(a6+92),w@(a6+8e),w@(a6+90),d@(a6+3c),d@(a6+40),b@(10c33c),w@(101500),w@(101504),w@(1015b0),w@(1015b4),w@(10e97c),w@(10e980),d0&ffff,d@(100084),'..table.concat(pool,',')
for _,p in ipairs({{0xaee0c,'ACTION_IN'},{0xaef10,'ACTION_OUT'},{0xaef12,'ACTION_OUT'}}) do
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1,{printf "%s site=%06X%s\\n",%s;g}',p[1],p[2],p[1],fmt,args))
end
