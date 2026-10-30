// Static audio driver reconstruction. Banked execution remains explicitly scoped.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.pcode.JumpTable;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
public class ExportZ80Code extends GhidraScript {
 public void run() throws Exception {
  File out=new File(getScriptArgs()[0]);out.mkdirs();
  currentProgram.getMemory().createUninitializedBlock("AUDIO_RAM",toAddr(0xf800),0x800,false).setWrite(true);
  // Bounds come from CP 10, nibble dispatch, four-word type tables, and C0<=op<DA.
  // The four-byte stubs preserve EXX before JP; skipping EXX changes register banks.
  long[][] tables={{0x2026,0x2027,16,2,0},{0x2063,0x2064,16,2,0},
   {0x2633,0x2634,16,4,1},{0x2861,0x2862,16,4,1},{0x2a8f,0x2a90,16,4,1},
   {0x4ac6,0x4ac7,4,2,0},{0x4ae2,0x4ae3,4,2,0},{0x4afe,0x4aff,4,2,0},{0x5396,0x5397,26,3,2}};
  LinkedHashSet<Long> queue=new LinkedHashSet<>();for(long pc:new long[]{0,8,16,24,32,40,48,56,0x66})queue.add(pc);
  Map<Long,ArrayList<Address>> destinations=new LinkedHashMap<>();List<String> tableRows=new ArrayList<>();
  for(long[] t:tables) {
   ArrayList<Address> targets=new ArrayList<>();
   for(int n=0;n<t[2];n++) {
    long at=t[1]+n*t[3],target;
    if(t[4]==0) {target=currentProgram.getMemory().getShort(toAddr(at))&65535;targets.add(toAddr(target));}
    else {
     if(t[4]==1) {if((currentProgram.getMemory().getByte(toAddr(at))&255)!=0xd9 || (currentProgram.getMemory().getByte(toAddr(at+1))&255)!=0xc3)throw new IOException("EXX/JP table bytes differ");target=currentProgram.getMemory().getShort(toAddr(at+2))&65535;}
     else {if((currentProgram.getMemory().getByte(toAddr(at))&255)!=0xc3)throw new IOException("JP table bytes differ");target=currentProgram.getMemory().getShort(toAddr(at+1))&65535;}
     targets.add(toAddr(at));disassemble(toAddr(at));
    }
    if(target>=0xf800)throw new IOException("Table target outside initial ROM map");queue.add(target);
    tableRows.add(Long.toHexString(t[0])+"\t"+n+"\t"+Long.toHexString(at)+"\t"+Long.toHexString(target));
   }
   destinations.put(t[0],targets);
  }
  Set<Long> done=new HashSet<>(),inspected=new HashSet<>(),overrides=new HashSet<>();Set<String> unknown=new TreeSet<>();
  while(done.size()<2000) {
   List<Long> batch=new ArrayList<>();for(Long pc:queue)if(!done.contains(pc))batch.add(pc);if(batch.isEmpty())break;
   for(Long pc:batch) {done.add(pc);Address a=toAddr(pc);if(!disassemble(a) && getInstructionAt(a)==null)continue;if(getFunctionAt(a)==null && getFunctionContaining(a)==null)createFunction(a,"z80_candidate_"+Long.toHexString(pc));}
   InstructionIterator all=currentProgram.getListing().getInstructions(true);
   while(all.hasNext()) {Instruction i=all.next();if(!inspected.add(i.getAddress().getOffset()))continue;
    if(i.getFlowType().isCall())for(Address t:i.getFlows())if(t.getOffset()<0xf800)queue.add(t.getOffset());
   }
   for(Long site:destinations.keySet())if(!overrides.contains(site)) {
    Function f=getFunctionContaining(toAddr(site));Instruction i=getInstructionAt(toAddr(site));if(f==null || i==null)continue;
    ArrayList<Address> dest=destinations.get(site);for(Address target:dest)i.addOperandReference(0,target,RefType.COMPUTED_JUMP,SourceType.ANALYSIS);
    new JumpTable(toAddr(site),dest,true).writeOverride(f);CreateFunctionCmd.fixupFunctionBody(currentProgram,f,monitor);overrides.add(site);
   }
  }
  DecompInterface d=new DecompInterface();d.openProgram(currentProgram);Set<Long> owners=new HashSet<>();List<Long> roots=new ArrayList<>(done);Collections.sort(roots);
  try(PrintWriter index=new PrintWriter(new File(out,"functions.tsv"));PrintWriter c=new PrintWriter(new File(out,"TENTATIVE_AUDIO_C.c"));PrintWriter asm=new PrintWriter(new File(out,"instructions.tsv"))) {
   index.println("seed\towner\tstatus");c.println("/* TENTATIVE Z80 C after statically bounded dispatch-table recovery. Initial bank mapping only. I/O, types and banking require validation. Not a native N64 driver. */");
   for(Long pc:roots) {
    Function f=getFunctionAt(toAddr(pc));if(f==null)f=getFunctionContaining(toAddr(pc));if(f==null){index.println(Long.toHexString(pc)+"\t-\tno_function");continue;}
    long owner=f.getEntryPoint().getOffset();if(!owners.add(owner)){index.println(Long.toHexString(pc)+"\t"+Long.toHexString(owner)+"\tshared_owner");continue;}
    DecompileResults r=d.decompileFunction(f,5,monitor);index.println(Long.toHexString(pc)+"\t"+Long.toHexString(owner)+"\t"+(r.decompileCompleted()?"tentative_c":"failed"));
    c.println("\n/* Initial-map Z80 owner 0x"+Long.toHexString(owner)+". */");if(r.decompileCompleted())c.println(r.getDecompiledFunction().getC());
   }
   asm.println("address\tinstruction");InstructionIterator all=currentProgram.getListing().getInstructions(true);
   while(all.hasNext()) {Instruction i=all.next();asm.println(i.getAddress()+"\t"+i);if(i.getFlowType().isComputed() && !overrides.contains(i.getAddress().getOffset()))unknown.add(i.getAddress()+"\tcomputed\t"+i);}
  }finally{d.dispose();}
  try(PrintWriter p=new PrintWriter(new File(out,"unresolved.tsv"));PrintWriter t=new PrintWriter(new File(out,"tables.tsv"))) {p.println("source\tkind\ttarget");for(String row:unknown)p.println(row);t.println("dispatch\tindex\tentry\ttarget");for(String row:tableRows)t.println(row);}
  println("AUDIO_EXPORT entries="+done.size()+" owners="+owners.size()+" indirect="+unknown.size()+" recovered_tables="+overrides.size());
 }
}
