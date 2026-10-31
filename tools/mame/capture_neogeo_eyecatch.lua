-- Reference capture of the Neo Geo BIOS eyecatch (spinning NEOGEO logo and its
-- jingle) with the user's own BIOS and cartridge (docs/LOGO_NEOGEO.md).
-- Metal Slug X declares "eyecatch done by the game" (68000 byte 0x114 = 1), so
-- the BIOS skips it. Only that header byte is cleared, in the emulated ROM
-- region, before the BIOS reads it: the BIOS then runs its own eyecatch with
-- the cartridge's sprite bank (byte 0x115) and sound driver. Nothing else is
-- patched and no input is injected.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local finish=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '900')
local dumps={}
for f in string.gmatch(os.getenv('MSX_DUMP_FRAMES') or '',"%d+") do dumps[tonumber(f)]=true end
local shot_step=tonumber(os.getenv('MSX_SHOT_STEP') or '10')
local log=assert(io.open(output..'/frames.jsonl','w'))
local region=assert(manager.machine.memory.regions[':cslot1:maincpu'],'cartridge program region')
local before=region:read_u16(0x114)
region:write_u16(0x114,before&0x00ff)
log:write(string.format('{"header_word_0x114_before":%d,"after":%d}\n',before,region:read_u16(0x114)))
local sprite=assert(manager.machine.devices[':spritegen'])
local vram=emu.item(assert(sprite.items['0/m_videoram']))
local cpu=manager.machine.devices[':maincpu'].spaces.program
local dbg=assert(manager.machine.debugger)
local screen=assert(manager.machine.screens[':screen'])
local frame=0
emu.register_frame_done(function()
 frame=frame+1;dbg:command(string.format('temp9=%x',frame))
 if frame==1 then
  dbg:command('wpset 320000:maincpu,1,w,1,{printf "AUDIO_COMMAND frame=%d pc=%08X value=%02X\\n",temp9,curpc,wpdata;g}')
 end
 log:write(string.format('{"frame":%d,"time":"%s"}\n',frame,tostring(manager.machine.time)));log:flush()
 if shot_step>0 and frame%shot_step==0 then screen:snapshot(string.format('shot%04d.png',frame)) end
 if dumps[frame] then
  screen:snapshot(string.format('frame%04d.png',frame))
  local v=assert(io.open(output..'/vram'..frame..'.bin','wb'))
  for a=0,0x85ff do v:write(string.pack('>I2',vram:read(a))) end
  v:close()
  local p=assert(io.open(output..'/palette'..frame..'.bin','wb'))
  for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
  p:close()
 end
 if frame>=finish then manager.machine:exit() end
end)
