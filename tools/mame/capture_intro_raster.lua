-- Focused title raster probe layered on the read-only intro/audio capture.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'capture_intro_audio.lua')
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local screen=manager.machine.screens[':screen']
local sprite=manager.machine.devices[':spritegen']
local offset=emu.item(assert(sprite.items['0/m_vram_offset']))
local memory=emu.item(assert(sprite.items['0/m_videoram']))
local function line()
 local total=math.floor(screen.frame_period/screen.scan_period+0.5)
 return math.floor((screen.frame_period-screen:time_until_pos(0))/screen.scan_period+1e-7)%total
end
local first=tonumber(os.getenv('MSX_RASTER_FIRST') or '350')
local last=tonumber(os.getenv('MSX_RASTER_LAST') or '364')
local frame=0
local file=assert(io.open(output..'/raster.jsonl','w'))
local tap=manager.machine.devices[':maincpu'].spaces.program:install_write_tap(0x3c0000,0x3c0005,'intro_raster',function(address,data,mask)
 if frame>=first and frame<=last then
  file:write(string.format('{"frame":%d,"time":"%s","line":%d,"address":%d,"data":%d,"mask":%d,"vram_offset":%d,"word":%d}\n',frame,tostring(manager.machine.time),line(),address,data,mask,offset:read(0),memory:read(offset:read(0))))
  file:flush()
 end
end)
local palette_bank=emu.item(assert(manager.machine.devices[':'].items['0/m_palette_bank']))
local bank_tap=manager.machine.devices[':maincpu'].spaces.program:install_write_tap(0x3a000e,0x3a001f,'intro_palette_bank',function(address,data,mask)
 if frame>=first and frame<=last and (address&0x0e)==0x0e and (mask&0xff)~=0 then
  file:write(string.format('{"frame":%d,"time":"%s","line":%d,"palette_bank_write":true,"address":%d,"mask":%d,"bank_before":%d}\n',frame,tostring(manager.machine.time),line(),address,mask,palette_bank:read(0)));file:flush()
 end
end)
local cpu=manager.machine.devices[':maincpu'].spaces.program
local palette_tap=cpu:install_write_tap(0x400000,0x401fff,'intro_palette_raster',function(address,data,mask)
 if frame>=first and frame<=last then
  file:write(string.format('{"frame":%d,"time":"%s","line":%d,"address":%d,"data":%d,"mask":%d,"palette_index":%d,"word":%d,"palette_bank":%d}\n',frame,tostring(manager.machine.time),line(),address,data,mask,(address-0x400000)//2,cpu:read_u16(address),palette_bank:read(0)));file:flush()
 end
end)
emu.register_frame_done(function()
 frame=frame+1
 if frame>=first and frame<=last+1 then
  file:write(string.format('{"boundary":%d,"time":"%s","line":%d}\n',frame,tostring(manager.machine.time),line()));file:flush()
 end
 if frame==last+26 then tap:remove();palette_tap:remove();bank_tap:remove();file:close() end
end,'frame')
