-- Exact attack animation/callback and spawned-actor motion boundaries.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_spawned_actor.lua')
local dbg=assert(manager.machine.debugger)
-- MAME runs only the first matching breakpoint action at an address. Replace
-- the broad parent animation probes while keeping the child's disjoint ones.
local cpu_debug=manager.machine.devices[':maincpu'].debug
for index,bp in pairs(cpu_debug:bplist()) do
 if (bp.address==0x4256 or bp.address==0x427a or bp.address==0x4350) and
    bp.action:find('STATE_ANIM_',1,true) then cpu_debug:bpclear(index) end
end
for index,bp in pairs(cpu_debug:bplist()) do
 if bp.address==0x143e4 and bp.action:find('CHILD_AIR_IN',1,true) then cpu_debug:bpclear(index) end
end
local sf=' frame=%d ptr=%08X timer=%04X speed=%04X f17=%02X f18=%02X field50=%08X'
local sa='temp9,d@(a6+20),w@(a6+24),w@(a6+26),b@(a6+17),b@(a6+18),d@(a6+50)'
local function anim(site,label,extra,args,condition)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1 && %s,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,condition or 'd@(a6)==af964',label,site,sf,extra or '',sa,args or ''))
end
anim(0x4256,'ATTACK_INIT_IN',' program=%08X requested_speed=%04X',',a1,d0&ffff','a1==23d7ee')
anim(0x4278,'ATTACK_INIT_OUT','','','d@(a6+20)==23d7ee')
anim(0x427a,'ATTACK_IN',' base=%08X f16=%04X palettes=%08X f5f=%02X phase=%02X x=%04X y=%04X correction=%02X z=%04X',
 ',d@(a6+12),w@(a6+16),d@(a6+19),b@(a6+5f),b@(10000f),w@(a6+3c),w@(a6+40),b@(a6+4d),w@(a6+1e)')
anim(0x4350,'ATTACK_OUT',' sr=%04X palettes=%08X',',sr,d@(a6+19)')
anim(0x438e,'ATTACK_RAW',' descriptor=%08X flags=%02X dx=%02X dy=%02X',',d7,d5&ff,d0&ff,d6&ff')
anim(0x4406,'ATTACK_DRAW',' descriptor=%08X xy=%08X attr=%04X zreg=%04X',',d7,d0,d1&ffff,d5&ffff')
local env=' parent_x=%08X parent_y=%08X oldx=%08X oldy=%08X scene_x=%08X scene_y=%08X scene_oldx=%08X scene_oldy=%08X'
local enva=',d@(a6+3c),d@(a6+40),d@(a6+44),d@(a6+48),d@(10e97c),d@(10e980),d@(10e984),d@(10e988)'
anim(0x44ec,'ATTACK_CALL_IN',' target=%08X attr=%04X palettes=%08X'..env,',a1,w@(a6+14),d@(a6+19)'..enva)
anim(0x44ee,'ATTACK_CALL_OUT',' attr=%04X palettes=%08X child=%08X cx=%08X cy=%08X cvx=%08X cvy=%08X cparent=%08X cflags=%02X',
 ',w@(a6+14),d@(a6+19),a0,d@(a0+28),d@(a0+2c),d@(a0+30),d@(a0+34),d@(a0+38),b@(a0+15)')
local mf=' frame=%d x=%08X y=%08X vx=%08X vy=%08X parent=%08X f15=%02X f5f=%02X f5a=%02X f6c=%04X f62=%08X f66=%04X f1e=%04X px=%08X py=%08X oldx=%08X oldy=%08X mark=%02X correction=%02X epoch=%02X'
local ma='temp9,d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+38),b@(a6+15),b@(a6+5f),b@(a6+5a),w@(a6+6c),d@(a6+62),w@(a6+66),w@(a6+1e),d@(a6+3c),d@(a6+40),d@(a6+44),d@(a6+48),b@(a6+4c),b@(a6+4d),b@(10c33a)'
local function motion(site,label,extra,args)
 dbg:command(string.format('bpset %x,a6==temp6 && temp7==1,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,mf,extra or '',ma,args or ''))
end
motion(0x143e4,'THROW_AIR_IN')
local regs=' d0=%08X d1=%08X d2=%08X d3=%08X d4=%08X d6=%08X d7=%08X a0=%08X'
local regargs=',d0,d1,d2,d3,d4,d6,d7,a0'
local parent=' p14=%08X p18=%08X p1c=%08X p20=%08X p25=%02X'
local parentargs=',d@(a0+14),d@(a0+18),d@(a0+1c),d@(a0+20),b@(a0+25)'
motion(0x13968,'THROW_KERNEL_IN',regs..parent..' mode=%08X mode8=%02X mode9=%02X modea=%02X moded=%02X',regargs..parentargs..',d@(10c004),b@(10c008),b@(10c009),b@(10c00a),b@(10c00d)')
motion(0x14492,'THROW_KERNEL_OUT',regs..parent,regargs..parentargs)
motion(0x14520,'THROW_COMMIT_OUT',regs,regargs)
motion(0x3bc8e,'THROW_INIT_TARGET',' active=%02X p1x=%04X p1y=%04X p2x=%04X p2y=%04X f6e=%04X',',b@(10c33c),w@(101500),w@(101504),w@(1015b0),w@(1015b4),w@(a6+6e)')
motion(0x3bd3a,'THROW_FLIGHT_IN')
motion(0x3bd5a,'THROW_FLIGHT_OUT')
