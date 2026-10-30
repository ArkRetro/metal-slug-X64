-- Campaign soldier evidence: per-frame RAM (capture_soldier_campaign.lua),
-- every original RNG call (ROM 0029C6) with its caller actor and return PC,
-- and palette RAM every MSX_PALETTE_STEP frames. Requires -debug -debugger none.
-- Only breakpoint actions that print; no gameplay RAM or register writes.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'capture_soldier_campaign.lua')
local output=os.getenv('MSX_CAPTURE_DIR')
local first=tonumber(os.getenv('MSX_RAM_FIRST') or '2100')
local last=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '3600')
local step=tonumber(os.getenv('MSX_PALETTE_STEP') or '20')
-- MSX_VRAM_STEP>0: sprite VRAM (read-only item 0/m_videoram) and a screen
-- snapshot together with the palette every MSX_VRAM_STEP frames.
local vram_step=tonumber(os.getenv('MSX_VRAM_STEP') or '0')
local video=vram_step>0 and emu.item(assert(manager.machine.devices[':spritegen'].items['0/m_videoram'])) or nil
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local dbg=assert(manager.machine.debugger,'Launch MAME with -debug -debugger none')
local frame=0
emu.register_frame_done(function()
 frame=frame+1
 dbg:command(string.format('temp9=%x',frame))
 if frame==first-1 then
  dbg:command('bpset 29c6,1,{printf "RNG f=%d a6=%06X ret=%06X cur=%04X",temp9,a6,d@(sp),w@(100084);g}')
  -- Level-script spawns (01A10A: a0 = record, a6 = script/runner actor) and
  -- runner creations (01A1E8), for an exact script fixture.
  dbg:command('bpset 1a10a,1,{printf "SPAWN f=%d a6=%06X rec=%06X",temp9,a6,a0;g}')
  dbg:command('bpset 1a1e8,1,{printf "RUNNER f=%d a6=%06X rec=%06X",temp9,a6,a0;g}')
 end
 if frame>=first and frame<=last and (frame-first)%step==0 then
  local p=assert(io.open(output..'/palette'..frame..'.bin','wb'))
  for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
  p:close()
 end
 if video and frame>=first and frame<=last and (frame-first)%vram_step==0 then
  local v=assert(io.open(output..'/vram'..frame..'.bin','wb'))
  for i=0,0x85ff do v:write(string.pack('>I2',video:read(i))) end
  v:close()
  if (frame-first)%step~=0 then
   local p=assert(io.open(output..'/palette'..frame..'.bin','wb'))
   for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
   p:close()
  end
  manager.machine.screens[':screen']:snapshot(output..'/game'..frame..'.png')
 end
end,'frame')
