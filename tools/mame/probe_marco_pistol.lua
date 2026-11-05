-- Read-only dynamic provenance for C4372/C4244 and original pistol actors.
-- MSX_CAPTURE_DIR owns output; MSX_PISTOL_PLAN is a scheduled joypad-only plan.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local plan=dofile(assert(os.getenv('MSX_PISTOL_PLAN')))
local cpu=manager.machine.devices[':maincpu'].spaces.program
local dbg=assert(manager.machine.debugger)
local frame=0
local function button(name,value)
 local found=false
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value);found=true end
  end
 end
 assert(found,'Unknown controller input '..name)
end
local function probe(site,label,condition,fmt,args)
 dbg:command(string.format('bpset %x,temp9>=%x && (%s),{printf "%s frame=%%d %s\\n",temp9,%s;g}',site,plan.first,condition,label,fmt,args))
end
probe(0xc4372,'PISTOL_EVENT','a6==1014c4',
 'actor=%08X x=%08X y=%08X desired=%02X aim=%02X form=%02X attr=%04X offsets=%08X kind=%02X',
 'a6,d@(a6+28),d@(a6+2c),b@(a6+90),b@(a6+92),b@(a6+93),w@(a6+14),d@(a6+72),b@(a5+471e+w@(a6+70))')
probe(0xc42a8,'PISTOL_MUZZLE','a6==1014c4 && d@(sp)==c443e',
 'dx=%04X dy=%04X angle=%02X','d2&ffff,d3&ffff,d7&ff')
probe(0x1b7e6,'PISTOL_VECTOR','1',
 'actor=%08X x=%08X y=%08X vx=%08X vy=%08X angle=%02X',
 'a6,d@(a6+28),d@(a6+2c),d0,d1,b@(a6+6e)')
probe(0x1b882,'PISTOL_STEP','1',
 'actor=%08X x=%08X y=%08X vx=%08X vy=%08X angle=%02X',
 'a6,d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),b@(a6+6e)')
probe(0x120f8,'PISTOL_SOUND','d@(sp)==1b78c',
 'actor=%08X token=%04X','a6,d0&ffff')
probe(0x4406,'PISTOL_DRAW','d@(a6)==1b882',
 'actor=%08X angle=%02X descriptor=%08X xy=%08X attr=%04X',
 'a6,b@(a6+6e),d7,d0,d1&ffff')
local boot={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1701]={'P1 A',0}}
emu.register_frame_done(function()
 frame=frame+1;dbg:command(string.format('temp9=%x',frame))
 if boot[frame] then button(boot[frame][1],boot[frame][2]) end
 for _,event in ipairs(plan.events[frame] or {}) do button(event[1],event[2]) end
 if frame==plan.first then
  local f=assert(io.open(output..'/program-fixed.bin','wb'));f:write(cpu:read_range(0,0xfffff,8));f:close()
 end
 if frame==plan.last then manager.machine:exit() end
end,'frame')
