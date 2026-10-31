-- MAME 0.264, mslugx euro BIOS. Reference capture of the presentation
-- sequences NOT covered by tools/mame/capture_intro_audio.lua
-- (docs/INTRO_AUDIO.md covers boot, title, tutorial, select and the Mission 1
-- entry after a coin). Set MSX_CAPTURE_DIR and MSX_SEQ_MODE:
--   attract   no input at all: the full attract loop (title, demo play,
--             rankings ...) until MSX_SEQ_LAST (default 40000) frames.
--   gameover  coin, Start, Marco confirmed as in capture_mission_rest.lua, then
--             only 'hold Right' from frame 2000 (no fire) and NO RAM override:
--             the player walks into the enemies and dies, then the continue
--             countdown and the game-over / ranking screens play.
-- Input only; no RAM is written. Video memory is read through the saved item
-- ':spritegen/0/m_videoram' (no VRAM register access).
-- Outputs: frames.csv (every frame: script position, section, camera, LSPC
-- auto-animation state, FIX source, palette bank, backdrop, a hash of the FIX
-- map and of the palette RAM) and every MSX_SEQ_PERIOD (30) frames
-- vram|palette|ram<frame>.bin and game<frame>.png.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local mode=os.getenv('MSX_SEQ_MODE') or 'attract'
local last=tonumber(os.getenv('MSX_SEQ_LAST') or '40000')
local period=tonumber(os.getenv('MSX_SEQ_PERIOD') or '30')
local machine=manager.machine
local cpu=machine.devices[':maincpu'].spaces.program
local sprite=assert(machine.devices[':spritegen'])
local video_memory=emu.item(assert(sprite.items['0/m_videoram']))
local animation_counter=emu.item(assert(sprite.items['0/m_auto_animation_counter']))
local animation_disabled=emu.item(assert(sprite.items['0/m_auto_animation_disabled']))
local fixed_source=emu.item(assert(sprite.items['0/m_fixed_layer_source']))
local palette_bank=emu.item(assert(machine.devices[':'].items['0/m_palette_bank']))
local frame=0
local SCRIPT=0x101414
local sections={}
for i=0,29 do sections[#sections+1]={index=i,base=cpu:read_u32(0x800+4*i)} end
local function section_of(pc)
 if pc==0 then return -1 end
 local s,best=-1,-1
 for _,e in ipairs(sections) do if e.base<=pc and e.base>best then s=e.index;best=e.base end end
 return s
end
local frames=assert(io.open(output..'/frames.csv','w'))
frames:write('frame,script_pc,section,camera,auto_counter,auto_disabled,fix_source,palette_bank,backdrop,fix_hash,palette_hash,player_routine,inputs\n')
local actions={}
if mode=='gameover' then
 actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1710]={'P1 A',0}}
end
local held={}
local function button(name,value)
 for tag,port in pairs(machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value) end
  end
 end
 held[name]=value==1 or nil
end
local function hash(words)
 local h=0
 for _,w in ipairs(words) do h=(h*33+w)%4294967291 end
 return h
end
local function dump()
 local parts={}
 for i=0,0x85ff do parts[#parts+1]=string.pack('>I2',video_memory:read(i)) end
 local v=assert(io.open(output..'/vram'..frame..'.bin','wb'));v:write(table.concat(parts));v:close()
 parts={}
 for i=0,4095 do parts[#parts+1]=string.pack('>I2',cpu:read_u16(0x400000+i*2)) end
 local p=assert(io.open(output..'/palette'..frame..'.bin','wb'));p:write(table.concat(parts));p:close()
 local r=assert(io.open(output..'/ram'..frame..'.bin','wb'));r:write(cpu:read_range(0x100000,0x10ffff,8));r:close()
 machine.screens[':screen']:snapshot(output..'/game'..frame..'.png')
end
emu.register_frame_done(function()
 frame=frame+1
 if actions[frame] then button(actions[frame][1],actions[frame][2]) end
 if mode=='gameover' and frame==2000 then button('P1 Right',1) end
 local fix={}
 for i=0x7000,0x74ff,3 do fix[#fix+1]=video_memory:read(i) end
 local pal={}
 for i=0,4095,7 do pal[#pal+1]=cpu:read_u16(0x400000+i*2) end
 local spc=cpu:read_u32(SCRIPT+0x6e)
 local names=''
 for k,_ in pairs(held) do names=names..k:gsub('[^%w]','') end
 frames:write(string.format('%d,%x,%d,%d,%d,%d,%d,%d,%04x,%x,%x,%x,%s\n',frame,spc,section_of(spc),cpu:read_u16(0x102036),
  animation_counter:read(0),animation_disabled:read(0),fixed_source:read(0),palette_bank:read(0),cpu:read_u16(0x401ffe),
  hash(fix),hash(pal),cpu:read_u32(0x1014c4),names))
 if frame%period==0 then dump() end
 if frame%1200==0 then frames:flush();print('FRAME '..frame..string.format(' script %x section %d',spc,section_of(spc))) end
 if frame>=last then frames:close();machine:exit() end
end,'frame')
