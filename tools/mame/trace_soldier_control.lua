local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
assert(os.getenv('MSX_RNG_CAPTURE_SR')=='1','Launch runner with --rng-flags')
dofile(here..'trace_soldier_rng.lua')
local dbg=assert(manager.machine.debugger)
local fmt=' frame=%d f15=%02X f5f=%02X f68=%02X f6e=%04X f70=%04X f74=%04X f7c=%02X f80=%02X f84=%08X f88=%08X f8c=%04X f8e=%04X f90=%04X f92=%04X px=%08X py=%08X active=%02X mode88=%02X rank=%02X p1active=%d p2active=%d p1x=%04X p1y=%04X p2x=%04X p2y=%04X originx=%04X originy=%04X seq=%d'
local args='temp9,b@(a6+15),b@(a6+5f),b@(a6+68),w@(a6+6e),w@(a6+70),w@(a6+74),b@(a6+7c),b@(a6+80),d@(a6+84),d@(a6+88),w@(a6+8c),w@(a6+8e),w@(a6+90),w@(a6+92),d@(a6+3c),d@(a6+40),b@(10c33c),b@(100088),b@(10fd8b),d@(1014c4)!=3f1e,d@(101574)!=3f1e,w@(101500),w@(101504),w@(1015b0),w@(1015b4),w@(10e97c),w@(10e980),temp2'
local function probe(site,label,extra_fmt,extra_args)
 if label=='CONTROL_IN' then
  local grid={};for address=0x1067e6,0x1068e2,4 do grid[#grid+1]=string.format('d@(%x)',address) end
  extra_fmt=(extra_fmt or '')..' grid='..string.rep('%08X',64)
  extra_args=(extra_args or '')..','..table.concat(grid,',')
 end
 dbg:command(string.format('bpset %x,a6==101f14 && temp1==1,{printf "%s site=%06X%s%s\\n",%s%s;g}',site,label,site,fmt,extra_fmt or '',args,extra_args or ''))
end
probe(0xae9c0,'TARGET_IN')
probe(0xaeb3e,'TARGET_OUT')
probe(0xae9da,'PLAYER_TARGETS',' d0=%08X d1=%08X d2=%08X d3=%08X d7=%08X',',d0,d1,d2,d3,d7')
probe(0xaeb5e,'CONTROL_IN')
probe(0xaec3c,'PROBABILITY_OUT',' threshold=%04X',',d0&ffff')
probe(0xaeb40,'DELAY_UPDATE_IN',' value=%04X targetx=%04X targety=%04X',',d0&ffff,d2&ffff,d3&ffff')
probe(0xaed1c,'CONTROL_PREFIX_OUT')
probe(0xaed54,'PROBE1_OUT',' d6=%08X',',d6')
probe(0xaed98,'PROBE2_OUT',' d6=%08X',',d6')
probe(0xaeda6,'CONTROL_OUT')
dbg:command('wpset 101f73,1,w,temp1==1,{printf "CONTROL_WRITE frame=%d address=%08X value=%02X reported_pc=%08X\\n",temp9,wpaddr,wpdata,pc;g}')
-- Snapshot the dynamic 16x8-word lookup consumed by 00510C each control call.
-- Register frame_done runs after the original dumper; write all mission frames.
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local output=assert(os.getenv('MSX_CAPTURE_DIR'));local frame=0
emu.register_frame_done(function()
 frame=frame+1
 if frame>=2350 and frame<=2460 then
  local f=assert(io.open(output..'/delay'..frame..'.bin','wb'))
  f:write(cpu:read_range(0x1067e6,0x1068e5,8));f:close()
 end
end,'frame')
