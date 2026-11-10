-- mslugx (euro BIOS), MAME 0.251 and 0.264: the TAS keyed by GAME UPDATE
-- instead of by video frame (docs/TAS.md). NO memory writes. Run it with
-- MSX_TAS_SCRIPT=tas_updates.lua tools/run_tas_play.sh OUT.
--
-- The game's main loop reads the input ports itself once per update (ROM
-- 03CB3A..03CC60: 380000, 300000, 340000) and then counts the update
-- (003B36, RAM 10000E). A write tap on 10000E numbers the updates from
-- power-on (U = 1, 2, ...; the three writes of the BIOS RAM test included)
-- and read taps on the ports note which movie frame each update read.
--
--  record (MSX_TAS_UPD_REC=<file>, MAME 0.251, where the movie syncs): the
--   movie is played by video frame as tools/tas_play.lua (lag 0) and each
--   update writes "U frame mask section progress score lives rng state":
--   frame = the frame being emulated, mask = the movie mask the game read,
--   rng/state = hashes of the RNG (100016..100085, 10F006) and of the
--   work RAM 1010A0..10EFFF at the tap (state only every MSX_TAS_UPD_HASH
--   updates, default 1). 10009A..10109F is left out: it is the queue of
--   deferred jobs of 0036A4, which saves D0-D7/A0-A5 with each job, and the
--   dead register values differ between the two MAME versions (101060).
--   10BFE8..10BFF3 is left out too: the read index and the Z80 reply of the
--   sound command ring 10BF68 (0121DA.., one command per vblank).
--  replay (MSX_TAS_UPD_PLAY=<file>, MAME 0.264): by video frame until the
--   update MSX_TAS_UPD_FROM (default: the first update of script section 0)
--   has been counted; from then on every frame boundary sets the mask the
--   record has for the NEXT update (U + 1), so each update reads the same
--   input however many vblanks it takes. The same log is written to
--   MSX_TAS_OUT/updates.txt and compared with the record: the first
--   differences are reported as DESYNC in events.txt (the first 20; the END
--   line has the count) and MSX_TAS_UPD_STOP=1 stops at the first.
--  MSX_TAS_UPD_SAVE=<name>:<U>,...  a MAME state <name> at the first frame
--   boundary after update U (the state directory of run_tas_play.sh);
--   MSX_TAS_UPD_DUMP=<U>,... the work RAM ram_u<U>.bin taken at the tap.
--   MSX_TAS_UPD_SAVE_SECTIONS=1: a state sec<NN>_u<U> at each section start.
--   MSX_TAS_UPD_SAVE_EVERY=<N>: a state u<U> every N updates (U % N == 0).
--   MSX_TAS_UPD_SNAP_EVERY=<N> / MSX_TAS_UPD_SNAP=<U>,...: a screen snapshot
--   snap_u<U>.png at the same kind of point. events.txt lists every state
--   with its update, frame, section, progress, score and lives.
--  MSX_TAS_UPD_LOAD=<state>:<U> (replay): resume from a state saved by this
--   script (the update U of its SAVED line in events.txt) and go on by update.
--  MSX_TAS_LAST: stop after this many frames; MSX_TAS_UPD_LAST: after this update.
local machine=manager.machine
local dev=machine.devices[':maincpu']
local cpu=dev.spaces['program']
local screen=machine.screens[':screen']
local out=assert(os.getenv('MSX_TAS_OUT'),'Set MSX_TAS_OUT')
local rec_path=os.getenv('MSX_TAS_UPD_REC')
local play_path=os.getenv('MSX_TAS_UPD_PLAY')
local hash_every=tonumber(os.getenv('MSX_TAS_UPD_HASH') or '1')
local masks={}
for line in io.lines((assert(os.getenv('MSX_TAS_INPUTS'),'Set MSX_TAS_INPUTS'))) do masks[#masks+1]=tonumber(line,16) end
local last=tonumber(os.getenv('MSX_TAS_LAST') or tostring(#masks+600))
local last_upd=tonumber(os.getenv('MSX_TAS_UPD_LAST') or '0')
-- The record: per update the mask and the line to compare.
local rec_mask,rec_line,from={},{},tonumber(os.getenv('MSX_TAS_UPD_FROM') or '0')
if play_path then
 for line in io.lines(play_path) do
  local u,_,mask,sec,rest=string.match(line,'^(%d+) (%d+) (%x+) (-?%d+) (.*)$')
  if u then
   u=tonumber(u);rec_mask[u]=tonumber(mask,16);rec_line[u]=sec..' '..rest
   if from==0 and tonumber(sec)==0 then from=u end
  end
 end
end
local names={[0]={':AUDIO_COIN','Coin 1'},{':AUDIO_COIN','Coin 2'},{':AUDIO_COIN','Service 1'},{':TEST','Service Mode'},
 {':edge:joy:START','1 Player Start'},{':edge:joy:START','2 Players Start'}}
local pads={'A','B','C','D','Down','Left','Right','Up'}
for i,b in ipairs(pads) do names[7+i]={':edge:joy:JOY1','P1 '..b};names[15+i]={':edge:joy:JOY2','P2 '..b} end
local fields={}
for bit,n in pairs(names) do
 local p=assert(machine.ioport.ports[n[1]],n[1])
 fields[bit]=assert(p.fields[n[2]],n[2])
end
local applied=0
local function apply(m)
 m=m or 0
 for bit,f in pairs(fields) do f:set_value((m>>bit)&1) end
 applied=m
end
os.execute('mkdir -p "'..out..'"')
local log=assert(io.open(rec_path or (out..'/updates.txt'),'w'))
local ev=assert(io.open(out..'/events.txt','w'))
local function event(s) ev:write(s..'\n');ev:flush();print(s) end
local function list(name)
 local t={}
 for h in string.gmatch(os.getenv(name) or '','(%d+)') do t[tonumber(h)]=true end
 return t
end
local dump_at=list('MSX_TAS_UPD_DUMP')
local save_at={}
for n,u in string.gmatch(os.getenv('MSX_TAS_UPD_SAVE') or '','([%w_]+):(%d+)') do save_at[tonumber(u)]=n end
local save_sections=os.getenv('MSX_TAS_UPD_SAVE_SECTIONS')=='1'
local save_every=tonumber(os.getenv('MSX_TAS_UPD_SAVE_EVERY') or '0')
local snap_every=tonumber(os.getenv('MSX_TAS_UPD_SNAP_EVERY') or '0')
local snap_at=list('MSX_TAS_UPD_SNAP')
local snaps={}
local stop_on_desync=os.getenv('MSX_TAS_UPD_STOP')=='1'
local sections=nil
local function section_of(pc)
 if pc==0 then return -1 end
 if not sections then
  sections={}
  for i=0,29 do sections[#sections+1]={index=i,base=cpu:read_u32(0x800+4*i)} end
  table.sort(sections,function(a,b) return a.base<b.base end)
 end
 local s=-1
 for _,e in ipairs(sections) do if e.base<=pc then s=e.index end end
 return s
end
local function hash(r,a,b)
 local h=0
 for i=a,b,4 do h=(h*31+string.unpack('>I4',r,i))%4294967291 end
 return h
end
local frame=0          -- frames completed (emulated time / frame period)
local updates=0        -- writes to 10000E so far
local read_mask=0      -- the mask in effect at the game's own P1 read of this update
local read_frame=-1
local pending={}       -- states to save at the next frame boundary
local last_sec=nil
local desync=nil
local info=''
local differ=0
local load_name,load_u=string.match(os.getenv('MSX_TAS_UPD_LOAD') or '','([%w_]+):(%d+)')
local loading=false
local done=false
local function on_update()
 -- A state is being loaded: the taps before it (emulated time under 1 s) do not count.
 if loading then if machine.time.seconds<1 then return end loading=false end
 updates=updates+1
 local u=updates
 local r=cpu:read_range(0x100000,0x10efff,8)
 local sec=section_of(cpu:read_u32(0x101414+0x6e))
 local rng=(hash(r,0x15,0x84)+string.unpack('>I2',r,0x85)*65537+cpu:read_u16(0x10f006)*257)%4294967291
 local line=string.format('%d %d %08x %d %x %08x %s',sec,string.unpack('>I2',r,0xe99f),string.unpack('>I4',r,0xc46b),
  string.unpack('>B',r,0x1289),string.unpack('>I4',r,0x14c5),rng,(u%hash_every==0) and string.format('%08x',(hash(r,0x10a1,0xbfe5)*31+hash(r,0xbff5,0xeffd))%4294967291) or '-')
 log:write(string.format('%d %d %x %s\n',u,frame+1,read_mask,line))
 if dump_at[u] then local f=assert(io.open(string.format('%s/ram_u%d.bin',out,u),'wb'));f:write(r);f:close() end
 if sec~=last_sec then
  event(string.format('SECTION %d update %d frame %d progress %d score %08x',sec,u,frame+1,string.unpack('>I2',r,0xe99f),string.unpack('>I4',r,0xc46b)))
  if save_sections and sec>=0 and u>=from and from>0 then pending[#pending+1]=string.format('sec%02d_u%d',sec,u) end
  last_sec=sec
 end
 if save_at[u] then pending[#pending+1]=save_at[u] end
 if save_every>0 and u%save_every==0 and sec>=0 then pending[#pending+1]=string.format('u%d',u) end
 if snap_at[u] or (snap_every>0 and u%snap_every==0 and sec>=0) then snaps[#snaps+1]=u end
 info=string.format('section %d progress %d score %08x lives %d',sec,string.unpack('>I2',r,0xe99f),string.unpack('>I4',r,0xc46b),string.unpack('>B',r,0x1289))
 if play_path and u>=from and rec_line[u] and rec_line[u]~=line and not (string.sub(line,-1)=='-' or string.sub(rec_line[u],-1)=='-') then
  desync=desync or u
  differ=differ+1
  if differ<=20 then event(string.format('DESYNC update %d frame %d: record "%s" here "%s"',u,frame+1,rec_line[u],line)) end
 end
end
-- Tap callbacks swallow Lua errors: report them.
local function guarded(f) return function(...) local ok,e=pcall(f,...) if not ok then print('TAP ERROR '..tostring(e)) end end end
tas_taps={
 cpu:install_write_tap(0x10000e,0x10000f,'tas_update',guarded(on_update)),
 -- The game's own read of the P1 port (not the BIOS, C00000..): 300000 high byte.
 cpu:install_read_tap(0x300000,0x300001,'tas_p1',guarded(function(o,d,m)
  if m&0xff00~=0 and dev.state['CURPC'].value<0xc00000 then read_mask=applied;read_frame=frame+1 end
 end)),
}
-- Read-only capture taps for a section of the movie (docs/MISION5.md), in the formats of
-- tools/capture_fullgame.lua so the same extractors read them; `frame` = frames completed, as there:
--  MSX_TAS_CAMERA=1     camera.csv: one line per camera update (the last store of ROM 0C7CA6, RAM 10E9D8):
--                       players, mission actor, flags, the block 10E94E.., the four layer objects +0..+2B.
--  MSX_TAS_CELLS=1      cells.txt: every collision cell the kernel reads with 0C79FE (PC 0C7A18) from a layer
--                       ring, aggregated (a0, x, y, value, first and last frame, reads, player reads), and
--                       events.csv: every write to the rings 10C7B6.. with its PC.
--  MSX_TAS_CAPTURE_FROM=<U>: the taps count from that update on (the section's first; default 0).
local cap_from=tonumber(os.getenv('MSX_TAS_CAPTURE_FROM') or '0')
local mstate=dev.state
local function hex(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
if os.getenv('MSX_TAS_CAMERA')=='1' then
 local camera_csv=assert(io.open(out..'/camera.csv','w'))
 camera_csv:write('frame,p1,p2,mission,flags,block,d8,layers\n')
 local function player(b) return string.format('%08x:%s:%02x%02x:%02x%02x',cpu:read_u32(b),hex(b+0x3c,8),cpu:read_u8(b+0x7a),cpu:read_u8(b+0x7b),cpu:read_u8(b+0x5f),cpu:read_u8(b+0x15)) end
 tas_taps[#tas_taps+1]=cpu:install_write_tap(0x10e9d8,0x10e9d9,'tas_camera',guarded(function(address,data,mask)
  if updates<cap_from then return end
  camera_csv:write(string.format('%d,%s,%s,%08x:%s,%02x%02x%02x,%s,%04x,%s:%s:%s:%s\n',frame,player(0x1014c4),player(0x101574),
   cpu:read_u32(0x101364),hex(0x101364+0x72,8),cpu:read_u8(0x10c322),cpu:read_u8(0x10f000),cpu:read_u8(0x10c300),hex(0x10e94e,0x8e),data&0xffff,
   hex(0x10c72e,0x2c),hex(0x10d7b6,0x2c),hex(0x10e83e,0x2c),hex(0x10e8c6,0x2c)))
  if frame%600==0 then camera_csv:flush() end
 end))
 tas_camera_csv=camera_csv
end
local cell_seen,cell_order=nil,{}
if os.getenv('MSX_TAS_CELLS')=='1' then
 cell_seen={}
 local events=assert(io.open(out..'/events.csv','w'))
 events:write('frame,kind,address,old,new,pc,camera\n')
 tas_taps[#tas_taps+1]=cpu:install_write_tap(0x10c7b6,0x10e83d,'tas_grid',guarded(function(address,data,mask)
  if updates<cap_from or (address>=0x10d7b6 and address<0x10d83e) then return end
  local pc=(mstate['PC'] or mstate['rPC']).value   -- as tools/capture_fullgame.lua msx_pc: the extractors know the writers by this value
  for lane=0,1 do
   local shift=lane==0 and 8 or 0
   if (mask&(0xff<<shift))~=0 then
    local a=address+lane
    events:write(string.format('%d,grid,%x,%d,%d,%x,%d\n',frame,a,cpu:read_u8(a),(data>>shift)&255,pc,cpu:read_u16(0x10e99e)))
   end
  end
 end))
 tas_taps[#tas_taps+1]=cpu:install_read_tap(0x10c7b6,0x10e83d,'tas_cell',guarded(function(address,data,mask)
  if updates<cap_from or mstate['CURPC'].value~=0xc7a18 then return end
  local a0=mstate['A0'].value
  local v=(address&1)==1 and (data&0xff) or ((data>>8)&0xff)
  if mask==0x00ff then v=data&0xff elseif mask==0xff00 then v=(data>>8)&0xff end
  local key=string.format('%x %d %d %d',a0,mstate['D0'].value&0xffff,mstate['D1'].value&0xffff,v)
  local e=cell_seen[key]
  if not e then e={frame,frame,0,0};cell_seen[key]=e;cell_order[#cell_order+1]=key end
  e[2]=frame;e[3]=e[3]+1
  if mstate['A6'].value==0x1014c4 then e[4]=e[4]+1 end
 end))
 tas_events_csv=events
end
local function capture_close()
 if tas_camera_csv then tas_camera_csv:close() end
 if tas_events_csv then tas_events_csv:close() end
 if cell_seen then
  local o=assert(io.open(out..'/cells.txt','w'))
  for _,k in ipairs(cell_order) do local e=cell_seen[k];o:write(string.format('C %s %d %d %d %d\n',k,e[1],e[2],e[3],e[4])) end
  o:close()
 end
end
apply(masks[1])
local first=true
emu.register_frame_done(function()
 local tm=machine.time
 local k=math.floor((tm.seconds*1e18+tm.attoseconds)/screen.refresh_attoseconds+0.5)
 if k==frame or done then return end
 frame=k
 if first then
  first=false;event(string.format('FIRST callback frame %d updates %d from %d',frame,updates,from))
  if load_name then machine:load(load_name);updates=tonumber(load_u);loading=true;event(string.format('LOAD %s update %d',load_name,updates)) end
 end
 -- MAME keeps one scheduled save: one state per frame boundary.
 if #pending>0 then local n=table.remove(pending,1);machine:save(n);event(string.format('SAVED %s update %d frame %d %s',n,updates,frame,info)) end
 for _,u in ipairs(snaps) do screen:snapshot(string.format('%s/snap_u%d.png',out,u)) end
 snaps={}
 if play_path and updates>=from then apply(rec_mask[updates+1]) else apply(masks[frame+1]) end
 if frame%3000==0 then log:flush();print(string.format('FRAME %d update %d',frame,updates)) end
 if frame>=last or (last_upd>0 and updates>=last_upd) or (desync and stop_on_desync) then
  done=true;log:close();capture_close();event(string.format('END frame %d update %d first desync %s, %d updates differ',frame,updates,tostring(desync),differ));ev:close();machine:exit()
 end
end,'tas')
