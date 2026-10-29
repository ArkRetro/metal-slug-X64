-- Extend the exact first-soldier inputs with read-only RNG entry/return probes.
-- Addresses originate in the executed 0x29C6 trace and Ghidra disassembly.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_first_soldier.lua')
local dbg=assert(manager.machine.debugger)
local words={}
for address=0x100016,0x100082,2 do words[#words+1]=string.format('w@(%x)',address) end
local function probe(address,label,condition,extra)
 local fmt=label..' seq=%d frame=%d d0=%08X cursor=%08X ret=%08X counter=%04X a6=%08X f15=%02X f92=%04X state='..string.rep('%04X',55)..'\\n'
 local args='temp2,temp9,d0,d@(100084),d@(sp),w@(10f006),a6,b@(a6+15),w@(a6+92),'..table.concat(words,',')
 if label=='RNG_OUT' and os.getenv('MSX_RNG_CAPTURE_SR')=='1' then
  fmt=fmt:sub(1,-3)..' sr=%04X mixed=%04X\\n';args=args..',sr,w@(a3)'
 end
 dbg:command(string.format('bpset %x,%s,{%sprintf "%s",%s;g}',address,condition,extra or '',fmt,args))
end
dbg:command('temp2=0')
probe(0x2976,'SEED_IN','1')
probe(0x29c4,'SEED_OUT','1')
probe(0x29c6,'RNG_IN','temp9>=898 && temp9<99c','temp2++;') -- frames 2200..2459
probe(0x29f0,'RNG_OUT','temp9>=898 && temp9<99c')
dbg:command('temp4=0')
dbg:command('bpset af6c0,a6==101f14 && temp9>=95a && temp4==0,{temp4=1;trace '..assert(os.getenv('MSX_CAPTURE_DIR'))..'/state-instructions.log,maincpu,noloop,{tracelog "A6=%08X D0=%08X D1=%08X D6=%08X SR=%04X ",a6,d0,d1,d6,sr};g}')
for _,address in ipairs({0xaf6d0,0xaf6f0,0xaf72a}) do
 dbg:command(string.format('bpset %x,a6==101f14,{printf "STATE site=%06X pc=%%08X frame=%%d routine=%%08X d6=%%08X f78=%%04X f84=%%04X f3c=%%04X f62=%%04X f7c=%%02X f6c=%%02X\\n",pc,temp9,d@(a6),d6,w@(a6+78),w@(a6+84),w@(a6+3c),w@(a6+62),b@(a6+7c),b@(a6+6c);g}',address,address))
end
local frame=0
emu.register_frame_done(function()
 frame=frame+1
 if frame==2401 then dbg:command('trace off,maincpu') end
end,'frame')
