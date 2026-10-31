-- Game-over capture for tools/extract_continue_fixture.py (docs/MISION1_COMPLETA.md).
-- MAME 0.264, mslugx, euro BIOS (MVS). Coin+start, then the player walks right
-- until it dies (Right released while dead). Lives 101288 forced to 0 once at frame 2000 (capture
-- only) so the next death is the game over 0C9848. MSX_GO_COIN=<frame>: insert
-- a coin at that frame and press start 60 frames later (continue variant).
-- MSX_GO_TAPB=1: B pressed 4 frames every 40 in frames 3600..4399.
-- Run: mame mslugx -bios euro -video none -sound none -nothrottle
--   -autoboot_script tools/mame/capture_gameover.lua (MSX_CAPTURE_DIR set).
local out=assert(os.getenv('MSX_CAPTURE_DIR'))
local last=tonumber(os.getenv('MSX_GO_LAST') or '9000')
local coin2=tonumber(os.getenv('MSX_GO_COIN') or '0')
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local video=emu.item(assert(manager.machine.devices[':spritegen'].items['0/m_videoram']))
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do for name,f in pairs(port.fields) do fields[name]=f end end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1700]={'P1 A',0}}
if coin2>0 then actions[coin2]={'Coin 1',1};actions[coin2+10]={'Coin 1',0};actions[coin2+60]={'1 Player Start',1};actions[coin2+70]={'1 Player Start',0} end
local log=assert(io.open(out..'/state.csv','w'))
log:write('frame,panel_routine,f84,f85,f86,f88,f89,f8c,mod1,mod2,sysmode,hud_on,join,two,dip8a,mvs,credits_d00034,mission_routine,p1_routine,video,f60,f74,ctimer,score,f80,f8d,in435d\n')
local fixlog=assert(io.open(out..'/fix.bin','wb'))
local walk=os.getenv('MSX_GO_WALK')~='0'
local tapb=os.getenv('MSX_GO_TAPB')=='1'
local lastfix=nil;local frame=0
emu.register_frame_done(function()
 frame=frame+1
 if actions[frame] then local a=actions[frame];if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame==2000 then cpu:write_u8(0x101288,0) end
 if tapb and frame>=3600 and frame<4400 and fields['P1 B'] then fields['P1 B']:set_value((frame%40)<4 and 1 or 0) end
 local dead=cpu:read_u32(0x101204)~=0x0c9644
 if fields['P1 Right'] then fields['P1 Right']:set_value((frame>=2000 and not dead and walk) and 1 or 0) end
 if frame==2002 then for _,r in ipairs({{'program-fixed.bin',0,0xfffff},{'program-bank.bin',0x200000,0x2fffff}}) do local f=assert(io.open(out..'/'..r[1],'wb'));f:write(cpu:read_range(r[2],r[3],8));f:close() end end
 if frame>=1900 then
  local P=0x101204
  log:write(string.format('%d,%06x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%06x,%06x,%04x,%02x,%04x,%04x,%s,%04x,%02x,%02x\n',frame,cpu:read_u32(P),cpu:read_u8(P+0x84),cpu:read_u8(P+0x85),cpu:read_u8(P+0x86),cpu:read_u8(P+0x88),cpu:read_u8(P+0x89),cpu:read_u8(P+0x8c),
   cpu:read_u8(0x10fdb6),cpu:read_u8(0x10fdb7),cpu:read_u8(0x10fd80),cpu:read_u8(0x10e9e1),cpu:read_u8(0x10f003),cpu:read_u8(0x10c2e7),cpu:read_u8(0x10fd8a),cpu:read_u8(0x10fd82),cpu:read_u8(0xd00034),cpu:read_u32(0x101364),cpu:read_u32(0x1014c4),cpu:read_u16(0x10000e),cpu:read_u8(P+0x60),cpu:read_u16(P+0x74),cpu:read_u16(0x1013e2),string.format('%08x',cpu:read_u32(0x10c46a)),cpu:read_u16(P+0x80),cpu:read_u8(P+0x8d),cpu:read_u8(0x10435d)))
  local t={};for i=0x7000,0x74ff do t[#t+1]=string.pack('>I2',video:read(i)) end
  local s=table.concat(t)
  if s~=lastfix then fixlog:write(string.pack('>I4',frame));fixlog:write(s);lastfix=s
   if frame%2==0 then manager.machine.screens[':screen']:snapshot(out..'/s'..frame..'.png') end
  end
 end
 if frame>=last then log:close();fixlog:close();manager.machine:exit() end
end)
