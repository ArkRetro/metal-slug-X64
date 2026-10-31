-- Read-only sprite/RAM observations. Only scheduled controller input is changed.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local plan=dofile(output..'/plan.lua')
local cpu=manager.machine.devices[':maincpu'].spaces.program
local sprite=manager.machine.devices[':spritegen']
local video=emu.item(assert(sprite.items['0/m_videoram']))
local dbg=assert(manager.machine.debugger)
local frame=0
local dump_until=nil
local inputs=assert(io.open(output..'/inputs.jsonl','w'))
local states=assert(io.open(output..'/states.jsonl','w'))
local function button(name,value)
 local found=false
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value);found=true end
  end
 end
 assert(found,'Unknown controller input '..name)
 inputs:write(string.format('{"frame":%d,"button":"%s","value":%d}\n',frame,name,value));inputs:flush()
end
-- The actor address is verified by the existing entry/air traces. This probe
-- does not assume its generic helper field meanings apply before observing them.
local function probe(site,label,format,args,actions)
 dbg:command(string.format('bpset %x,temp9>=%x && a6==1014c4,{printf "%s frame=%%d %s\\n",temp9,%s;'..(actions or '')..'g}',site,plan.first,label,format,args))
end
if os.getenv('MSX_AMMO_TRACE')=='1' then
 local fmt='slot=%04X request=%02X ammo=%04X live=%02X cap=%02X kind=%02X cheat=%02X special=%02X sr=%04X base=%08X'
 local args='(d0&1),(d1&ff),w@(a5+4724+((d0&1)*2)),b@(a5+471a+(d0&1)),b@(a5+471c+(d0&1)),b@(a5+471e+(d0&1)),b@10f001,b@10c2e7,sr,a5'
 probe(0xc0d64,'MARCO_AMMO_IN',fmt,args)
 for _,pc in ipairs({0xc0d9c,0xc0dc4,0xc0dda,0xc0ddc,0xc0de2}) do
  probe(pc,'MARCO_AMMO_OUT',fmt,args)
 end
end
if os.getenv('MSX_COMMAND_TRACE')=='1' then
 probe(0x429c,'MARCO_COMMAND',
  'cursor=%08X state=%08X timer=%04X speed=%04X flags=%04X f17=%02X correction=%02X f50=%08X f6c=%02X f72=%08X f76=%02X f77=%02X f78=%02X f80=%02X f81=%02X f90=%02X f91=%02X f94=%02X f98=%02X f9a=%02X',
  'a1,a0,w@(a0+4),w@(a0+6),w@(a6+14),b@(a6+17),b@(a6+4d),d@(a6+50),b@(a6+6c),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+80),b@(a6+81),b@(a6+90),b@(a6+91),b@(a6+94),b@(a6+98),b@(a6+9a)')
end
if os.getenv('MSX_QUEUE_TRACE')=='1' then
 dbg:command('temp6=0')
 dbg:command('bpset 11e2c,temp9>='..string.format('%x',plan.first)..' && temp6==0,{temp6=1;trace '..output..'/publication-step.log,maincpu,noloop,{tracelog "frame=%d A5=%08X flag=%02X ",temp9,a5,b@(a5+1946)};g}')
 dbg:command('bpset 11e46,temp6==1,{trace off,maincpu;temp6=2;g}')
 probe(0xa03e,'MARCO_QUEUE_SUBMIT','a5=%08X flag=%02X phase=%02X descriptor=%08X xy=%08X attr=%04X z=%04X callback=%08X',
  'a5,b@(a5+1946),b@(10000f),d7,d0,d1&ffff,d5&ffff,d@(a5+3f64)')
