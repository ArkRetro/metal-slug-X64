-- MAME 0.264, mslugx euro BIOS: what the game does with each soft-DIP byte
-- (docs/OPCIONES.md). Reference capture, never shipped.
--   MSX_CAPTURE_DIR  output directory
--   MSX_DIP          forced bytes, e.g. "8d=0,8e=0": RAM 10FDxx written every
--                    frame from frame 600 (the BIOS RAM test of the first
--                    seconds uses that RAM; the game starts at 700);
--                    MSX_DIP_FROM moves that first frame (the attract runs
--                    from 300, RAM 100088 = 1)
--   MSX_CAPTURE_END_FRAME  last frame (default 1500)
-- Inputs as tools/mame/capture_intro_audio.lua: coin at 700, start at 760.
-- Output:
--   reads.csv   every first read of 10FD88..10FD93 per (pc, byte) by game
--               code: frame, pc, address, value
--   mode.csv    every change of RAM 100088 (frame, value)
--   latch.csv   every 68000 write to the sound latch 320000: frame, pc, value
--   gameN.png   a snapshot every 60 frames
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local finish=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '1500')
local cpu=manager.machine.devices[':maincpu']
local space=cpu.spaces['program']
local frame=0
local from=tonumber(os.getenv('MSX_DIP_FROM') or '600')
local force={}
for k,v in (os.getenv('MSX_DIP') or ''):gmatch('(%x+)=(%x+)') do force[0x10fd00+tonumber(k,16)]=tonumber(v,16) end
local function pc() return cpu.state['CURPC'].value end
local reads=assert(io.open(output..'/reads.csv','w'));reads:write('frame,pc,address,value\n')
local modes=assert(io.open(output..'/mode.csv','w'));modes:write('frame,mode\n')
local latch=assert(io.open(output..'/latch.csv','w'));latch:write('frame,pc,value\n')
local seen={}
local function dip_read(offset,data,mask)
 local p=pc()
 if frame<300 or p>=0x100000 then return data end
 for b=0,1 do
  local m=b==0 and 0xff00 or 0x00ff
  if (mask&m)~=0 then
   local a=offset+b;local key=string.format('%06x:%06x',p,a)
   if not seen[key] then seen[key]=true
    local v=b==0 and (data>>8)&0xff or data&0xff
    reads:write(string.format('%d,%06x,%06x,%02x\n',frame,p,a,v));reads:flush() end
  end
 end
 return data
end
local function latch_write(offset,data,mask)
 latch:write(string.format('%d,%06x,%02x\n',frame,pc(),(mask&0xff00)~=0 and (data>>8)&0xff or data&0xff))
end
local fields={}
for tag,port in pairs(manager.machine.ioport.ports) do for name,f in pairs(port.fields) do fields[name]=f end end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0}}
local mode=-1
emu.register_frame_done(function()
 frame=frame+1
 -- Taps installed once the game runs (the boot's map changes drop earlier ones).
 if frame==(tonumber(os.getenv('MSX_TAP_FROM') or '300')) then
  dip_tap=space:install_read_tap(0x10fd88,0x10fd93,'msx_dip',dip_read)
  latch_tap=space:install_write_tap(0x320000,0x320001,'msx_latch',latch_write)
 end
 if frame>=from then for a,v in pairs(force) do space:write_u8(a,v) end end
 if actions[frame] and fields[actions[frame][1]] then fields[actions[frame][1]]:set_value(actions[frame][2]) end
 local m=space:read_u8(0x100088)
 if m~=mode then mode=m;modes:write(string.format('%d,%d\n',frame,m));modes:flush() end
 if frame%60==0 then manager.machine.screens[':screen']:snapshot(output..'/game'..frame..'.png') end
 if frame==finish then reads:close();modes:close();latch:close();manager.machine:exit() end
end,'frame')
