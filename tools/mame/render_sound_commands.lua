-- Render original Neo Geo sound commands in isolation (MAME 0.264, mslugx).
-- The game is driven with inputs only to the start of Mission 1 (same
-- timeline as capture_gameplay_audio.lua), so the Z80 driver is in its
-- gameplay state. At MSX_SUSPEND_FRAME the 68000 is suspended with the
-- debugger (it can no longer queue commands; this script kicks the watchdog) and this script writes each
-- planned byte sequence to the sound latch 0x320000 itself, one byte per
-- video frame as ROM 01221A does. Every write and its emulated time goes to
-- render.jsonl; -wavwrite records the output, trimmed offline by
-- tools/render_sound_commands.py. Plan format (MSX_RENDER_PLAN):
--   label=b1,b2,...:frames;label=...    (bytes hex, frames to record)
-- Each entry is preceded by the driver reset/stop pair 03,06 and a silent gap.
-- A machine reset re-runs the autoboot script in the same Lua state: refuse
-- to start a second instance (it would reopen and truncate the log).
if msx_render_started then return end
msx_render_started=true
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local suspend_at=tonumber(os.getenv('MSX_SUSPEND_FRAME') or '3700')
local gap=tonumber(os.getenv('MSX_GAP_FRAMES') or '90')
local plan={}
for label,bytes,frames in string.gmatch(assert(os.getenv('MSX_RENDER_PLAN')),'([%w_%-]+)=([%x,]+):(%d+)') do
 local list={}
 for b in string.gmatch(bytes,'%x+') do list[#list+1]=tonumber(b,16) end
 plan[#plan+1]={label=label,bytes=list,frames=tonumber(frames)}
end
assert(#plan>0,'Empty plan')
local cpu=manager.machine.devices[':maincpu']
local space=cpu.spaces['program']
local dbg=assert(manager.machine.debugger,'Launch MAME with -debug -debugger none')
local log=assert(io.open(output..'/render.jsonl','w'))
local frame=0
local screen=manager.machine.screens[':screen']
local function button(name,value)
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value) end
  end
 end
end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[3620]={'P1 A',1},[3630]={'P1 A',0}}
-- Queue of {frame offset, byte or marker}.
local queue={};local cursor=1;local t=0
local function push(at,kind,value,label) queue[#queue+1]={at=at,kind=kind,value=value,label=label} end
for _,entry in ipairs(plan) do
 push(t,'write',0x03,entry.label);push(t+2,'write',0x06,entry.label)
 t=t+2+gap
 push(t,'start',0,entry.label)
 for i,b in ipairs(entry.bytes) do push(t+i-1,'write',b,entry.label) end
 t=t+entry.frames
 push(t,'end',0,entry.label)
end
local base=nil
emu.register_frame_done(function()
 -- Count emulated video frames: frame_done callbacks can be skipped when
 -- the host runs slowly under -nothrottle.
 local current=screen:frame_number()
 if current<=frame then return end
 local previous=frame
 frame=current
 for f=previous+1,frame do if actions[f] then button(actions[f][1],actions[f][2]) end end
 if frame>=suspend_at and not base then dbg:command('suspend 0');base=frame+1 end
 if base then
  -- The suspended 68000 no longer kicks the watchdog (0x300001): do it here,
  -- otherwise the watchdog resets the machine after a fraction of a second.
  space:write_u8(0x300001,0)
  while cursor<=#queue and frame-base>=queue[cursor].at do
   local q=queue[cursor];local now=tostring(manager.machine.time)
   if q.kind=='write' then space:write_u8(0x320000,q.value) end
   log:write(string.format('{"frame":%d,"time":"%s","kind":"%s","value":%d,"label":"%s","pc":%d}\n',frame,now,q.kind,q.value,q.label,cpu.state['rPC'].value))
   cursor=cursor+1
  end
  if cursor>#queue then log:close();manager.machine:exit() end
 end
end,'frame')