end
if os.getenv('MSX_UPPER_TRACE')=='1' then
 local fmt='mode=%08X desired=%02X active=%02X aim=%02X form=%02X combo=%02X idle=%02X action=%02X f94=%02X f96=%02X f99=%02X f5f=%02X f9a=%02X f9b=%02X f9c=%02X character=%04X player=%04X kind=%02X bombs=%02X unlimited=%d packed=%08X input_latch=%04X gate_mode=%02X gate_current=%02X gate_limit=%02X cursor=%08X timer=%04X speed=%04X flash=%02X f18=%02X field50=%08X f72=%08X f76=%02X f77=%02X f78=%02X f79=%02X x=%04X y=%04X cached_x=%04X cached_y=%04X attr=%04X z=%04X correction=%02X flash_palette=%02X palettes=%08X phase=%02X'
 local args='d@(a6+7c),b@(a6+90),b@(a6+91),b@(a6+92),b@(a6+93),b@(a6+95),b@(a6+97),b@(a6+98),b@(a6+94),b@(a6+96),b@(a6+99),b@(a6+5f),b@(a6+9a),b@(a6+9b),b@(a6+9c),w@(a6+6e),w@(a6+70),b@(a5+471e+w@(a6+70)),b@(a5+4720+w@(a6+70)),(b@(10f001)&4)!=0,d@(a5+69ec),w@(a5+69f0),b@(a5+4718+w@(a6+70)),b@(a5+471a+w@(a6+70)),b@(a5+471c+w@(a6+70)),d@(a6+88),w@(a6+8c),w@(a6+8e),b@(a6+17),b@(a6+18),d@(a6+50),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+79),w@(a6+3c),w@(a6+40),w@(a6+44),w@(a6+48),w@(a6+14),w@(a6+1e),b@(a6+4d),b@(a6+16),d@(a6+19),b@(10000f)'
 probe(0xc330c,'MARCO_UPPER_IN',fmt,args)
 probe(0xc330e,'MARCO_UPPER_OUT',fmt,args)
 probe(0xc4b1a,'MARCO_UPPER_IN',fmt,args)
 probe(0xc4b1c,'MARCO_UPPER_OUT',fmt,args)
 probe(0xc0f16,'MARCO_UPPER_RESET_IN',fmt,args)
 probe(0xc0f30,'MARCO_UPPER_RESET_OUT',fmt,args)
 -- Full RNG return values are services of the global world schedule, not
 -- values deduced from an animation output or a placeholder RNG.
 dbg:command('bpset 29c6,temp9>='..string.format('%x',plan.first)..',{printf "MARCO_RANDOM_CALL frame=%d caller=%08X a6=%08X\\n",temp9,d@(sp),a6;g}')
 probe(0xc3aea,'MARCO_UPPER_RANDOM','value=%04X','d0&ffff')
 probe(0xc488a,'MARCO_UPPER_RANDOM','value=%04X','d0&ffff')
 probe(0xc3bb2,'MARCO_UPPER_LATCH','word=%04X','w@(a0+4)')
 probe(0xc3de8,'MARCO_UPPER_LATCH','word=%04X','w@(a0+4)')
 for _,pc in ipairs({0xc4428,0xc4474,0xc449c,0xc44d8,0xc44f6}) do
  probe(pc,'MARCO_UPPER_SPAWN_RESULT','site=%08X success=%d',string.format('%x,sr&1',pc))
 end

 probe(0xc0f96,'MARCO_AVAILABLE_IN','table=%08X form=%02X character=%04X','a0,b@(a6+93),w@(a6+6e)')
 probe(0xc0fb4,'MARCO_AVAILABLE_OUT','carry=%d','sr&1')
 probe(0xc0fce,'MARCO_DIRECT_PROGRAM_IN','table=%08X character=%04X','a0,w@(a6+6e)')
 probe(0xc0fda,'MARCO_DIRECT_PROGRAM_CHOICE','program=%08X','a1')
 probe(0xc0fe8,'MARCO_DIRECT_PROGRAM_OUT','cursor=%08X timer=%04X speed=%04X','d@(a6+88),w@(a6+8c),w@(a6+8e)')

