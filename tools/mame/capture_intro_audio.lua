-- Reference capture of original boot, character confirmation and mission entry.
-- Input only: no gameplay RAM patches, no fast-forward/skip inputs after confirm.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local finish=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '2400')
local cpu=manager.machine.devices[':maincpu'].spaces.program
local dbg=assert(manager.machine.debugger)
local sprite=assert(manager.machine.devices[':spritegen'])
local video_memory=emu.item(assert(sprite.items['0/m_videoram']))
local animation_counter=emu.item(assert(sprite.items['0/m_auto_animation_counter']))
local animation_disabled=emu.item(assert(sprite.items['0/m_auto_animation_disabled']))
local fixed_source=emu.item(assert(sprite.items['0/m_fixed_layer_source']))
local palette_bank=emu.item(assert(manager.machine.devices[':'].items['0/m_palette_bank']))
local palette_memory=emu.item(assert(manager.machine.devices[':'].items['0/m_paletteram']))
local screen_shadow=emu.item(assert(manager.machine.devices[':'].items['0/m_screen_shadow']))
local players=assert(io.open(output..'/player.jsonl','w'))
local shot_first=tonumber(os.getenv('MSX_SHOT_FIRST') or '0')
local shot_last=tonumber(os.getenv('MSX_SHOT_LAST') or '0')
local video_step=tonumber(os.getenv('MSX_VIDEO_STEP') or '2')
local frame=0
local inputs=assert(io.open(output..'/inputs.jsonl','w'))
local timing=assert(io.open(output..'/frames.jsonl','w'))
local function button(name,value)
 local found=false
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value);found=true end
  end
 end
 assert(found,'Missing input '..name)
 inputs:write(string.format('{"frame":%d,"name":"%s","value":%d}\n',frame,name,value));inputs:flush()
end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0}}
local confirm=tonumber(os.getenv('MSX_CONFIRM_FRAME') or '0')
if confirm>0 then actions[confirm]={'P1 A',1};actions[confirm+10]={'P1 A',0} end
local function dump()
 local video=assert(io.open(output..'/vram'..frame..'.bin','wb'))
 for address=0,0x85ff do video:write(string.pack('>I2',video_memory:read(address))) end
 video:close()
 local pal=assert(io.open(output..'/palette'..frame..'.bin','wb'))
 for i=0,4095 do pal:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
 pal:close()
 local banks=assert(io.open(output..'/palette-banks'..frame..'.bin','wb'))
 for i=0,8191 do banks:write(string.pack('>I2',palette_memory:read(i))) end
 banks:close()
end
emu.register_frame_done(function()
 frame=frame+1;dbg:command(string.format('temp9=%x',frame))
 if frame==1 then
  dbg:command('wpset 320000:maincpu,1,w,1,{printf "AUDIO_COMMAND frame=%d pc=%08X value=%02X\\n",temp9,pc,wpdata;g}')
  -- Driver mame0276 src/mame/neogeo/neogeo.cpp:1799..1802: ports 4..7,
  -- mirrored through the Z80 upper address byte. Retain full addresses.
  dbg:command('wpiset 0:audiocpu,10000,w,(wpaddr&ff)>=4 && (wpaddr&ff)<=7,{printf "YM_WRITE frame=%d port=%04X value=%02X\\n",temp9,wpaddr,wpdata;g}')
 end
 if actions[frame] then button(actions[frame][1],actions[frame][2]) end
 timing:write(string.format('{"frame":%d,"time":"%s","animation_counter":%d,"animation_disabled":%d,"fixed_source":%d,"palette_bank":%d,"screen_shadow":%d}\n',frame,tostring(manager.machine.time),animation_counter:read(0),animation_disabled:read(0),fixed_source:read(0),palette_bank:read(0),screen_shadow:read(0)));timing:flush()
 if frame>=3800 then
  players:write(string.format('{"frame":%d,"routine":%d,"x":%d,"y":%d,"vx":%d,"vy":%d,"flags15":%d}\n',frame,cpu:read_u32(0x1014c4),cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x1014f4),cpu:read_u32(0x1014f8),cpu:read_u8(0x1014d9)));players:flush()
 end
 if frame%video_step==0 then dump() end
 if frame%60==0 or (frame>=shot_first and frame<=shot_last) then manager.machine.screens[':screen']:snapshot(output..'/game'..frame..'.png') end
 if frame==finish then inputs:close();timing:close();players:close();manager.machine:exit() end
end,'frame')
