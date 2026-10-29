-- Discovery trace, MAME 0.276 / mslugx euro. No gameplay RAM overrides.
-- Known snapshot routine addresses are candidates, not assumed soldier identities.
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local frame=0
local end_frame=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '2460')
assert(end_frame>=2460 and end_frame<=6000)
local trace_stopped=false
local trace_first_frame=nil
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do
 for name,field in pairs(port.fields) do fields[name]=field end
end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0}}
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local dbg=assert(manager.machine.debugger,'Launch MAME with -debug -debugger none')
local commands=assert(io.open(output..'/commands.log','w'))
local function command(text)
 commands:write(text..'\n');commands:flush();dbg:command(text)
end
local function dump_video(f)
 local v=assert(io.open(output..'/vram'..f..'.bin','wb'))
 for i=0,0x85ff do cpu:write_u16(0x3c0000,i);v:write(string.pack('>I2',cpu:read_u16(0x3c0002))) end
 v:close()
 local p=assert(io.open(output..'/palette'..f..'.bin','wb'))
 for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
 p:close();manager.machine.screens[':screen']:snapshot(output..'/game'..f..'.png')
end
emu.register_frame_done(function()
 frame=frame+1
 dbg:command(string.format("temp9=%x",frame))
 if actions[frame] then local a=actions[frame]; print('ACTION '..frame..' '..a[1]..' '..a[2]..' exists='..tostring(fields[a[1]]~=nil)); if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame>=1550 and frame<=1700 then for tag,p in pairs(manager.machine.ioport.ports) do if string.find(tag,'JOY1') then for name,f in pairs(p.fields) do if name=='P1 A' or name=='%p A' then f:set_value(1) end end end end end
 if frame==1710 then for tag,p in pairs(manager.machine.ioport.ports) do if string.find(tag,'JOY1') then for name,f in pairs(p.fields) do if name=='P1 A' or name=='%p A' then f:set_value(0) end end end end end
 if not trace_stopped then
  local trace=io.open(output..'/instructions.log','r')
  if trace then
   trace:close();if not trace_first_frame then trace_first_frame=frame end
   if frame>=trace_first_frame+4 then command('trace off,maincpu');trace_stopped=true;print('TRACE_STOP frame='..frame) end
  end
 end
 if frame==2200 then
  dbg:command('temp1=0;temp0=0')
  command('wpset 101f14,10,w,wpaddr!=101f16 || wpdata!=f17e,{printf "WP_SLOT frame=%d addr=%08X data=%08X pc=%08X a0=%08X a1=%08X a6=%08X sp=%08X\\n",temp9,wpaddr,wpdata,pc,a0,a1,a6,sp;g}')
  local trace_action='temp1=1;printf "CONSTRUCTOR_WRITE frame=%d pc=%08X a6=%08X slot=%08X value=%08X\\n",temp9,pc,a6,wpaddr,wpdata;temp0=0;trace '..output..'/instructions.log,maincpu,noloop,{tracelog "A6=%08X A0=%08X D0=%08X ",a6,a0,d0;temp0++};g'
  command('wpset 101f16,2,w,wpdata==f17e && temp1==0,{'..trace_action..'}')
  for _,address in ipairs({0x0af17e,0x0af65a,0x0af6c0}) do
   local action='printf "BP frame=%d pc=%08X a6=%08X ptr=%08X sp=%08X\\n",temp9,pc,a6,d@(a6),sp;g'
   command(string.format('bpset %x,a6==101f14 && temp9<937,{%s}',address,action))
  end
 end
 if frame==2280 then
  for tag,p in pairs(manager.machine.ioport.ports) do
   if string.find(tag,'JOY1') then for name,f in pairs(p.fields) do
    if name=='P1 Right' or name=='%p Right' then f:set_value(1) end
   end end
  end
 end
 local stop_right=tonumber(os.getenv('MSX_STOP_RIGHT_FRAME') or '0')
 if frame==stop_right then
  for tag,p in pairs(manager.machine.ioport.ports) do
   if string.find(tag,'JOY1') then for name,f in pairs(p.fields) do
    if name=='P1 Right' or name=='%p Right' then f:set_value(0) end
   end end
  end
 end
 if frame>=2100 and frame<=end_frame then
  local ram=assert(io.open(output..'/ram'..frame..'.bin','wb'))
  ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 end
 if frame>=2300 and frame<=2440 and frame%20==0 then dump_video(frame) end
 if frame==2360 then
  for _,region in ipairs({{'program-fixed.bin',0x000000,0x0fffff},{'program-bank.bin',0x200000,0x2fffff},{'bios.bin',0xc00000,0xc1ffff}}) do
   local file=assert(io.open(output..'/'..region[1],'wb'));file:write(cpu:read_range(region[2],region[3],8));file:close()
  end
 end
 if frame==end_frame then
  dbg:command('trace off,maincpu');command('bplist');command('wplist')
  local log=dbg.consolelog;local file=assert(io.open(output..'/debugger-console.log','w'))
  for i=1,#log do if log[i] then file:write(log[i]..'\n') end end
  file:close();manager.machine:exit()
 end
end, 'frame')
