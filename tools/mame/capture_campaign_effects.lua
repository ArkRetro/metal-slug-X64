-- Controlled sound-board experiment; this is not a gameplay reference run.
-- After boot, skip the original maincpu queue write at verified ROM 01222A,
-- advancing through its original queue-read increment at 012232. Send the
-- original Z80 driver restart command03 through the real sound-latch handler,
-- then only the verified effect-token byte pairs. A matched baseline performs
-- identical resets with no effect bytes. No YM registers/voices are patched.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local baseline=os.getenv('MSX_EFFECTS_BASELINE')=='1'
local cpu=manager.machine.devices[':maincpu'].spaces.program
local dbg=assert(manager.machine.debugger)
local frames=assert(io.open(output..'/frames.jsonl','w'))
local frame=0
local effects={[600]=0x10f7,[900]=0x10fc,[1200]=0x1146,[1500]=0x10f7,
 [1800]=0x10fb,[2100]=0x10f9,[2400]=0x1069}
local resets={[300]=true,[750]=true,[1050]=true,[1350]=true,
 [1650]=true,[1950]=true,[2250]=true}
local function send(value,label)
 dbg:command(string.format('printf "EFFECT_INJECT frame=%%d value=%02X label=%s\\n",temp9',value,label))
 cpu:write_u8(0x320000,value)
end
emu.register_frame_done(function()
 frame=frame+1;dbg:command(string.format('temp9=%x',frame))
 if frame==1 then
  assert(cpu:read_u16(0x1222a)==0x13f0 and cpu:read_u16(0x12232)==0x5241)
  dbg:command('bpset 1222a,temp9>=c8,{printf "EFFECT_SUPPRESS frame=%d value=%02X\\n",temp9,b@(a0+d1);pc=12232;g}')
  dbg:command('wpset 320000:maincpu,1,w,temp9>=c8,{printf "EFFECT_GAME_LATCH frame=%d pc=%08X value=%02X\\n",temp9,pc,wpdata;g}')
  dbg:command('bpset 1ee6:audiocpu,temp9>=c8,{printf "EFFECT_DRIVER_NMI frame=%d\\n",temp9;g}')
  dbg:command('wpiset 0:audiocpu,10000,w,temp9>=12c && (wpaddr&ff)>=4 && (wpaddr&ff)<=7,{printf "EFFECT_YM frame=%d port=%04X value=%02X\\n",temp9,wpaddr,wpdata;g}')
 end
 if resets[frame] then send(3,'driver_restart') end
 if not baseline then
  if effects[frame] then send(effects[frame]>>8,'token_high') end
  if effects[frame-1] then send(effects[frame-1]&255,'token_low') end
 end
 frames:write(string.format('{"frame":%d,"time":"%s"}\n',frame,tostring(manager.machine.time)));frames:flush()
 if frame==2550 then frames:close();manager.machine:exit() end
end,'frame')