end
if os.getenv('MSX_BODY_TRACE')=='1' then
 -- Isolate complete crouch-controller blocks. External tail calls are explicit
 -- boundaries; no unobserved idle/air/world routine is treated as implemented.
 dbg:command('temp5=0')
 local fmt='mode=%08X attr=%04X f5f=%02X f6c=%02X character=%04X f80=%02X f81=%02X f90=%02X f91=%02X f92=%02X f93=%02X f95=%02X f97=%02X f98=%02X f9a=%02X f9b=%02X f9c=%02X f9d=%02X motion=%08X x=%08X y=%08X vx=%08X vy=%08X body_cursor=%08X body_timer=%04X body_speed=%04X upper_mode=%08X upper_cursor=%08X upper_timer=%04X upper_speed=%04X f17=%02X f18=%02X field50=%08X f72=%08X f76=%02X f77=%02X f78=%02X f79=%02X kind=%02X packed=%08X gate_mode=%02X gate_current=%02X gate_limit=%02X'
 local args='d@(a6),w@(a6+14),b@(a6+5f),b@(a6+6c),w@(a6+6e),b@(a6+80),b@(a6+81),b@(a6+90),b@(a6+91),b@(a6+92),b@(a6+93),b@(a6+95),b@(a6+97),b@(a6+98),b@(a6+9a),b@(a6+9b),b@(a6+9c),b@(a6+9d),d@(a6+62),d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+20),w@(a6+24),w@(a6+26),d@(a6+7c),d@(a6+88),w@(a6+8c),w@(a6+8e),b@(a6+17),b@(a6+18),d@(a6+50),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+79),b@(a5+471e+w@(a6+70)),d@(a5+69ec),b@(a5+4718+w@(a6+70)),b@(a5+471a+w@(a6+70)),b@(a5+471c+w@(a6+70))'
 for _,pc in ipairs({0xc2214,0xc2236,0xc2294,0xc229c,0xc22a4,0xc22b6,0xc2338,0xc2368,0xc23ca,0xc23f2,0xc2458,0xc246a,0xc2588}) do
  dbg:command(string.format('bpset %x,temp9>=%x && a6==1014c4 && temp5==0,{temp5=1;printf "MARCO_BODY_IN frame=%%d entry=%%08X %s\\n",temp9,%x,%s;g}',pc,plan.first,fmt,pc,args))
 end
 for _,pc in ipairs({0xc32f4,0xc1e34,0xc2654,0xc2706,0xc271c}) do
  dbg:command(string.format('bpset %x,temp5==1 && a6==1014c4,{printf "MARCO_BODY_OUT frame=%%d exit=%%08X %s\\n",temp9,%x,%s;temp5=0;g}',pc,fmt,pc,args))
 end
 for _,p in ipairs({{0x14594,'MOVE_IN'},{0xc226c,'MOVE_OUT'},{0xc230c,'MOVE_OUT'},{0xc2396,'MOVE_OUT'},{0xc2444,'MOVE_OUT'},{0xc25be,'MOVE_OUT'}}) do
  dbg:command(string.format('bpset %x,temp5==1 && a6==1014c4,{printf "MARCO_BODY_%s frame=%%d %s\\n",temp9,%s;g}',p[1],p[2],fmt,args))
 end
end
if os.getenv('MSX_MOTION_TRACE')=='1' then
 -- Isolate complete movement-controller blocks. External tail calls are explicit
 -- boundaries; no unobserved idle/air/world routine is treated as implemented.
 dbg:command('temp5=0')
 local fmt='mode=%08X attr=%04X f5f=%02X f6c=%02X character=%04X f80=%02X f81=%02X f90=%02X f91=%02X f92=%02X f93=%02X f95=%02X f97=%02X f98=%02X f9a=%02X f9b=%02X f9c=%02X f9d=%02X motion=%08X x=%08X y=%08X vx=%08X vy=%08X body_cursor=%08X body_timer=%04X body_speed=%04X upper_mode=%08X upper_cursor=%08X upper_timer=%04X upper_speed=%04X f17=%02X f18=%02X field50=%08X f72=%08X f76=%02X f77=%02X f78=%02X f79=%02X kind=%02X packed=%08X gate_mode=%02X gate_current=%02X gate_limit=%02X f7a=%02X f86=%02X'
 local args='d@(a6),w@(a6+14),b@(a6+5f),b@(a6+6c),w@(a6+6e),b@(a6+80),b@(a6+81),b@(a6+90),b@(a6+91),b@(a6+92),b@(a6+93),b@(a6+95),b@(a6+97),b@(a6+98),b@(a6+9a),b@(a6+9b),b@(a6+9c),b@(a6+9d),d@(a6+62),d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+20),w@(a6+24),w@(a6+26),d@(a6+7c),d@(a6+88),w@(a6+8c),w@(a6+8e),b@(a6+17),b@(a6+18),d@(a6+50),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+79),b@(a5+471e+w@(a6+70)),d@(a5+69ec),b@(a5+4718+w@(a6+70)),b@(a5+471a+w@(a6+70)),b@(a5+471c+w@(a6+70)),b@(a6+7a),b@(a6+86)'
 for _,pc in ipairs({0xc2642,0xc2654,0xc2692,0xc2706,0xc271c,0xc2746,0xc27a8,0xc27e8,0xc27fa,0xc285c,0xc2888}) do
  dbg:command(string.format('bpset %x,temp9>=%x && a6==1014c4 && temp5==0,{temp5=1;printf "MARCO_MOTION_IN frame=%%d entry=%%08X %s\\n",temp9,%x,%s;g}',pc,plan.first,fmt,pc,args))
 end
 for _,pc in ipairs({0xc32f4,0xc2214,0xc1f4a,0xc1efa,0xc215e}) do
  dbg:command(string.format('bpset %x,temp5==1 && a6==1014c4,{printf "MARCO_MOTION_OUT frame=%%d exit=%%08X %s\\n",temp9,%x,%s;temp5=0;g}',pc,fmt,pc,args))
 end
 for _,p in ipairs({{0x14594,'MOVE_IN'},{0xc27d4,'MOVE_OUT'},{0xc2852,'MOVE_OUT'},{0xc2944,'MOVE_OUT'},{0xc109c,'AIR_IN'},{0xc26e2,'AIR_OUT'},{0x143ee,'FALL_IN'},{0xc2714,'FALL_OUT'}}) do
  dbg:command(string.format('bpset %x,temp5==1 && a6==1014c4,{printf "MARCO_MOTION_%s frame=%%d %s\\n",temp9,%s;g}',p[1],p[2],fmt,args))
 end
