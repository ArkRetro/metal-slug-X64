-- MAME 0.264, mslugx (euro BIOS, MAME's default for neogeo, as BizHawk's MAME
-- core): replays a BizHawk movie converted by tools/tas_convert.py, frame by
-- frame from power-on, with NO memory writes (docs/TAS.md). Run it with
-- tools/run_tas_play.sh.
--
-- Frame convention (as tools/capture_fullgame.lua): `frame` counts the frames
-- MAME has completed (register_frame_done), taken from the emulated time so
-- that -debug (one more callback at time 0) does not shift it. The inputs of movie frame k
-- (line k of the converted file, 0-based) are set before MAME emulates the
-- frame that ends with frame == k + 1 - MSX_TAS_LAG (default 0: movie frame
-- 0 is MAME's first frame, BizHawk's FrameAdvance order "set inputs, run one
-- frame").
--
-- RTC (uPD4990A): the movie sets InitialTime 2010-01-01T00:00:00 and
-- UseRealTime false; BizHawk hands MAME that base time. MAME 0.264 has no
-- option for it, and the RTC registers are re-read from the base time at
-- every 1 s tick (writing m_time_counter from Lua does not hold), so the
-- runner fixes the wall clock MAME reads (tools/mame/fixed_time.c, TZ=UTC).
--
-- Environment:
--  MSX_TAS_INPUTS  converted movie (one hex mask per frame)   (required)
--  MSX_TAS_OUT     output directory                            (required)
--  MSX_TAS_LAG     integer frame lag (see above)               (default 0)
--  MSX_TAS_LAST    stop after this many frames (default: end of movie + 600)
--  MSX_TAS_SAVE_AT frames for a MAME state tas_<frame> (comma separated)
--  MSX_TAS_SAVE_SECTIONS=1  a state sec<NN>_<frame> at each script section start
--  MSX_TAS_LOAD / MSX_TAS_LOAD_FRAME  resume from a state saved by this script
--  MSX_TAS_DUMP_AT / MSX_TAS_SNAP_AT  frames for ram<f>.bin / game<f>.png
--  MSX_TAS_DEBUG=1 + MSX_TAS_DEBUG_CMDS  debugger commands (print only)
-- Outputs (MSX_TAS_OUT): tas.csv every frame (frame, input mask, script
-- section, camera progress 10E99E, P1 score 10C46A (BCD), lives 101288,
-- player routine 1014C4, RAM hash every 60 frames, 10F008..10F00F (vblank
-- flag/counter, protection idle counters), 10008A = vblanks taken by the
-- last main loop (2 = 30 Hz, 3/4 = slowdown; ROM 003A42), 10000E = main
-- loop counter (003B36)), events.txt (first frame and RTC, sections, lives).
local machine=manager.machine
local cpu=machine.devices[':maincpu'].spaces['program']
local out=assert(os.getenv('MSX_TAS_OUT'),'Set MSX_TAS_OUT')
local lag=tonumber(os.getenv('MSX_TAS_LAG') or '0')
-- MSX_TAS_LAG2=<frame>:<lag>: another lag from that frame on (sync research).
local lag2_from,lag2=string.match(os.getenv('MSX_TAS_LAG2') or '','(%d+):(-?%d+)')
lag2_from,lag2=tonumber(lag2_from),tonumber(lag2)
-- MSX_TAS_SHIFTS=<frame>:<d>,...: from each frame on, the inputs of movie
-- frame (frame - lag + d) (re-timing research, docs/TAS.md); ascending frames.
local shifts,shift_i,shift={},1,0
for f,d in string.gmatch(os.getenv('MSX_TAS_SHIFTS') or '','(%d+):(-?%d+)') do shifts[#shifts+1]={tonumber(f),tonumber(d)} end
local masks={}
local inputs_path=assert(os.getenv('MSX_TAS_INPUTS'),'Set MSX_TAS_INPUTS')
for line in io.lines(inputs_path) do masks[#masks+1]=tonumber(line,16) end
local last=tonumber(os.getenv('MSX_TAS_LAST') or tostring(#masks+600))
-- Fields in LogKey bit order (tools/tas_convert.py).
local names={[0]={':AUDIO_COIN','Coin 1'},{':AUDIO_COIN','Coin 2'},{':AUDIO_COIN','Service 1'},{':TEST','Service Mode'},
 {':edge:joy:START','1 Player Start'},{':edge:joy:START','2 Players Start'}}
local pads={'A','B','C','D','Down','Left','Right','Up'}
for i,b in ipairs(pads) do names[7+i]={':edge:joy:JOY1','P1 '..b};names[15+i]={':edge:joy:JOY2','P2 '..b} end
local fields={}
for bit,n in pairs(names) do
 local p=assert(machine.ioport.ports[n[1]],n[1])
 fields[bit]=assert(p.fields[n[2]],n[2])
end
local function apply(k)
 local m=(k>=0 and masks[k+1]) or 0
 for bit,f in pairs(fields) do f:set_value((m>>bit)&1) end
 return m
end
-- RTC check only: tools/run_tas_play.sh fixes MAME's wall clock (LD_PRELOAD
-- tools/mame/fixed_time.c), so the uPD4990A starts at the movie's InitialTime.
local rtc=emu.item(machine.devices[':upd4990a'].items['0/m_time_counter'])
local function rtc_text() local t='' for i=0,5 do t=t..string.format('%02x',rtc:read(i)) end return t end
local function sections_table()
 local s={}
 for i=0,29 do s[#s+1]={index=i,base=cpu:read_u32(0x800+4*i)} end
 table.sort(s,function(a,b) return a.base<b.base end)
 return s
end
local sections=nil
local function section_of(pc)
 if pc==0 then return -1 end
 local s=-1
 for _,e in ipairs(sections) do if e.base<=pc then s=e.index end end
 return s
end
os.execute('mkdir -p "'..out..'"')
local load_name=os.getenv('MSX_TAS_LOAD')
local csv=assert(io.open(out..'/tas.csv',load_name and 'a' or 'w'))
if not load_name then csv:write('frame,input,section,progress,score,lives,player,ramhash,f008,loopvb,loop\n') end
local ev=assert(io.open(out..'/events.txt',load_name and 'a' or 'w'))
local function event(s) ev:write(s..'\n');ev:flush();print(s) end
local save_at={}
for h in string.gmatch(os.getenv('MSX_TAS_SAVE_AT') or '','(%d+)') do save_at[tonumber(h)]=true end
--  MSX_TAS_DUMP_AT frames for a work RAM dump ram<frame>.bin (sync research)
--  MSX_TAS_SNAP_AT frames for a screen snapshot game<frame>.png
local dump_at,snap_at={},{}
for h in string.gmatch(os.getenv('MSX_TAS_SNAP_AT') or '','(%d+)') do snap_at[tonumber(h)]=true end
for h in string.gmatch(os.getenv('MSX_TAS_DUMP_AT') or '','(%d+)') do dump_at[tonumber(h)]=true end
local frame=0
local last_sec,last_lives=nil,nil
local function ramhash()
 local h=0
 local r=cpu:read_range(0x100000,0x10ffff,8)
 for i=1,#r,4 do h=(h*31+string.unpack('<I4',r,i))%4294967291 end
 return string.format('%08x',h)
end
apply(0-lag)
local screen=machine.screens[':screen']
-- MSX_TAS_DEBUG=1 (tools/run_tas_play.sh: -debug -debugger none): the
-- debugger commands MSX_TAS_DEBUG_CMDS ('|' separated) are set at the first
-- frame; temp9 holds the frame index (print only, as capture_fullgame.lua).
local dbg=os.getenv('MSX_TAS_DEBUG')=='1' and machine.debugger or nil
local first=true
emu.register_frame_done(function()
 -- The frame index from the emulated time (with -debug MAME calls this once
 -- more at time 0): frame k ends at k frame periods.
 local tm=machine.time
 local k=math.floor((tm.seconds*1e18+tm.attoseconds)/screen.refresh_attoseconds+0.5)
 if k==frame then return end
 frame=k
 if first and dbg then
  dbg:command(string.format('temp9=%x',frame))
  for c in string.gmatch(os.getenv('MSX_TAS_DEBUG_CMDS') or '','[^|]+') do dbg:command(c) end
 end
 if dbg then dbg:command(string.format('temp9=%x',frame)) end
 if first then first=false;event(string.format('FIRST callback frame %d time %d.%018d rtc %s',frame,machine.time.seconds,machine.time.attoseconds,rtc_text())) end
 if load_name and frame==1 then
  machine:load(load_name);frame=tonumber(os.getenv('MSX_TAS_LOAD_FRAME'))
  event(string.format('LOAD %s frame %d',load_name,frame))
 end
 if lag2_from and frame>=lag2_from then lag=lag2 end
 while shift_i<=#shifts and frame>=shifts[shift_i][1] do shift=shifts[shift_i][2];shift_i=shift_i+1 end
 local m=apply(frame-lag+shift)
 if not sections then sections=sections_table() end
 local spc=cpu:read_u32(0x101414+0x6e)
 local sec=section_of(spc)
 local score=cpu:read_u32(0x10c46a)
 local lives=cpu:read_u8(0x101288)
 local prog=cpu:read_u16(0x10e99e)
 local player=cpu:read_u32(0x1014c4)
 if sec~=last_sec then
  event(string.format('SECTION %d frame %d pc %x progress %d score %08x',sec,frame,spc,prog,score))
  if os.getenv('MSX_TAS_SAVE_SECTIONS')=='1' and sec>=0 and last_sec~=nil then machine:save(string.format('sec%02d_%d',sec,frame)) end
  last_sec=sec
 end
 if lives~=last_lives then event(string.format('LIVES %d frame %d section %d progress %d',lives,frame,sec,prog));last_lives=lives end
 if snap_at[frame] then screen:snapshot(string.format('%s/game%d.png',out,frame)) end
 if dump_at[frame] then local f=assert(io.open(string.format('%s/ram%d.bin',out,frame),'wb'));f:write(cpu:read_range(0x100000,0x10ffff,8));f:close() end
 if save_at[frame] then machine:save(string.format('tas_%d',frame));event(string.format('SAVED tas_%d',frame)) end
 csv:write(string.format('%d,%x,%d,%d,%08x,%d,%x,%s,%08x%08x,%d,%d\n',frame,m,sec,prog,score,lives,player,frame%60==0 and ramhash() or '',cpu:read_u32(0x10f008),cpu:read_u32(0x10f00c),cpu:read_u8(0x10008a),cpu:read_u16(0x10000e)))
 if frame%3000==0 then csv:flush();print(string.format('FRAME %d section %d progress %d score %08x lives %d',frame,sec,prog,score,lives)) end
 if frame>=last then csv:close();event('END '..frame);ev:close();machine:exit() end
end,'tas')
