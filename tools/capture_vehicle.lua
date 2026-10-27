-- Verified MAME 0.276 capture, mslugx euro BIOS. Set MSX_CAPTURE_DIR.
-- Reference-only survival/ammunition overrides are never shipped in the N64 game.
-- Set MSX_RAMP_DELAY_COMBAT=1 to pause attacks at frames 2880..3599,
-- then turn back towards the surviving vehicle and resume firing.
-- Set MSX_RAMP_RETURN_FIRE=0 to retain the original forward-facing control.
-- Compare measured activation across playthroughs; frames are not runtime triggers.
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local frame=0
local delay_combat=true
local states=assert(io.open(output..'/vehicle-states.csv','w'))
states:write('frame,object,routine,x16,y16,vx16,health,flags,animation,phase,target\n')
local return_fire_enabled=os.getenv('MSX_RAMP_RETURN_FIRE')~='0'
local trace=assert(io.open(output..'/ramp-writes.csv','w'))
trace:write('frame,address,old,new,pc,a0,a1,a2,a3,a4,a5,a6,d0,d1,d2,d3,object_routine,object_health,object_flags,object_x16,object_y16\n')
local changes=assert(io.open(output..'/ramp-states.csv','w'))
changes:write('frame,camera,player_x,cells\n')
local last_cells=nil
local event_capture_until=0
local last_x=-1;local stuck=0;local aim_until=0
local positions=assert(io.open(output..'/positions.csv','w'))
positions:write('frame,x16,y16,vx16,vy16,flags\n')
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do
 for name,field in pairs(port.fields) do fields[name]=field end
end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0}}
local cpu=manager.machine.devices[':maincpu'].spaces['program']
-- Read-only trace: do not write collision, enemy life or level-event state.
-- Retain the tap globally so Lua GC cannot remove it while emulation runs.
ramp_tap=cpu:install_write_tap(0x10c7b6,0x10d7b5,'ramp_collision_trace',function(address,data,mask)
 if frame<2800 or manager.machine.devices[':maincpu'].state['PC'].value~=0xc8982 then return end
 for lane=0,1 do
  local byte_address=address+lane
  local shift=lane==0 and 8 or 0
  if (mask&(0xff<<shift))~=0 then
   local old=cpu:read_u8(byte_address);local new=(data>>shift)&255
   if old~=new then
    local state=manager.machine.devices[':maincpu'].state
    local row={frame,string.format('%x',byte_address),old,new,string.format('%x',state['PC'].value)}
    for _,name in ipairs({'A0','A1','A2','A3','A4','A5','A6','D0','D1','D2','D3'}) do
     row[#row+1]=string.format('%x',state[name].value&0xffffffff)
    end
    local object=state['A6'].value&0xffffff
    row[#row+1]=string.format('%x',cpu:read_u32(object))
    row[#row+1]=cpu:read_u16(object+0x4e)
    row[#row+1]=cpu:read_u8(object+0x84)
    row[#row+1]=cpu:read_u32(object+0x28)
    row[#row+1]=cpu:read_u32(object+0x2c)
    trace:write(table.concat(row,',')..'\n');trace:flush()
    event_capture_until=frame+8
   end
  end
 end
end)
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
  cpu:write_u8(0x10c621,0x30);cpu:write_u8(0x10c627,0x63)
  local x=cpu:read_u16(0x102036)
  if x==last_x then stuck=stuck+1 else stuck=0 end
  last_x=x
  if frame<aim_until then stuck=0 end
  if stuck>120 then aim_until=frame+300;stuck=0 end
  if frame>=3600 then cpu:write_u8(0x10c71e,0) end -- Original pistol, measure actual damage.
  local return_fire=delay_combat and return_fire_enabled and frame>=3600
  button('P1 Right',return_fire and 0 or 1)
  button('P1 Left',return_fire and frame<3602 and 1 or 0)
  button('P1 Up',not return_fire and frame<aim_until and 1 or 0)
  local paused=delay_combat and frame>=2880 and frame<3600
  button('P1 A',not paused and frame%12<6 and 1 or 0)
  button('P1 B',frame>3000 and frame%120<8 and 1 or 0)
  button('P1 C',not paused and frame>2900 and frame%60<8 and 1 or 0)
 end
 if frame>=2100 then positions:write(string.format('%d,%u,%u,%u,%u,%u\n',frame,cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x1014f4),cpu:read_u32(0x1014f8),cpu:read_u8(0x1014d9))) end
 if frame>=2800 then
  for address=0x102000,0x107ffc,4 do
   local routine=cpu:read_u32(address)
   if routine>=0x29168 and routine<0x29b62 and cpu:read_u32(address+0x38)==0x10c72e then
    states:write(string.format('%d,%x,%x,%u,%u,%u,%u,%u,%x,%d,%u\n',frame,address,routine,
     cpu:read_u32(address+0x28),cpu:read_u32(address+0x2c),cpu:read_u32(address+0x30),
     cpu:read_u16(address+0x4e),cpu:read_u8(address+0x84),cpu:read_u32(address+0x20),
     cpu:read_u16(address+0x24),cpu:read_u16(address+0x7a)))
   end
  end
  local values={}
  for _,address in ipairs({0x10caad,0x10caec,0x10cb2b,0x10cb6b,0x10cbab,0x10cbeb,0x10cc2b,0x10cc6b,0x10ccab,0x10cceb}) do
   values[#values+1]=string.format('%02x',cpu:read_u8(address))
  end
  local cells=table.concat(values,'')
  changes:write(string.format('%d,%u,%f,%s\n',frame,cpu:read_u16(0x102036),cpu:read_u32(0x1014ec)/65536,cells))
  if cells~=last_cells then event_capture_until=frame+8 end
  last_cells=cells
  if frame%60==0 or frame<=event_capture_until or (frame>=3600 and frame<=3648) then dump(frame) end
 end
 if frame==4800 then positions:close();trace:close();changes:close();states:close();manager.machine:exit() end
end, 'frame')
