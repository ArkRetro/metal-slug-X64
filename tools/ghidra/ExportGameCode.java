// Export a conservative fixed-ROM frontier. Output is TENTATIVE, not a native port.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import java.util.regex.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.*;
import ghidra.program.model.symbol.Reference;
public class ExportGameCode extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();File seedsFile=new File(args[0]),out=new File(args[1]);out.mkdirs();
  int limit=Integer.parseInt(args[2]);
  currentProgram.getMemory().getBlock(toAddr(0)).setExecute(true);
  // Known diagnostic strings and inline tables must not become invented code.
  for(long[] r:new long[][]{{0x42aa,0x12},{0x4438,0xe},{0x1a074,0x12},{0xc3152,8},{0xc32e4,16}})
   createData(toAddr(r[0]),new ArrayDataType(ByteDataType.dataType,(int)r[1],1));
  LinkedHashSet<Long> queue=new LinkedHashSet<>();
  try(BufferedReader in=new BufferedReader(new FileReader(seedsFile))) {
   for(String s;(s=in.readLine())!=null;)queue.add(Long.parseLong(s.trim(),16));
  }
  Set<Long> processed=new HashSet<>(),inspected=new HashSet<>(),owners=new HashSet<>();
  Set<String> unknown=new TreeSet<>();
  Pattern state=Pattern.compile("move\\.l #0x([0-9a-f]+),\\((?:0x7c,)?A6\\)");
  DecompInterface decompiler=new DecompInterface();decompiler.openProgram(currentProgram);
  try(PrintWriter index=new PrintWriter(new File(out,"functions.tsv"));PrintWriter asm=new PrintWriter(new File(out,"instructions.tsv"));PrintWriter c=new PrintWriter(new File(out,"TENTATIVE_ROM_C.c"))) {
   index.println("seed\towner\tbody_bytes\tstatus");asm.println("address\tinstruction");
   c.println("/* AUTOMATIC TENTATIVE GHIDRA C. Types, boundaries and indirect flow are unverified.\n * NOT buildable native game logic. MC68020-superset analysis of supplied M68000 ROM. */");
   while(processed.size()<limit) {
    Long pc=null;for(Long q:queue)if(!processed.contains(q)){pc=q;break;}if(pc==null)break;
    processed.add(pc);Address a=toAddr(pc);
    if(!disassemble(a) && getInstructionAt(a)==null){index.println(Long.toHexString(pc)+"\t-\t0\tdisassembly_rejected");continue;}
    Function f=getFunctionAt(a);if(f==null)f=getFunctionContaining(a);if(f==null)f=createFunction(a,"candidate_"+Long.toHexString(pc));
    // Inspect newly decoded instructions, including callees reached by disassembly.
    InstructionIterator all=currentProgram.getListing().getInstructions(true);
    while(all.hasNext()) {
     Instruction i=all.next();long at=i.getAddress().getOffset();if(at>=0x100000 || !inspected.add(at))continue;
     asm.println(i.getAddress()+"\t"+i);
     if(i.getFlowType().isCall()) {
      Address[] flow=i.getFlows();if(flow.length==0)unknown.add(i.getAddress()+"\tcomputed_call\t"+i);
      for(Address target:flow) {long t=target.getOffset();if(t>=0x100 && t<0x100000 && (t&1)==0)queue.add(t);else unknown.add(i.getAddress()+"\texternal_call\t"+target);}
     } else if(i.getFlowType().isComputed())unknown.add(i.getAddress()+"\tcomputed_flow\t"+i);
     Matcher m=state.matcher(i.toString());if(m.matches()) {
      long t=Long.parseLong(m.group(1),16);if(t>=0x100 && t<0x100000 && (t&1)==0)queue.add(t);
     }
    }
    if(f==null){index.println(Long.toHexString(pc)+"\t-\t0\tno_function");continue;}
    long owner=f.getEntryPoint().getOffset();
    if(!owners.add(owner)){index.println(Long.toHexString(pc)+"\t"+Long.toHexString(owner)+"\t"+f.getBody().getNumAddresses()+"\tshared_owner");continue;}
    DecompileResults result=decompiler.decompileFunction(f,3,monitor);
    index.println(Long.toHexString(pc)+"\t"+Long.toHexString(owner)+"\t"+f.getBody().getNumAddresses()+"\t"+(result.decompileCompleted()?"tentative_c":"decompiler_failed"));
    c.println("\n/* Candidate root 0x"+Long.toHexString(pc)+", Ghidra owner 0x"+Long.toHexString(owner)+". */");
    if(result.decompileCompleted())c.println(result.getDecompiledFunction().getC().replaceAll("(?m)[ \t]+$", ""));else c.println("/* No C: "+result.getErrorMessage().replace("*/","* /")+" */");
    index.flush();asm.flush();c.flush();
    if(processed.size()%50==0)println("FRONTIER processed="+processed.size()+" discovered="+queue.size()+" owners="+owners.size());
   }
  }finally{decompiler.dispose();}
  try(PrintWriter pending=new PrintWriter(new File(out,"pending-seeds.txt"));PrintWriter unresolved=new PrintWriter(new File(out,"unresolved.tsv"))) {
   for(Long pc:queue)if(!processed.contains(pc))pending.println(Long.toHexString(pc));
   unresolved.println("source\tkind\ttarget_or_instruction");for(String row:unknown)unresolved.println(row);
  }
  println("EXPORTED processed="+processed.size()+" discovered="+queue.size()+" tentative_owners="+owners.size()+" unresolved="+unknown.size());
 }
}
