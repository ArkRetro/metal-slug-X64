-- Verified MAME 0.276 capture, mslugx euro BIOS. Set MSX_CAPTURE_DIR.
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local frame=0
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do
 for name,field in pairs(port.fields) do fields[name]=field end
end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0}}
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local function dump(f)
 local v=io.open(output..'/vram'..f..'.bin','wb')
 for i=0,0x85ff do
  cpu:write_u16(0x3c0000,i)
  v:write(string.pack('>I2',cpu:read_u16(0x3c0002)))
 end
 v:close()
 local p=io.open(output..'/palette'..f..'.bin','wb')
 for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
 p:close()
 manager.machine.screens[':screen']:snapshot(output..'/game'..f..'.png')
 print('CAPTURE '..f)
end
emu.register_frame_done(function()
 frame=frame+1
 if actions[frame] then local a=actions[frame]; print('ACTION '..frame..' '..a[1]..' '..a[2]..' exists='..tostring(fields[a[1]]~=nil)); if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame>=1550 and frame<=1700 then for tag,p in pairs(manager.machine.ioport.ports) do if string.find(tag,'JOY1') then for name,f in pairs(p.fields) do if name=='P1 A' or name=='%p A' then f:set_value(1) end end end end end
 if frame==1710 then for tag,p in pairs(manager.machine.ioport.ports) do if string.find(tag,'JOY1') then for name,f in pairs(p.fields) do if name=='P1 A' or name=='%p A' then f:set_value(0) end end end end end
 local function button(name,value)
  for tag,p in pairs(manager.machine.ioport.ports) do
   if string.find(tag,'JOY1') then for key,f in pairs(p.fields) do
    if key==name or key=='%p '..string.sub(name,4) then f:set_value(value) end
   end end
  end
 end
 if frame==2260 then button('P1 Right',1) end
 if frame==2300 then button('P1 B',1) end
 if frame==2310 then button('P1 B',0) end
 if frame>=2300 and frame<2360 and frame%2==0 then dump(frame) end
 if frame==2370 then manager.machine:exit() end
end, 'frame')
