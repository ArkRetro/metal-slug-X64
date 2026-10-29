-- Reposition-state boundaries, native data inputs and inline pose-table choices.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_state.lua')
local dbg=assert(manager.machine.debugger)
local cpu_debug=manager.machine.devices[':maincpu'].debug
for index,bp in pairs(cpu_debug:bplist()) do
 if ((bp.address==0x4256 or bp.address==0x427a or bp.address==0x4350) and bp.action:find('STATE_ANIM_',1,true)) or
    (bp.address==0xb051e and bp.action:find('STATE_BOUNDARY',1,true)) then cpu_debug:bpclear(index) end
end
local words={};for a=0x100016,0x100082,2 do words[#words+1]=string.format('w@(%x)',a) end
local grid={};for a=0x1067e6,0x1068e2,4 do grid[#grid+1]=string.format('d@(%x)',a) end
local sf=' frame=%d routine=%08X f61=%02X f62=%08X speed=%04X f78=%04X f7e=%02X f81=%02X f15=%02X f5f=%02X f68=%02X f6e=%04X f70=%04X f74=%04X f7c=%02X f80=%02X f84=%08X f88=%08X f8c=%04X f8e=%04X f90=%04X f92=%04X px=%08X py=%08X active=%02X p1x=%04X p1y=%04X p2x=%04X p2y=%04X originx=%04X originy=%04X cursor=%08X pool='..string.rep('%04X',55)
local sa='temp9,d@(a6),b@(a6+61),d@(a6+62),w@(a6+26),w@(a6+78),b@(a6+7e),b@(a6+81),b@(a6+15),b@(a6+5f),b@(a6+68),w@(a6+6e),w@(a6+70),w@(a6+74),b@(a6+7c),b@(a6+80),d@(a6+84),d@(a6+88),w@(a6+8c),w@(a6+8e),w@(a6+90),w@(a6+92),d@(a6+3c),d@(a6+40),b@(10c33c),w@(101500),w@(101504),w@(1015b0),w@(1015b4),w@(10e97c),w@(10e980),d@(100084),'..table.concat(words,',')
local function state(site,label)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1,{printf "%s site=%06X%s\\n",%s;printf "LOCO_GRID site=%06X frame=%%d grid=%s\\n",temp9,%s;g}',site,label,site,sf,sa,site,string.rep('%08X',64),table.concat(grid,',')))
end
state(0xaf4f6,'LOCO_SEED_IN');state(0xaf572,'LOCO_BODY_IN');state(0xb051e,'LOCO_MAIN_OUT');state(0xaf5b8,'LOCO_POSE_READY')
local af=' frame=%d ptr=%08X timer=%04X speed=%04X f17=%02X f18=%02X field50=%08X'
local aa='temp9,d@(a6+20),w@(a6+24),w@(a6+26),b@(a6+17),b@(a6+18),d@(a6+50)'
local function anim(site,label,extra,args)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1 && (d@(a6)==af4f6 || d@(a6)==af572),{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,af,extra or '',aa,args or ''))
end
anim(0x4256,'LOCO_INIT_IN',' program=%08X requested_speed=%04X',',a1,d0&ffff')
anim(0x4278,'LOCO_INIT_OUT')
anim(0x427a,'LOCO_ANIM_IN',' base=%08X f16=%04X palettes=%08X f5f=%02X phase=%02X x=%04X y=%04X correction=%02X z=%04X f61=%02X',
 ',d@(a6+12),w@(a6+16),d@(a6+19),b@(a6+5f),b@(10000f),w@(a6+3c),w@(a6+40),b@(a6+4d),w@(a6+1e),b@(a6+61)')
anim(0x4350,'LOCO_ANIM_OUT',' sr=%04X',',sr')
anim(0x438e,'LOCO_RAW',' descriptor=%08X flags=%02X dx=%02X dy=%02X',',d7,d5&ff,d0&ff,d6&ff')
anim(0x4406,'LOCO_DRAW',' descriptor=%08X xy=%08X attr=%04X zreg=%04X',',d7,d0,d1&ffff,d5&ffff')
local mf=' frame=%d x=%08X y=%08X vx=%08X vy=%08X parent=%08X f15=%02X f5f=%02X f5a=%02X f6c=%04X f62=%08X f66=%04X f1e=%04X px=%08X py=%08X oldx=%08X oldy=%08X mark=%02X correction=%02X epoch=%02X'
local ma='temp9,d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+38),b@(a6+15),b@(a6+5f),b@(a6+5a),w@(a6+6c),d@(a6+62),w@(a6+66),w@(a6+1e),d@(a6+3c),d@(a6+40),d@(a6+44),d@(a6+48),b@(a6+4c),b@(a6+4d),b@(10c33a)'
local function motion(site,label,extra,args)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1 && d@(a6)==af572,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,mf,extra or '',ma,args or ''))
end
motion(0x1456a,'LOCO_GROUND_IN')
local regs=' d0=%08X d1=%08X d2=%08X d3=%08X d6=%08X d7=%08X a0=%08X p14=%08X p18=%08X p1c=%08X p20=%08X p25=%02X'
local regargs=',d0,d1,d2,d3,d6,d7,a0,d@(a0+14),d@(a0+18),d@(a0+1c),d@(a0+20),b@(a0+25)'
motion(0x13968,'LOCO_KERNEL_IN',regs..' mode=%08X mode8=%02X mode9=%02X modea=%02X moded=%02X',regargs..',d@(10c004),b@(10c008),b@(10c009),b@(10c00a),b@(10c00d)')
motion(0x14492,'LOCO_KERNEL_OUT',regs,regargs)
motion(0x14520,'LOCO_COMMIT_OUT')