end
if os.getenv('MSX_IDLE_TRACE')=='1' then
 dbg:command('temp5=0')
 local fmt='attr=%04X f5f=%02X f6c=%02X f9a=%02X f9b=%02X f9c=%02X f9d=%02X packed=%08X gate_mode=%02X gate_current=%02X gate_limit=%02X'
 local args='w@(a6+14),b@(a6+5f),b@(a6+6c),b@(a6+9a),b@(a6+9b),b@(a6+9c),b@(a6+9d),d@(a5+69ec),b@(a5+4718+w@(a6+70)),b@(a5+471a+w@(a6+70)),b@(a5+471c+w@(a6+70))'
 dbg:command(string.format('bpset c1fa6,temp9>=%x && a6==1014c4 && temp5==0,{temp5=1;printf "MARCO_IDLE_IN frame=%%d %s\\n",temp9,%s;g}',plan.first,fmt,args))
 for _,pc in ipairs({0xc271c,0xc2214,0xc285c,0xc2654,0xc215e,0xc1f4a,0xc2144,0xc2022}) do
  dbg:command(string.format('bpset %x,temp5==1 && a6==1014c4,{printf "MARCO_IDLE_OUT frame=%%d exit=%%08X %s\\n",temp9,%x,%s;temp5=0;g}',pc,fmt,pc,args))
 end
