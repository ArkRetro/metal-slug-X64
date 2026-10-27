-- Verified MAME 0.276 capture, mslugx euro BIOS. Set MSX_CAPTURE_DIR.
-- Reference-only RAM overrides are never shipped in the N64 game.
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local frame=0
local last_x=-1;local stuck=0;local aim_until=0
local positions=assert(io.open(output..'/positions.csv','w'))
positions:write('frame,x16,y16,vx16,vy16,flags\n')
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
 local ram=assert(io.open(output..'/ram'..f..'.bin','wb'));ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
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
 if frame>=2000 then
  cpu:write_u8(0x1013de,0x5a) -- MAME cheat.dat reference: timer.
  cpu:write_u8(0x101523,cpu:read_u8(0x101523)|0x40) -- P1 invulnerability, capture only.
  cpu:write_u8(0x101549,0x99)
  cpu:write_u8(0x101288,2)
 end
 if frame==2190 then button('P1 Right',1) end
 if frame==2100 then cpu:write_u8(0x10c71e,3) end
 if frame>=2190 then
  cpu:write_u8(0x10c725,0xff);cpu:write_u8(0x10c720,0x63)
  local x=cpu:read_u32(0x1014ec)
  if x==last_x then stuck=stuck+1 else stuck=0 end
  last_x=x
  if stuck>120 and frame>=aim_until then aim_until=frame+600;stuck=0 end
  button('P1 Right',frame<aim_until and 0 or 1)
  button('P1 Up',frame<aim_until and 1 or 0)
  button('P1 A',frame%12<6 and 1 or 0)
  button('P1 B',frame>3000 and frame%120<8 and 1 or 0)
  button('P1 C',frame>2900 and frame%60<8 and 1 or 0)
 end
 if frame>=2100 then positions:write(string.format('%d,%u,%u,%u,%u,%u\n',frame,cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x1014f4),cpu:read_u32(0x1014f8),cpu:read_u8(0x1014d9))) end
 if frame>=2100 and (frame==2100 or frame%120==0) then dump(frame) end
 if frame==9000 then positions:close();manager.machine:exit() end
end, 'frame')
