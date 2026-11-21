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
-- MSX_UPDATE_TRACE=1: the player's node words right after its routine returns to the node loop
-- (004248 jsr (a0) -> 00424A), once per game update whatever the frame (Mission 6 runs an update in
-- 1..5 frames: the frame-end actor dumps can catch an update half done).
if os.getenv('MSX_UPDATE_TRACE')=='1' then
 probe(0x424a,'MARCO_UPDATE','r=%08X f15=%02X v30=%08X v34=%08X x3c=%08X y40=%08X x28=%08X y2c=%08X p38=%08X f26=%04X f5a=%02X f5b=%02X f93=%02X f6e=%04X f19=%02X f80=%04X',
  'd@a6,b@(a6+15),d@(a6+30),d@(a6+34),d@(a6+3c),d@(a6+40),d@(a6+28),d@(a6+2c),d@(a6+38),w@(a6+26),b@(a6+5a),b@(a6+5b),b@(a6+93),w@(a6+6e),b@(a6+19),w@(a6+80)')
 -- The water death 0C2D06: 03C55C found the surface (carry) -> 0C2D28 with D1 the surface - 1 relative
 -- to the object A0 (its +18 added next).
 probe(0xc2d28,'MARCO_WATER','d1=%08X a0=%08X y18=%08X y40=%08X f80=%04X','d1,a0,d@(a0+18),d@(a6+40),w@(a6+80)')
end
probe(0x4406,'MARCO_DRAW','descriptor=%08X xy=%08X attr=%04X z=%04X','d7,d0,d1&ffff,d5&ffff')
probe(0x44ec,'MARCO_CALLBACK','target=%08X','a1')
dbg:command('wpset 1014e4,4,w,temp9>='..string.format('%x',plan.first)..',{printf "MARCO_CURSOR_WRITE frame=%d pc=%08X a6=%08X address=%08X data=%08X\\n",temp9,pc,a6,wpaddr,wpdata;g}')
local boot={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1701]={'P1 A',0}}
-- MSX_CHARACTER=n (player field +6E: 0 Marco, 1 Tarma, 2 Eri, 3 Fio;
-- docs/PERSONAJES.md): chosen with the controller only. The P1 cursor shows
-- on Marco at about 1650 (screen order Marco, Eri, Tarma, Fio); Right is
-- pressed once per place from 1700 and A at 1800 (the landing then comes
-- 250 frames later than with the A at 1550: plan.first >= 2350).
local character=tonumber(os.getenv('MSX_CHARACTER') or '-1')
if character>=0 then
 local place=({[0]=0,[1]=2,[2]=1,[3]=3})[character];assert(place,'MSX_CHARACTER 0..3')
 boot[1550]=nil;boot[1701]=nil
 for k=0,place-1 do boot[1700+20*k]={'P1 Right',1};boot[1710+20*k]={'P1 Right',0} end
 boot[1800]={'P1 A',1};boot[1810]={'P1 A',0}
end
-- MSX_SELECT_INPUTS=<string of L/R>: instead of MSX_CHARACTER, one Left or
-- Right press per letter from 1700 (every 20 frames) and A at 1800: how the
-- cursor moves at the ends of the row (docs/PERSONAJES.md).
local select_inputs=os.getenv('MSX_SELECT_INPUTS')
if select_inputs then
 boot[1550]=nil;boot[1701]=nil
 for k=1,#select_inputs do
  local b=select_inputs:sub(k,k)=='L' and 'P1 Left' or 'P1 Right'
  boot[1700+20*(k-1)]={b,1};boot[1710+20*(k-1)]={b,0}
 end
 boot[1800]={'P1 A',1};boot[1810]={'P1 A',0}
