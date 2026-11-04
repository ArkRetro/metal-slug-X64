-- Natural execution observer. No emulated RAM/register or device-register writes.
local output=assert(os.getenv('MSX_CAPTURE_DIR'))
local finish=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '2460')
local frame=0
local cpu=manager.machine.devices[':maincpu'].spaces.program
local dbg=assert(manager.machine.debugger)
local commands=assert(io.open(output..'/commands.log','w'))
local inputs=assert(io.open(output..'/inputs.jsonl','w'))
local function command(text)
 commands:write(text..'\n');commands:flush();dbg:command(text)
end
command('temp0=0;temp1=0;temp2=0xffffffff;temp3=0xffffffff;temp4=0;temp9=0')
local function event(kind,extra_fmt,extra_args)
 return 'printf "CTX kind='..kind..' frame=%d reported_pc=%08X d0=%08X d1=%08X d2=%08X d3=%08X d4=%08X d5=%08X d6=%08X d7=%08X a0=%08X a1=%08X a2=%08X a3=%08X a4=%08X a5=%08X sp=%08X sr=%04X return_pc=%08X ram0=%08X ramfc=%08X slot0=%08X slot1=%08X'..(extra_fmt or '')..'",temp9,pc,d0,d1,d2,d3,d4,d5,d6,d7,a0,a1,a2,a3,a4,a5,sp,sr,d@(sp),d@(0x100000),d@(0x10fc00),d@(0x10fc00),d@(0x10fc20)'..(extra_args or '')
end
-- Observe actual indirect IRQ targets; RAM-test patterns are not code roots.
for _,probe in ipairs({{0x22da,2},{0x2354,3}}) do
 local args=string.format(',0x%x,a0',probe[1])
 command(string.format('bpset 0x%x:maincpu,a0!=temp%d,{%s;temp%d=a0;g}',
  probe[1],probe[2],event('irq_target_change',' site=%08X target=%08X',args),probe[2]))
end
command('bpset 0x2850:maincpu,temp4==0,{'..event('irq_idle_entry',' site=%08X opcode=%04X',',0x2850,w@(0x2850)')..';temp4=1;g}')
for _,probe in ipairs({
 {0x1004,'rom_001004'},{0x1040,'rom_001040'},{0x1046,'rom_001046'},
 {0x1070,'rom_001070'},{0x10ea,'rom_0010ea'},
 {0x100000,'ram_100000'},{0x10081a,'ram_dispatch'},{0x10fc00,'ram_10fc00'},
 {0x10fc68,'ram_10fc68'},
}) do
 local fmt=' site=%08X'..(probe[2]=='ram_dispatch' and ' target=%08X' or '')
 local args=string.format(',0x%x',probe[1])..(probe[2]=='ram_dispatch' and ',a0' or '')
 local action=event(probe[2],fmt,args)
 if probe[2]=='ram_100000' then action=action..';save "'..output..'/last-entry-100000.bin",0x100000:maincpu,0x114a' end
 if probe[2]=='ram_10fc00' then action=action..';save "'..output..'/last-entry-10fc00.bin",0x10fc00:maincpu,0x6a' end
 command(string.format('bpset 0x%x:maincpu,1,{%s;g}',probe[1],action))
end
-- Separate first-copy trace starts avoid multiple breakpoints at the same PC.
for _,probe in ipairs({
 {0x1024,'copy_100000',0},{0x10da,'copy_10fc00',1},
}) do
 local trace=output..'/'..probe[2]..'.log'
 local action=event(probe[2],' site=%08X',string.format(',0x%x',probe[1]))..';temp'..probe[3]..'=1;trace "'..trace..'",maincpu,noloop,{tracelog "FRAME=%d A4=%08X A0=%08X SR=%04X ",temp9,a4,a0,sr};g'
 command(string.format('bpset 0x%x:maincpu,temp%d==0,{%s}',probe[1],probe[3],action))
end
for _,region in ipairs({{0x100000,0x10,'code_prefix'},{0x10fc00,0x24,'descriptor_or_code'}}) do
 command(string.format('wpset 0x%x:maincpu,0x%x,w,1,{%s;g}',region[1],region[2],
  event('write_'..region[3],' address=%08X data=%016X',',wpaddr,wpdata')))
end
local function button(name,value)
 local found=false
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then
    field:set_value(value);found=true
   end
  end
 end
 assert(found,'Missing input '..name)
 inputs:write(string.format('{"frame":%d,"name":"%s","value":%d}\n',frame,name,value));inputs:flush()
end
local actions={
 [700]={'Coin 1',1},[710]={'Coin 1',0},
 [760]={'1 Player Start',1},[770]={'1 Player Start',0},
 [1450]={'P1 A',1},[1460]={'P1 A',0},
 [1550]={'P1 A',1},[1710]={'P1 A',0},
 [2280]={'P1 Right',1},
}
emu.register_frame_done(function()
 frame=frame+1
 -- Traces cover at most the first partial VI after each first-copy breakpoint.
 command('trace off,maincpu')
 dbg:command(string.format('temp9=0x%x',frame))
 if actions[frame] then button(actions[frame][1],actions[frame][2]) end
 local ram=assert(io.open(string.format('%s/ram/%04d.bin',output,frame),'wb'))
 ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 if frame==finish then
  command(event('capture_end'))
  inputs:close();commands:close()
  local file=assert(io.open(output..'/finished.json','w'))
  file:write(string.format('{"frames":%d,"mode":"inputs_only_no_game_state_patches"}\n',frame));file:close()
  manager.machine.screens[':screen']:snapshot(output..'/final.png')
  manager.machine:exit()
 end
end,'frame')