end
if os.getenv('MSX_STANCE_TRACE')=='1' then
 -- Entry initializers stop before the idle update at C1FA6.
 dbg:command('temp5=0')
 local fmt='mode=%08X attr=%04X f5f=%02X f6c=%02X character=%04X f80=%02X f81=%02X f90=%02X f91=%02X f92=%02X f93=%02X f95=%02X f97=%02X f98=%02X f9a=%02X f9b=%02X f9c=%02X f9d=%02X motion=%08X x=%08X y=%08X vx=%08X vy=%08X body_cursor=%08X body_timer=%04X body_speed=%04X upper_mode=%08X upper_cursor=%08X upper_timer=%04X upper_speed=%04X f17=%02X f18=%02X field50=%08X f72=%08X f76=%02X f77=%02X f78=%02X f79=%02X kind=%02X packed=%08X gate_mode=%02X gate_current=%02X gate_limit=%02X f7a=%02X f86=%02X f7b=%02X f96=%02X f1e=%04X f66=%04X'
 local args='d@(a6),w@(a6+14),b@(a6+5f),b@(a6+6c),w@(a6+6e),b@(a6+80),b@(a6+81),b@(a6+90),b@(a6+91),b@(a6+92),b@(a6+93),b@(a6+95),b@(a6+97),b@(a6+98),b@(a6+9a),b@(a6+9b),b@(a6+9c),b@(a6+9d),d@(a6+62),d@(a6+28),d@(a6+2c),d@(a6+30),d@(a6+34),d@(a6+20),w@(a6+24),w@(a6+26),d@(a6+7c),d@(a6+88),w@(a6+8c),w@(a6+8e),b@(a6+17),b@(a6+18),d@(a6+50),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+79),b@(a5+471e+w@(a6+70)),d@(a5+69ec),b@(a5+4718+w@(a6+70)),b@(a5+471a+w@(a6+70)),b@(a5+471c+w@(a6+70)),b@(a6+7a),b@(a6+86),b@(a6+7b),b@(a6+96),w@(a6+1e),w@(a6+66)'
 for _,pc in ipairs({0xc1e34,0xc1e94,0xc1ebe,0xc1efa,0xc1f4a}) do
  dbg:command(string.format('bpset %x,temp9>=%x && a6==1014c4 && temp5==0,{temp5=1;printf "MARCO_STANCE_IN frame=%%d entry=%%08X %s\\n",temp9,%x,%s;g}',pc,plan.first,fmt,pc,args))
 end
 for _,pc in ipairs({0xc1fa6}) do
  dbg:command(string.format('bpset %x,temp5==1 && a6==1014c4,{printf "MARCO_STANCE_OUT frame=%%d exit=%%08X %s\\n",temp9,%x,%s;temp5=0;g}',pc,fmt,pc,args))
 end
end
if os.getenv('MSX_CONTROLLER_TRACE')=='1' then
 -- Source-table reads and actual chosen pointer, before any initialization.
 for _,p in ipairs({{0xc0f32,0},{0xc0f62,1}}) do
  probe(p[1],'MARCO_PROGRAM_IN','part=%d table=%08X form=%02X character=%04X caller=%08X flash=%02X f18=%02X field50=%08X',
   p[2]..',a0,b@(a6+93),w@(a6+6e),d@(sp),b@(a6+17),b@(a6+18),d@(a6+50)')
 end
 for _,p in ipairs({{0xc0f54,0},{0xc0f84,1}}) do
  probe(p[1],'MARCO_PROGRAM_CHOICE','part=%d table=%08X offset=%04X program=%08X',p[2]..',a0,d7&ffff,d0')
 end
 for _,p in ipairs({{0xc0f60,0},{0xc0f94,1}}) do
  local state=p[2]==0 and 'a6+20' or 'a6+88'
  probe(p[1],'MARCO_PROGRAM_OUT','part=%d cursor=%08X timer=%04X speed=%04X flash=%02X f18=%02X field50=%08X',
   p[2]..',d@('..state..'),w@('..state..'+4),w@('..state..'+6),b@(a6+17),b@(a6+18),d@(a6+50)')
 end
 -- CA2CE selects the input buffer; capture its result as a boundary, then
 -- independently verify all masking and actor flag writes in C0FEA.
 probe(0xc0ff4,'MARCO_INPUT_IN','buffer=%08X packed=%08X mode=%02X current=%02X limit=%02X attr=%04X f5f=%02X f9a=%02X f9b=%02X f9c=%02X',
  'a0,d@(a0),b@(a5+4718+w@(a6+70)),b@(a5+471a+w@(a6+70)),b@(a5+471c+w@(a6+70)),w@(a6+14),b@(a6+5f),b@(a6+9a),b@(a6+9b),b@(a6+9c)')
 probe(0xc109a,'MARCO_INPUT_OUT','result=%08X f5f=%02X f9a=%02X f9b=%02X',
  'd0,b@(a6+5f),b@(a6+9a),b@(a6+9b)')