end
-- MSX_LOAD=<state> with MSX_LOAD_FRAME=<frame> (capture only, run_marco_catalog.py --load): a
-- save state of the -state_directory loaded at frame 1, the count going on from its frame (the
-- boot inputs are then never reached).
-- MSX_FAMILY_ROUTINES=lo-hi,lo-hi (hex) with MSX_FAMILY_FROM/TO (frames;
-- capture only, run_marco_catalog.py --family-routines): boss-trace.txt in
-- the format of tools/capture_weapons.lua (U/V node dumps around every update
-- 004246..00424A of a work-RAM node whose routine +0 lies in a range, W its
-- writes, F frames) for tools/export_rest_sprites.py DIR@lo-hi.
local fam_ranges={}
for lo,hi in (os.getenv('MSX_FAMILY_ROUTINES') or ''):gmatch('(%x+)-(%x+)') do fam_ranges[#fam_ranges+1]={tonumber(lo,16),tonumber(hi,16)} end
local fam_from=tonumber(os.getenv('MSX_FAMILY_FROM') or '0');local fam_to=tonumber(os.getenv('MSX_FAMILY_TO') or '0')
local mstate=manager.machine.devices[':maincpu'].state
local fam_log,fam_rtap,fam_wtap=nil,nil,nil;local fam_busy=true;local fam_active=nil
local FPOOL0,FPOOL1=0x1010a4,0x1010a4+0xb0*100
local function fam_hex(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
local function fam_read(address,data,mask)
 if fam_busy then return end
 if address>=FPOOL0 and address<FPOOL1 then
  local o=(address-FPOOL0)%0xb0
  if o==0 and mstate['CURPC'].value==0x4246 then
   fam_busy=true
   local r=cpu:read_u32(address);fam_active=nil
   for _,q in ipairs(fam_ranges) do if r>=q[1] and r<=q[2] then fam_active=address end end
   if fam_active then fam_log:write(string.format('U %x %x %s\n',address,r,fam_hex(address,0xb0))) end
   fam_busy=false
  elseif o==8 and fam_active==address-8 and mstate['CURPC'].value==0x424a then
   fam_busy=true;fam_log:write(string.format('V %x %s\n',fam_active,fam_hex(fam_active,0xb0)));fam_busy=false;fam_active=nil
  end
 end
end
local function fam_write(address,data,mask)
 if fam_busy or not fam_active then return end
 fam_log:write(string.format('W %x %x %x %x\n',address,mask,data,mstate['CURPC'].value))
end
local function fam_frame(f)
 if #fam_ranges==0 then return end
 if f==fam_from then
  fam_log=assert(io.open(output..'/boss-trace.txt','w'))
  fam_rtap=cpu:install_read_tap(0x100000,0x10ffff,'fam_r',fam_read)
  fam_wtap=cpu:install_write_tap(0x100000,0x10ffff,'fam_w',fam_write)
 end
 if fam_log then
  fam_log:write(string.format('F %d\n',f))
  if f==fam_to then fam_rtap:remove();fam_wtap:remove();fam_log:close();fam_log=nil end
 end
end
local load_name=os.getenv('MSX_LOAD')
emu.register_frame_done(function()
 frame=frame+1
 fam_busy=true
 if load_name and frame==1 then manager.machine:load(load_name);frame=tonumber(os.getenv('MSX_LOAD_FRAME')) end
 dbg:command(string.format('temp9=%x',frame))
 -- MSX_CHARACTER_WATCH=1: every access to player 1's character word +6E
 -- (101532) from frame 1600 on (the choice), to find its readers
 -- (docs/PERSONAJES.md).
 if frame==1600 and os.getenv('MSX_CHARACTER_WATCH')=='1' then
  for _,sym in ipairs({'curpc','ppc','genpc'}) do dbg:command('wpset 101532,2,rw,1,{printf "CHARACTER_ACCESS frame=%d '..sym..'=%08X a6=%08X data=%08X\\n",temp9,'..sym..',a6,wpdata;g}') end
 end
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
 -- Capture only (MSX_GIVE_RGUN): the R as a pickup gives it (id 03 at
 -- 10C71E, 30 rounds, cap 2, none live, +94 = 1: marco_weapons_grant),
 -- rounds kept at 30.
 if os.getenv('MSX_GIVE_RGUN')=='1' and frame>=plan.first then
  if frame==plan.first then cpu:write_u8(0x10c71e,0x03);cpu:write_u8(0x10c71c,2);cpu:write_u8(0x10c71a,0);cpu:write_u8(0x1014c4+0x94,1) end
  cpu:write_u16(0x10c724,30)
 end
 -- Capture only (MSX_GIVE_WEAPON=<hex id>): any weapon as a pickup gives it
 -- (0C0928: id at 10C71E, cap 10C71C from ROM 0C08A8, none live 10C71A,
 -- +94 from 0C0918, rounds from 0C08B8), rounds kept at that value.
 local give_weapon=os.getenv('MSX_GIVE_WEAPON')
 if give_weapon and frame>=plan.first then
  local id=tonumber(give_weapon,16)
  if frame==plan.first then cpu:write_u8(0x10c71e,id);cpu:write_u8(0x10c71c,cpu:read_u8(0x0c08a8+id));cpu:write_u8(0x10c71a,0);cpu:write_u8(0x1014c4+0x94,cpu:read_u8(0x0c0918+id)) end
  cpu:write_u16(0x10c724,cpu:read_u16(0x0c08b8+2*id))
 end
 -- Capture only (MSX_GIVE_MUMMY): the form request +94 = 3 at the first plan
 -- frame, as the player's contact 0C36C0 sets it for a hit of kind 0x10
 -- (the mummy's breath); the original forms check 0C3428 then runs the
 -- transformation 0C17A0/0C182A and the mummy controller 0C18B4.
 if os.getenv('MSX_GIVE_MUMMY')=='1' and frame==plan.first then cpu:write_u8(0x1014c4+0x94,3) end
 -- Capture only (MSX_GIVE_FAT): form 2 as the food 0C0BF4 requests it
 -- (+94 = 2) with the mission value RAM 10C2F6 = 4 (Mission 3), the one
 -- for which 0C0BAE keeps the form (else the forms check drops it).
 if os.getenv('MSX_GIVE_FAT')=='1' and frame==plan.first then cpu:write_u16(0x10c2f6,4);cpu:write_u8(0x1014c4+0x94,2) end
 -- Capture only (MSX_VICTORY=<frames after the first plan frame>): the
 -- mission end as 018F92/018846 leave it for player 1 (RAM 10C304 = 04, the
 -- boss's +5E in MAME runK; 10C301 = FF): the idle 0C2022 then takes the
 -- victory table 0BF658 (0C1E84).
 local victory=tonumber(os.getenv('MSX_VICTORY') or '-1')
 if victory>=0 and frame==plan.first+victory then cpu:write_u8(0x10c304,4);cpu:write_u8(0x10c301,0xff) end
 -- Capture only (MSX_HIT=<frames after the first plan frame>:<hex kind>):
 -- the player's hit as the contact 004C24 leaves it (+5A = kind, +5F bit 7);
 -- the player's 0C3702 then runs the death 0C2956 (forms 2/3: 0C2A36/0C2C40).
 local hit=os.getenv('MSX_HIT')
 if hit then
  local after,kind=hit:match('^(%d+):(%x+)$')
  if frame==plan.first+tonumber(after) then
   cpu:write_u8(0x1014c4+0x5a,tonumber(kind,16));cpu:write_u8(0x1014c4+0x5f,cpu:read_u8(0x1014c4+0x5f)|0x80)
  end
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
 -- Capture only (MSX_CHILD_ROUTINES=hex,hex): every node of the work RAM
 -- whose routine (+0) is one of these and whose creator (+0C) is the
 -- player 1014C4, its 0xB0 bytes as hex, one line per node and frame.
 local children=os.getenv('MSX_CHILD_ROUTINES')
 if children and frame>=plan.first and frame<=plan.last then
  if not child_log then child_log=assert(io.open(output..'/children.log','w')) end
  local ram=cpu:read_range(0x100000,0x10ffff,8)
  for r in children:gmatch('(%x+)') do
   local pat=string.pack('>I4',tonumber(r,16));local at=1
   while true do
    local i=ram:find(pat,at,true);if not i then break end
    if (i-1)%2==0 and ram:sub(i+12,i+15)==string.pack('>I4',0x1014c4) then
     child_log:write(string.format('%d %06X %s\n',frame,0x100000+i-1,(ram:sub(i,i+0xaf):gsub('.',function(c) return string.format('%02x',c:byte()) end))))
    end
    at=i+2
   end
  end
  child_log:flush()
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
 -- The program blocks after the landing (frame 2102 at the latest): before
 -- it the protection words at 2FFFE0.. still change (descent captures).
 if frame==math.max(plan.first,2100)+2 then
  for _,r in ipairs({{'program-fixed.bin',0,0xfffff},{'program-bank.bin',0x200000,0x2fffff}}) do
   local file=assert(io.open(output..'/'..r[1],'wb'));file:write(cpu:read_range(r[2],r[3],8));file:close()
  end
 end
 fam_frame(frame);fam_busy=false
 if frame==plan.last then if fam_log then fam_log:close() end;inputs:close();states:close();manager.machine:exit() end
end,'frame')
