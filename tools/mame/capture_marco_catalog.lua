-- Read-only sprite/RAM observations. Only scheduled controller input is changed.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local plan=dofile(output..'/plan.lua')
local cpu=manager.machine.devices[':maincpu'].spaces.program
local sprite=manager.machine.devices[':spritegen']
local video=emu.item(assert(sprite.items['0/m_videoram']))
local dbg=assert(manager.machine.debugger)
local frame=0
local inputs=assert(io.open(output..'/inputs.jsonl','w'))
local states=assert(io.open(output..'/states.jsonl','w'))
local function button(name,value)
 local found=false
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value);found=true end
  end
 end
 assert(found,'Unknown controller input '..name)
 inputs:write(string.format('{"frame":%d,"button":"%s","value":%d}\n',frame,name,value));inputs:flush()
end
-- The actor address is verified by the existing entry/air traces. This probe
-- does not assume its generic helper field meanings apply before observing them.
local function probe(site,label,format,args)
 dbg:command(string.format('bpset %x,temp9>=%x && a6==1014c4,{printf "%s frame=%%d %s\\n",temp9,%s;g}',site,plan.first,label,format,args))
end
if os.getenv('MSX_COMMAND_TRACE')=='1' then
 probe(0x429c,'MARCO_COMMAND',
  'cursor=%08X state=%08X timer=%04X speed=%04X flags=%04X f17=%02X correction=%02X f50=%08X f6c=%02X f72=%08X f76=%02X f77=%02X f78=%02X f80=%02X f81=%02X f90=%02X f91=%02X f94=%02X f98=%02X f9a=%02X',
  'a1,a0,w@(a0+4),w@(a0+6),w@(a6+14),b@(a6+17),b@(a6+4d),d@(a6+50),b@(a6+6c),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+80),b@(a6+81),b@(a6+90),b@(a6+91),b@(a6+94),b@(a6+98),b@(a6+9a)')
end
probe(0x4412,'MARCO_SELECT','cursor=%08X field=%02X value=%02X count=%04X state=%08X x=%04X y=%04X attributes=%04X z=%04X correction=%02X flash_palette=%02X palettes=%08X',
 'a1-1,b@(a1),b@(a6+b@(a1)),w@(a1+1)/8,a0,w@(a6+3c),w@(a6+40),w@(a6+14),w@(a6+1e),b@(a6+4d),b@(a6+16),d@(a6+19)')
probe(0x4256,'MARCO_INIT','program=%08X speed=%04X','a1,d0&ffff')
probe(0x4406,'MARCO_DRAW','descriptor=%08X xy=%08X attr=%04X z=%04X','d7,d0,d1&ffff,d5&ffff')
probe(0x44ec,'MARCO_CALLBACK','target=%08X','a1')
dbg:command('wpset 1014e4,4,w,temp9>='..string.format('%x',plan.first)..',{printf "MARCO_CURSOR_WRITE frame=%d pc=%08X a6=%08X address=%08X data=%08X\\n",temp9,pc,a6,wpaddr,wpdata;g}')
local boot={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1701]={'P1 A',0}}
emu.register_frame_done(function()
 frame=frame+1;dbg:command(string.format('temp9=%x',frame))
 if boot[frame] then button(boot[frame][1],boot[frame][2]) end
 for _,event in ipairs(plan.events[frame] or {}) do button(event[1],event[2]) end
 if frame>=plan.first and frame<=plan.last then
  local file=assert(io.open(output..'/actor'..frame..'.bin','wb'));file:write(cpu:read_range(0x1014c4,0x101573,8));file:close()
  states:write(string.format('{"frame":%d,"routine":%d,"cursor":%d,"x":%d,"y":%d,"projected_x":%d,"projected_y":%d,"flags":%d}\n',frame,cpu:read_u32(0x1014c4),cpu:read_u32(0x1014e4),cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x101500),cpu:read_u32(0x101504),cpu:read_u16(0x1014d8)));states:flush()
  local v=assert(io.open(output..'/vram'..frame..'.bin','wb'));for i=0,0x85ff do v:write(string.pack('>I2',video:read(i))) end;v:close()
  local p=assert(io.open(output..'/palette'..frame..'.bin','wb'));for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end;p:close()
  manager.machine.screens[':screen']:snapshot(output..'/game'..frame..'.png')
 end
 if frame==plan.first+2 then
  for _,r in ipairs({{'program-fixed.bin',0,0xfffff},{'program-bank.bin',0x200000,0x2fffff}}) do
   local file=assert(io.open(output..'/'..r[1],'wb'));file:write(cpu:read_range(r[2],r[3],8));file:close()
  end
 end
 if frame==plan.last then inputs:close();states:close();manager.machine:exit() end
end,'frame')
