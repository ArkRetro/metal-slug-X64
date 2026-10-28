-- Main attack state and both contact-wrapper calls around the recovered animation.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_attack.lua')
local dbg=assert(manager.machine.debugger)
local sf=' frame=%d routine=%08X f61=%02X f62=%08X speed=%04X f72=%04X f78=%04X f7e=%02X f81=%02X f15=%02X f5f=%02X f68=%02X f6e=%04X f70=%04X f74=%04X f7c=%02X f80=%02X f84=%08X f88=%08X f8c=%04X f8e=%04X f90=%04X f92=%04X'
local sa='temp9,d@(a6),b@(a6+61),d@(a6+62),w@(a6+26),w@(a6+72),w@(a6+78),b@(a6+7e),b@(a6+81),b@(a6+15),b@(a6+5f),b@(a6+68),w@(a6+6e),w@(a6+70),w@(a6+74),b@(a6+7c),b@(a6+80),d@(a6+84),d@(a6+88),w@(a6+8c),w@(a6+8e),w@(a6+90),w@(a6+92)'
local function state(site,label)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1,{printf "%s site=%06X%s\\n",%s;g}',site,label,site,sf,sa))
end
state(0xaf918,'ATTACK_STATE_INIT');state(0xaf964,'ATTACK_STATE_BODY');state(0xaf994,'ATTACK_STATE_END')
local mf=' frame=%d x=%08X y=%08X vx=%08X vy=%08X parent=%08X f15=%02X f5f=%02X f5a=%02X f6c=%04X f62=%08X f66=%04X f1e=%04X px=%08X py=%08X oldx=%08X oldy=%08X mark=%02X correction=%02X epoch=%02X'
local ma='temp9,d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+38),b@(a6+15),b@(a6+5f),b@(a6+5a),w@(a6+6c),d@(a6+62),w@(a6+66),w@(a6+1e),d@(a6+3c),d@(a6+40),d@(a6+44),d@(a6+48),b@(a6+4c),b@(a6+4d),b@(10c33a)'
local function motion(site,label,extra,args)
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1 && d@(a6)==af964,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,mf,extra or '',ma,args or ''))
end
motion(0x1436a,'ATTACK_STATIONARY_IN');motion(0xaf96a,'ATTACK_STATIONARY_OUT');motion(0x1456a,'ATTACK_GROUND_IN')
local regs=' d0=%08X d1=%08X d2=%08X d3=%08X d6=%08X d7=%08X a0=%08X p14=%08X p18=%08X p1c=%08X p20=%08X p25=%02X'
local regargs=',d0,d1,d2,d3,d6,d7,a0,d@(a0+14),d@(a0+18),d@(a0+1c),d@(a0+20),b@(a0+25)'
motion(0x13968,'ATTACK_CONTACT_IN',regs..' mode=%08X mode8=%02X mode9=%02X modea=%02X moded=%02X',regargs..',d@(10c004),b@(10c008),b@(10c009),b@(10c00a),b@(10c00d)')
motion(0x14492,'ATTACK_CONTACT_OUT',regs,regargs)
motion(0x14520,'ATTACK_CONTACT_COMMIT')
