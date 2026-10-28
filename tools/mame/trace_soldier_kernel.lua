-- Soldier collision-kernel boundaries on top of trace_soldier_campaign.lua.
-- Logs entry/exit of ROM 013968 (generic sweep) and 013DB2 (floor search)
-- whenever the current actor runs a soldier routine (ROM 0AE000..0B1000) or
-- the shared death routines (ROM 040D00..041000). Print-only breakpoints.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_campaign.lua')
local first=tonumber(os.getenv('MSX_RAM_FIRST') or '2100')
local dbg=assert(manager.machine.debugger)
local frame=0
local cond='((d@(a6)>=ae000 && d@(a6)<b1000) || (d@(a6)>=40d00 && d@(a6)<41000))'
local regs='d0=%08X d1=%08X d2=%08X d3=%08X d4=%08X d5=%08X d6=%08X d7=%08X a0=%08X a1=%08X'
local args='d0,d1,d2,d3,d4,d5,d6,d7,a0,a1'
local mode='mode=%08X m8=%02X m9=%02X ma=%02X mb=%02X mc=%02X md=%02X'
local margs='d@(10c004),b@(10c008),b@(10c009),b@(10c00a),b@(10c00b),b@(10c00c),b@(10c00d)'
-- Scene objects used by the kernel: terrain 10C72E and root 10E968 (+14..+20)
-- and the persistent scratch words RAM 10C018/10C068.
local scene='printf "KSC t=%08X,%08X,%08X,%08X r=%08X,%08X,%08X,%08X w4018=%04X w4068=%04X gm=%08X",d@(10c742),d@(10c746),d@(10c74a),d@(10c74e),d@(10e97c),d@(10e980),d@(10e984),d@(10e988),w@(10c018),w@(10c068),d@(10c00e)'
emu.register_frame_done(function()
 frame=frame+1
 if frame==first-1 then
  dbg:command('bpset 13968,'..cond..',{printf "KIN f=%d a6=%06X rt=%06X ret=%06X '..regs..' '..mode..'",temp9,a6,d@(a6),d@(sp),'..args..','..margs..';'..scene..';g}')
  dbg:command('bpset 13c18,'..cond..',{printf "KOUT f=%d a6=%06X '..regs..'",temp9,a6,'..args..';g}')
  dbg:command('bpset 13db2,'..cond..',{printf "FIN f=%d a6=%06X rt=%06X ret=%06X '..regs..' '..mode..'",temp9,a6,d@(a6),d@(sp),'..args..','..margs..';'..scene..';g}')
  dbg:command('bpset 1471c,'..cond..',{printf "FOUT f=%d a6=%06X '..regs..'",temp9,a6,'..args..';g}')
 end
end,'frame')
