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
local frame=0
local file=assert(io.open(output..'/raster.jsonl','w'))
local tap=manager.machine.devices[':maincpu'].spaces.program:install_write_tap(0x3c0000,0x3c0005,'intro_raster',function(address,data,mask)
 if frame>=350 and frame<=364 then
  file:write(string.format('{"frame":%d,"time":"%s","line":%d,"address":%d,"data":%d,"mask":%d,"vram_offset":%d,"word":%d}\n',frame,tostring(manager.machine.time),line(),address,data,mask,offset:read(0),memory:read(offset:read(0))))
  file:flush()
 end
end)
emu.register_frame_done(function()
 frame=frame+1
 if frame>=350 and frame<=365 then
  file:write(string.format('{"boundary":%d,"time":"%s","line":%d}\n',frame,tostring(manager.machine.time),line()));file:flush()
 end
 if frame==390 then tap:remove();file:close() end
end,'frame')
