-- Motion wrappers, kernel boundary and projection probes after the constructor.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_first_soldier.lua')
local dbg=assert(manager.machine.debugger)
local fmt=' frame=%d x=%08X y=%08X vx=%08X vy=%08X parent=%08X f15=%02X f5f=%02X f5a=%02X f6c=%04X f62=%08X f66=%04X f1e=%04X px=%08X py=%08X oldx=%08X oldy=%08X mark=%02X correction=%02X epoch=%02X'
local args='temp9,d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+38),b@(a6+15),b@(a6+5f),b@(a6+5a),w@(a6+6c),d@(a6+62),w@(a6+66),w@(a6+1e),d@(a6+3c),d@(a6+40),d@(a6+44),d@(a6+48),b@(a6+4c),b@(a6+4d),b@(10c33a)'
local function probe(site,label,extra_fmt,extra_args)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,fmt,extra_fmt or '',args,extra_args or ''))
end
probe(0x1456a,'GROUND_IN')
probe(0x143e4,'AIR_IN')
local regs=' d0=%08X d1=%08X d2=%08X d3=%08X d4=%08X d6=%08X d7=%08X a0=%08X'
local regargs=',d0,d1,d2,d3,d4,d6,d7,a0'
local parent=' p14=%08X p18=%08X p1c=%08X p20=%08X p25=%02X'
local parentargs=',d@(a0+14),d@(a0+18),d@(a0+1c),d@(a0+20),b@(a0+25)'
probe(0x13968,'KERNEL_IN',regs..parent..' mode=%08X mode8=%02X mode9=%02X modea=%02X moded=%02X',regargs..parentargs..',d@(10c004),b@(10c008),b@(10c009),b@(10c00a),b@(10c00d)')
probe(0x14492,'KERNEL_OUT',regs..parent,regargs..parentargs)
probe(0x14520,'COMMIT_OUT',regs,regargs)
probe(0x146a6,'GROUND_OUT',regs,regargs)
probe(0x2e98,'VECTOR_IN',regs,regargs)
probe(0x2eb2,'VECTOR_OUT',regs,regargs)
