-- Recover real hit-query inputs/candidates and side effects of the thrown actor.
-- No gameplay memory writes; identity is the child allocated by 03BB8E.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'trace_soldier_throw_lifecycle.lua')
local dbg=assert(manager.machine.debugger)
local function probe(site,label,fmt,args)
 dbg:command(string.format('bpset %x,a6==temp6 && temp7==1,{printf "%s frame=%%d actor=%%08X%s\\n",temp9,a6%s;g}',site,label,fmt,args))
end
local function words(expr,count)
 local fmt,args='', ''
 for i=0,count-1 do fmt=fmt..'%04X';args=args..',w@('..expr..'+'..string.format('%x',2*i)..')' end
 return fmt,args
end
local f,a=words('a0',32)
probe(0x49e8,'HIT_QUERY_IN',' query=%08X x=%04X y=%04X flags=%02X source5e=%02X descriptor='..f,',a0,w@(a6+3c),w@(a6+40),b@(a6+15),b@(a6+5e)'..a)
-- The selected read bank is A4; 64 pointers, count and bitmaps total 0x1B2 bytes.
-- Stay below the debugger printf parameter limit by splitting the table.
local p,pa=words('a4',0x40)
local q,qa=words('a4+80',0x40)
probe(0x4a0c,'HIT_BANK_HEAD',' bank=%08X phase=%02X head='..p,',a4,b@(10e3e2)'..pa)
local p2,pa2=words('a4+100',0x59)
local actorfmt=' target=%08X x=%04X y=%04X flags=%02X flash=%02X field4e=%04X descriptor=%08X callback=%08X status58=%04X status5a=%04X status5c=%04X status5e=%04X'
local actorargs=',a2,w@(a2+3c),w@(a2+40),b@(a2+15),b@(a2+17),w@(a2+4e),d@(a2+50),d@(a2+54),w@(a2+58),w@(a2+5a),w@(a2+5c),w@(a2+5e)'
probe(0x4a1e,'HIT_BANK_MIDDLE',' bank=%08X middle='..q,',a4'..qa)
probe(0x4a24,'HIT_BANK_TAIL',' bank=%08X tail='..p2,',a4'..pa2)
local tf,ta=words('d@(a2+50)',24)
probe(0x476c,'HIT_CANDIDATE_IN',actorfmt..' index=%04X d3=%08X d4=%08X d5=%08X query=%08X data='..tf,actorargs..',d2&ffff,d3,d4,d5,a0'..ta)
probe(0x49e6,'HIT_CANDIDATE_OUT',actorfmt..' result=%04X max_target=%08X min_target=%08X',actorargs..',w@(106754),d@(1067d6),d@(1067dc)')
