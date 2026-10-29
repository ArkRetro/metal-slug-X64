// Addressed analysis of the captured Marco controller and shared animation VM.
// The language is an MC68020 superset; executed 68000 probes remain authoritative.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
public class AnalyzeMarcoAnimations extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();File capture=new File(args[0]),out=new File(args[1]);out.mkdirs();
  try(InputStream in=new FileInputStream(new File(capture,"program-bank.bin"))) {
   MemoryBlock b=currentProgram.getMemory().createInitializedBlock("OBSERVED_BANK",toAddr(0x200000),in,0x100000,monitor,false);
   b.setWrite(false);b.setExecute(false);
  }
  TreeSet<Long> seeds=new TreeSet<>();
  for(long pc=0x42c0;pc<=0x42f0;pc+=4)seeds.add(pc);
  for(long pc:new long[]{0x4256,0x427a,0x435c,0x4412,0x4454,0x4470,0x44da,0xa03e})seeds.add(pc);
  if(args.length>2)seeds.clear();
  File seedFile=args.length>2?new File(args[2]):new File(capture,"routine-seeds.txt");
  try(BufferedReader reader=new BufferedReader(new FileReader(seedFile))) {
   for(String line;(line=reader.readLine())!=null;)seeds.add(Long.parseLong(line.trim(),16));
  }
  Set<Long> decompiled=new HashSet<>();
  DecompInterface decompiler=new DecompInterface();decompiler.openProgram(currentProgram);
  try(PrintWriter ins=new PrintWriter(new File(out,"instructions.tsv"));PrintWriter refs=new PrintWriter(new File(out,"references.tsv"))) {
   ins.println("seed\taddress\tinstruction");refs.println("source\tdestination\ttype");
   for(long pc:seeds) {
    Address a=toAddr(pc);disassemble(a);Function f=getFunctionAt(a);if(f==null)f=getFunctionContaining(a);if(f==null)f=createFunction(a,"observed_"+Long.toHexString(pc));
    InstructionIterator iter=currentProgram.getListing().getInstructions(a,true);
    for(int n=0;n<600 && iter.hasNext();n++) {
     Instruction i=iter.next();if(i.getAddress().getOffset()>pc+0x1000)break;
     ins.println(Long.toHexString(pc)+"\t"+i.getAddress()+"\t"+i);
     for(ghidra.program.model.symbol.Reference r:i.getReferencesFrom())refs.println(i.getAddress()+"\t"+r.getToAddress()+"\t"+r.getReferenceType());
    }
    if(f!=null && decompiled.add(f.getEntryPoint().getOffset()))try(PrintWriter c=new PrintWriter(new File(out,"TENTATIVE_"+Long.toHexString(pc)+".c"))) {
     c.println("/* TENTATIVE types/control flow; compare with executed M68000 instructions. */");
     DecompileResults result=decompiler.decompileFunction(f,20,monitor);if(result.decompileCompleted())c.println(result.getDecompiledFunction().getC());
    }
   }
  }finally{decompiler.dispose();}
 }
}
