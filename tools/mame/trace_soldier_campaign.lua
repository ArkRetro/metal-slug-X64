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
-- MSX_ASSIST_WEAPON=<hex id> (reference-only, as capture_weapons.lua's
-- MSX_WEAPONS_ASSIST_ID): from MSX_ASSIST_FROM (default MSX_RAM_FIRST)
-- every frame player 1's weapon 10C71E = id and the rounds' low byte
-- 10C725 = FF; the first frame also the cap 10C71C (ROM 0C08A8 + id) and
-- the player's +94 (ROM 0C0918 + id), as the grant 0C0928.
local assist=os.getenv('MSX_ASSIST_WEAPON') and tonumber(os.getenv('MSX_ASSIST_WEAPON'),16) or nil
local assist_from=tonumber(os.getenv('MSX_ASSIST_FROM') or tostring(first))
local assist_first=true
-- MSX_SOLDIER_ENTRY=1: the 0xB0 bytes of every soldier node at the start of
-- its update (ROM 004248 with a routine of the soldier, AE000..B0FFF, or of
-- its deaths, 040500..040FFF): ENTRY lines, so a hit applied earlier in the
-- same update by another actor (a player's flame or blast) is in the
-- evidence.
local entry=os.getenv('MSX_SOLDIER_ENTRY')=='1'
-- MSX_FORCE_DEATH=<hex kind> (reference-only RAM write, for death
-- dispatches no input reaches, e.g. the blast of weapon 02 aimed up, kind
-- 0B): every MSX_FORCE_PERIOD (default 40) frames from MSX_FORCE_FROM
-- (default 2300), the first soldier of the list (routine AE000..B0FFF, +5F
-- bit 7 clear, +3C inside 0x10..0x130) gets +5A = kind and +5F bit 7, as a
-- hit of that kind would leave it (0049E8): its next update runs the
-- original death entry of 0406EA/040750/0407F4 (FORCE lines).
local force=os.getenv('MSX_FORCE_DEATH') and tonumber(os.getenv('MSX_FORCE_DEATH'),16) or nil
local force_from=tonumber(os.getenv('MSX_FORCE_FROM') or '2300')
local force_period=tonumber(os.getenv('MSX_FORCE_PERIOD') or '40')
emu.register_frame_done(function()
 frame=frame+1
 dbg:command(string.format('temp9=%x',frame))
 if frame==first-1 then
  dbg:command('bpset 29c6,1,{printf "RNG f=%d a6=%06X ret=%06X cur=%04X",temp9,a6,d@(sp),w@(100084);g}')
  -- Level-script spawns (01A10A: a0 = record, a6 = script/runner actor) and
  -- runner creations (01A1E8), for an exact script fixture.
  dbg:command('bpset 1a10a,1,{printf "SPAWN f=%d a6=%06X rec=%06X",temp9,a6,a0;g}')
  dbg:command('bpset 1a1e8,1,{printf "RUNNER f=%d a6=%06X rec=%06X",temp9,a6,a0;g}')
  if entry then
   dbg:command('bpset 4248,((a0>=ae000)&&(a0<b1000))||((a0>=40500)&&(a0<41000)),{printf "ENTRY f=%d a6=%06X o=00 %08X %08X %08X %08X",temp9,a6,d@(a6+0),d@(a6+4),d@(a6+8),d@(a6+c);printf "ENTRY f=%d a6=%06X o=10 %08X %08X %08X %08X",temp9,a6,d@(a6+10),d@(a6+14),d@(a6+18),d@(a6+1c);printf "ENTRY f=%d a6=%06X o=20 %08X %08X %08X %08X",temp9,a6,d@(a6+20),d@(a6+24),d@(a6+28),d@(a6+2c);printf "ENTRY f=%d a6=%06X o=30 %08X %08X %08X %08X",temp9,a6,d@(a6+30),d@(a6+34),d@(a6+38),d@(a6+3c);printf "ENTRY f=%d a6=%06X o=40 %08X %08X %08X %08X",temp9,a6,d@(a6+40),d@(a6+44),d@(a6+48),d@(a6+4c);printf "ENTRY f=%d a6=%06X o=50 %08X %08X %08X %08X",temp9,a6,d@(a6+50),d@(a6+54),d@(a6+58),d@(a6+5c);printf "ENTRY f=%d a6=%06X o=60 %08X %08X %08X %08X",temp9,a6,d@(a6+60),d@(a6+64),d@(a6+68),d@(a6+6c);printf "ENTRY f=%d a6=%06X o=70 %08X %08X %08X %08X",temp9,a6,d@(a6+70),d@(a6+74),d@(a6+78),d@(a6+7c);printf "ENTRY f=%d a6=%06X o=80 %08X %08X %08X %08X",temp9,a6,d@(a6+80),d@(a6+84),d@(a6+88),d@(a6+8c);printf "ENTRY f=%d a6=%06X o=90 %08X %08X %08X %08X",temp9,a6,d@(a6+90),d@(a6+94),d@(a6+98),d@(a6+9c);printf "ENTRY f=%d a6=%06X o=A0 %08X %08X %08X %08X",temp9,a6,d@(a6+a0),d@(a6+a4),d@(a6+a8),d@(a6+ac);g}')
  end
  -- MSX_EXEC_LOG=1: every actor execution of the list traversal (ROM 004248,
  -- A6 = node, routine at dispatch) and every node allocation (004116 at the
  -- end of 0040BC: A0 = new node, A6 = creator), so the closed-loop fixture
  -- knows the exact execution order and the RNG caller positions.
  if os.getenv('MSX_EXEC_LOG')=='1' then
   dbg:command('bpset 4248,1,{printf "EXEC f=%d a6=%06X rt=%06X",temp9,a6,a0;g}')
   dbg:command('bpset 4116,1,{printf "ALLOC f=%d a0=%06X rt=%06X cr=%06X",temp9,a0,d@(a0),a6;g}')
   -- Delay-grid splats (00505A, at A6's projected +3C/+40).
   dbg:command('bpset 505a,1,{printf "SPLAT f=%d a6=%06X x=%04X y=%04X ret=%06X a2=%06X",temp9,a6,w@(a6+3c),w@(a6+40),d@(sp),a2;g}')
  end
 end
 if force and frame>=force_from and frame<=last and (frame-force_from)%force_period==0 then
  local n=0x1010a4;local seen=0
  while n~=0 and seen<200 do
   seen=seen+1
   local r=cpu:read_u32(n)
   local x=cpu:read_u16(n+0x3c)
   if r>=0xae000 and r<0xb1000 and (cpu:read_u8(n+0x5f)&0x80)==0 and x>=0x10 and x<0x130 then
    cpu:write_u8(n+0x5a,force);cpu:write_u8(n+0x5f,cpu:read_u8(n+0x5f)|0x80)
    print(string.format('FORCE %d %06x %x',frame,n,r));break
   end
   n=cpu:read_u32(n+8)
  end
 end
 if assist and frame>=assist_from and frame<=last then
  if assist_first then
   assist_first=false
   cpu:write_u8(0x10c71c,cpu:read_u8(0x0c08a8+assist));cpu:write_u8(0x1014c4+0x94,cpu:read_u8(0x0c0918+assist))
  end
  cpu:write_u8(0x10c71e,assist);cpu:write_u8(0x10c725,0xff)
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
