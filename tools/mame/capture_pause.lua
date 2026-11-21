-- Capture of the home (AES) pause 015520 of mslugx in MAME 0.264 (docs/PAUSA.md).
-- MSX_OUT=dir MSX_STATE=u1020 MSX_SELECT=120-124,420-424 MSX_LAST=560 MSX_SNAP1=100 MSX_SNAP2=300 MSX_SNAP3=500 \
--   mame mslugx -bios euro -video none -sound none -nothrottle -autoboot_delay 0 -autoboot_script capture_pause.lua \
--   -state_directory /home/user/captures-tas/sync264/states -snapshot_directory dir/snap -wavwrite dir/pause.wav
-- (a state of the TAS, docs/TAS.md; no input but SELECT). Output pause.csv, one row per frame; tools/pause_fixture.py.
-- Capture-only write: RAM 10FD82 = 0 (BIOS_MVS_FLAG: home) from frame HOME on.
local machine=manager.machine
local cpu=machine.devices[':maincpu'];local mem=cpu.spaces['program']
local sprite=machine.devices[':spritegen'];local video=emu.item(assert(sprite.items['0/m_videoram']))
local out=os.getenv('MSX_OUT');local state=os.getenv('MSX_STATE')
local plan={} for a,b in string.gmatch(os.getenv('MSX_SELECT') or '','(%d+)-(%d+)') do plan[#plan+1]={tonumber(a),tonumber(b)} end
local last=tonumber(os.getenv('MSX_LAST'));local home=tonumber(os.getenv('MSX_HOME') or '10')
local sel
for tag,port in pairs(machine.ioport.ports) do for name,f in pairs(port.fields) do
  if os.getenv('MSX_LIST') then print(tag,name,string.format('%X',f.mask)) end
  end end
-- The select line of REG_STATUS_B 380000 (bit 1 of the byte, P1 SELECT, active low): the single-slot MVS driver has no field for it.
local pressed=0
msx_select_tap=mem:install_read_tap(0x380000,0x380001,'select',function(offset,data,mask) if pressed==1 then return data & ~0x0200 end end)
local log=assert(io.open(out..'/pause.csv','w'))
local frame=-1;local loaded=false
emu.register_frame_done(function()
 if not loaded then machine:load(state);loaded=true;return end
 if machine.time.seconds<1 then return end
 frame=frame+1
 if frame>=home then mem:write_u8(0x10FD82,0) end
 local v=0;for _,p in ipairs(plan) do if frame>=p[1] and frame<=p[2] then v=1 end end
 pressed=v
 local t={frame,v}
 for _,a in ipairs{0x10C2BC,0x10C2BD,0x10C2BE,0x10C2BF,0x10C2C2,0x10008D,0x10008E,0x100012,0x100013,0x10128A,0x10133A,0x10FDB6,0x10C366,0x10C367,0x10992C} do t[#t+1]=string.format('%02X',mem:read_u8(a)) end
 for a=0x10991A,0x109940,2 do t[#t+1]=string.format('%04X',mem:read_u16(a)) end
 for i=0,5 do t[#t+1]=string.format('%04X',video:read(0x725A+i*32)) end
 t[#t+1]=string.format('%04X',mem:read_u16(0x10000E))
 t[#t+1]=string.format('%04X',mem:read_u16(0x400000))
 for _,a in ipairs{0x400002,0x400022,0x401002,0x401FFE,0x400202} do t[#t+1]=string.format('%04X',mem:read_u16(a)) end
 log:write(table.concat(t,',')..'\n')
 for _,f in ipairs{tonumber(os.getenv('MSX_SNAP1') or '-1'),tonumber(os.getenv('MSX_SNAP2') or '-1'),tonumber(os.getenv('MSX_SNAP3') or '-1')} do if f==frame then
   machine.video:snapshot()
   local p=assert(io.open(out..'/palette'..frame..'.bin','wb'));for i=0,4095 do p:write(string.pack('>I2',mem:read_u16(0x400000+i*2))) end;p:close()
   local w=assert(io.open(out..'/vram'..frame..'.bin','wb'));for i=0,0x85ff do w:write(string.pack('>I2',video:read(i))) end;w:close()
 end end
 if frame>=last then log:close();machine:exit() end
end)