end
if os.getenv('MSX_PAIRED_TRACE')=='1' then
 local function snapshot(site,label,state,extra_fmt,extra_args,actions)
  local fmt='state=%08X cursor=%08X timer=%04X speed=%04X f17=%02X f18=%02X field50=%08X x=%04X y=%04X cached_x=%04X cached_y=%04X attr=%04X correction=%02X flash_palette=%02X palettes=%08X flags5f=%02X phase=%02X f72=%08X f76=%02X f77=%02X f78=%02X f79=%02X z=%04X'
  local args=state..',d@('..state..'),w@('..state..'+4),w@('..state..'+6),b@(a6+17),b@(a6+18),d@(a6+50),w@(a6+3c),w@(a6+40),w@(a6+44),w@(a6+48),w@(a6+14),b@(a6+4d),b@(a6+16),d@(a6+19),b@(a6+5f),b@(10000f),d@(a6+72),b@(a6+76),b@(a6+77),b@(a6+78),b@(a6+79),w@(a6+1e)'
  probe(site,label,fmt..(extra_fmt or ''),args..(extra_args or ''),actions)
 end
 snapshot(0x427e,'MARCO_STEP_IN','a0')
 snapshot(0x4350,'MARCO_STEP_OUT','a0',' sr=%04X',',sr')
 snapshot(0x4288,'MARCO_NULL_OUT','a0',' sr=%04X',',sr')
 snapshot(0xc3778,'MARCO_WEAPON_IN','a6+88')
 snapshot(0xc37cc,'MARCO_WEAPON_OUT','a6+88',' d7=%08X',',d7','trace off,maincpu;')
 local twin=' weapon_cursor=%08X weapon_timer=%04X weapon_speed=%04X'
 local twinargs=',d@(a6+88),w@(a6+8c),w@(a6+8e)'
 snapshot(0x44ec,'MARCO_EFFECT_IN','a6+20',twin..' target=%08X payload=%08X bytes=%04X',twinargs..',a1,a0,d0&ffff')
 snapshot(0x44ee,'MARCO_EFFECT_OUT','a6+20',twin,twinargs)
 snapshot(0x4492,'MARCO_SERVICE_IN','a6+20',twin..' token=%04X',twinargs..',d0&ffff')
 snapshot(0x4498,'MARCO_SERVICE_OUT','a6+20',twin,twinargs)
 dbg:command('temp8=0')
 dbg:command('bpset c377e,temp9>='..string.format('%x',plan.first)..' && a6==1014c4 && temp8==0,{temp8=1;trace '..output..'/weapon-step.log,maincpu,noloop,{tracelog "frame=%d A6=%08X A0=%08X D0=%08X D1=%08X SP=%08X ",temp9,a6,a0,d0,d1,sp};g}')
end
probe(0x4412,'MARCO_SELECT','cursor=%08X field=%02X value=%02X count=%04X state=%08X x=%04X y=%04X attributes=%04X z=%04X correction=%02X flash_palette=%02X palettes=%08X',
 'a1-1,b@(a1),b@(a6+b@(a1)),w@(a1+1)/8,a0,w@(a6+3c),w@(a6+40),w@(a6+14),w@(a6+1e),b@(a6+4d),b@(a6+16),d@(a6+19)')
probe(0x4256,'MARCO_INIT','program=%08X speed=%04X','a1,d0&ffff')
probe(0x4406,'MARCO_DRAW','descriptor=%08X xy=%08X attr=%04X z=%04X','d7,d0,d1&ffff,d5&ffff')
probe(0x44ec,'MARCO_CALLBACK','target=%08X','a1')
dbg:command('wpset 1014e4,4,w,temp9>='..string.format('%x',plan.first)..',{printf "MARCO_CURSOR_WRITE frame=%d pc=%08X a6=%08X address=%08X data=%08X\\n",temp9,pc,a6,wpaddr,wpdata;g}')
local boot={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1701]={'P1 A',0}}
emu.register_frame_done(function()
 frame=frame+1;dbg:command(string.format('temp9=%x',frame))
 if boot[frame] then button(boot[frame][1],boot[frame][2]) end
 -- Capture only (MSX_GIVE_HGUN): the H as a pickup gives it (weapon id 0A at
 -- 10C71E, rounds 10C724, 10C71C = FF, +94 = 1; tools/capture_weapons.lua
 -- plan hold), rounds kept at 200 so the plan never runs out.
 if os.getenv('MSX_GIVE_HGUN')=='1' and frame>=plan.first then
  if frame==plan.first then cpu:write_u8(0x10c71e,0x0a);cpu:write_u8(0x10c71c,0xff);cpu:write_u8(0x1014c4+0x94,1) end
  cpu:write_u16(0x10c724,200)
 end
 -- Capture only (MSX_GIVE_CGUN): the C as a pickup gives it (id 0C at
 -- 10C71E, 40 rounds at 10C724, cap 3 at 10C71C, none live 10C71A, +94 = 1;
 -- tools/capture_weapons.lua plan air), rounds kept at 40.
 if os.getenv('MSX_GIVE_CGUN')=='1' and frame>=plan.first then
  if frame==plan.first then cpu:write_u8(0x10c71e,0x0c);cpu:write_u8(0x10c71c,3);cpu:write_u8(0x10c71a,0);cpu:write_u8(0x1014c4+0x94,1) end
  cpu:write_u16(0x10c724,40)
 end
 -- Capture only (MSX_KEEP_BOMBS): bombs 10C720 kept at 10 so a plan of
 -- several throws never runs out (the assist of capture_weapons.lua).
 if os.getenv('MSX_KEEP_BOMBS')=='1' and frame>=plan.first then cpu:write_u8(0x10c720,10) end
 for _,event in ipairs(plan.events[frame] or {}) do button(event[1],event[2]) end
 if frame>=plan.first and frame<=plan.last and os.getenv('MSX_QUEUE_TRACE')=='1' then
  if frame==plan.first+2 then
   dbg:command('wpset a5+1946,1,w,1,{printf "MARCO_QUEUE_SWAP frame=%d pc=%08X a5=%08X data=%08X flag=%02X phase=%02X\\n",temp9,pc,a5,wpdata,b@(a5+1946),b@(10000f);g}')
   dbg:command('wpset 10000f,1,w,1,{printf "MARCO_PHASE_WRITE frame=%d pc=%08X data=%08X\\n",temp9,pc,wpdata;g}')
  end
  dbg:command(string.format('printf "MARCO_VIDEO_STATE frame=%%d a5=%%08X flag=%%02X phase=%%02X head0=%%08X head1=%%08X\\n",%x,a5,b@(a5+1946),b@(10000f),d@(a5+1d48),d@(a5+2d4e)',frame))
 end
 if frame>=plan.first and frame<=plan.last then
  local file=assert(io.open(output..'/actor'..frame..'.bin','wb'));file:write(cpu:read_range(0x1014c4,0x101573,8));file:close()
  states:write(string.format('{"frame":%d,"routine":%d,"cursor":%d,"x":%d,"y":%d,"projected_x":%d,"projected_y":%d,"flags":%d}\n',frame,cpu:read_u32(0x1014c4),cpu:read_u32(0x1014e4),cpu:read_u32(0x1014ec),cpu:read_u32(0x1014f0),cpu:read_u32(0x101500),cpu:read_u32(0x101504),cpu:read_u16(0x1014d8)));states:flush()
  -- MSX_DUMP_ROUTINES=lo-hi,lo-hi (hex): VRAM/palette/screen only while the
  -- player's routine (+0) is in a range, plus the 4 frames after (the
  -- three-tick submission delay); the audit takes such a sparse capture.
  local dump=true
  local ranges=os.getenv('MSX_DUMP_ROUTINES')
  if ranges then
   local routine=cpu:read_u32(0x1014c4);local inside=false
   for lo,hi in ranges:gmatch('(%x+)-(%x+)') do if routine>=tonumber(lo,16) and routine<=tonumber(hi,16) then inside=true end end
   if inside then dump_until=frame+4 end
   dump=dump_until~=nil and frame<=dump_until
  end
  if os.getenv('MSX_LOGIC_ONLY')~='1' and dump then
  local v=assert(io.open(output..'/vram'..frame..'.bin','wb'));for i=0,0x85ff do v:write(string.pack('>I2',video:read(i))) end;v:close()
  local p=assert(io.open(output..'/palette'..frame..'.bin','wb'));for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end;p:close()
  manager.machine.screens[':screen']:snapshot(output..'/game'..frame..'.png')
  end
 end
 if frame==plan.first+2 then
  for _,r in ipairs({{'program-fixed.bin',0,0xfffff},{'program-bank.bin',0x200000,0x2fffff}}) do
   local file=assert(io.open(output..'/'..r[1],'wb'));file:write(cpu:read_range(r[2],r[3],8));file:close()
  end
 end
 if frame==plan.last then inputs:close();states:close();manager.machine:exit() end
end,'frame')
