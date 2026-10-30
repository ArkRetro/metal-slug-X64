// Whole-program candidate pass. C output remains tentative and unverified.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import java.util.regex.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.pcode.JumpTable;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.*;
public class ExportProgramFrontier extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();File out=new File(args[1]);out.mkdirs();int limit=Integer.parseInt(args[2]);
  currentProgram.getMemory().getBlock(toAddr(0)).setExecute(true);
  for(long[] r:new long[][]{{0x42aa,0x12},{0x4438,0xe},{0x1a074,0x12},{0xc3152,8},{0xc32e4,16}})createData(toAddr(r[0]),new ArrayDataType(ByteDataType.dataType,(int)r[1],1));
  LinkedHashSet<Long> queue=new LinkedHashSet<>();
  try(BufferedReader in=new BufferedReader(new FileReader(args[0]))) {for(String s;(s=in.readLine())!=null;)queue.add(Long.parseLong(s,16));}
  Set<Long> done=new HashSet<>(),seen=new HashSet<>(),tableDone=new HashSet<>();List<String> tables=new ArrayList<>();Set<String> unknown=new TreeSet<>();
  Pattern state=Pattern.compile("move\\.l #0x([0-9a-f]+),\\((?:0x7c,)?A6\\)");
  Pattern indirect=Pattern.compile("jmp \\(0x([0-9a-f]+),PC,.*\\)");
  int round=0;
  while(done.size()<limit) {
   List<Long> batch=new ArrayList<>();for(Long pc:queue)if(!done.contains(pc))batch.add(pc);
   if(batch.isEmpty())break;
   for(Long pc:batch) {
    if(done.size()>=limit)break;done.add(pc);Address a=toAddr(pc);
    if(!disassemble(a) && getInstructionAt(a)==null)continue;
    if(getFunctionAt(a)==null && getFunctionContaining(a)==null)createFunction(a,"candidate_"+Long.toHexString(pc));
   }
   InstructionIterator all=currentProgram.getListing().getInstructions(true);
   while(all.hasNext()) {
    Instruction i=all.next();long at=i.getAddress().getOffset();if(at>=0x100000)continue;
    if(i.getFlowType().isComputed() && !tableDone.contains(at)) {
     Matcher m=indirect.matcher(i.toString());
     if(m.matches()) {
      long base=Long.parseLong(m.group(1),16),entry=base;ArrayList<Address> stubs=new ArrayList<>();List<Long> targets=new ArrayList<>();
      while(stubs.size()<256 && entry+4<=0x100000 && (currentProgram.getMemory().getShort(toAddr(entry))&65535)==0x6000) {
       long target=entry+2+currentProgram.getMemory().getShort(toAddr(entry+2));if(target<0x100 || target>=0x100000 || (target&1)!=0)break;
       stubs.add(toAddr(entry));targets.add(target);entry+=4;
      }
      Function owner=getFunctionContaining(i.getAddress());
      if(stubs.size()>=2 && stubs.size()<256 && owner!=null) {
       for(int n=0;n<stubs.size();n++) {Address stub=stubs.get(n);disassemble(stub);queue.add(targets.get(n));i.addOperandReference(0,stub,RefType.COMPUTED_JUMP,SourceType.ANALYSIS);tables.add(i.getAddress()+"\t"+n+"\t"+stub+"\t"+Long.toHexString(targets.get(n)));}
       new JumpTable(i.getAddress(),stubs,true).writeOverride(owner);CreateFunctionCmd.fixupFunctionBody(currentProgram,owner,monitor);tableDone.add(at);
      }
     }
    }
    if(!seen.add(at))continue;
    if(i.getFlowType().isCall())for(Address t:i.getFlows()) {long target=t.getOffset();if(target>=0x100 && target<0x100000 && (target&1)==0)queue.add(target);}
    Matcher m=state.matcher(i.toString());if(m.matches()) {long target=Long.parseLong(m.group(1),16);if(target>=0x100 && target<0x100000 && (target&1)==0)queue.add(target);}
   }
   println("DISCOVERY round="+(++round)+" examined="+done.size()+" queued="+queue.size()+" decoded_instructions="+seen.size());
  }
  List<Long> roots=new ArrayList<>(done);Collections.sort(roots);Set<Long> owners=new HashSet<>();
  DecompInterface d=new DecompInterface();d.openProgram(currentProgram);
  try(PrintWriter index=new PrintWriter(new File(out,"functions.tsv"));PrintWriter c=new PrintWriter(new File(out,"TENTATIVE_ROM_C.c"));PrintWriter asm=new PrintWriter(new File(out,"instructions.tsv"))) {
   index.println("seed\towner\tbody_bytes\tstatus");c.println("/* WHOLE-P1 CANDIDATE DECOMPILATION, NOT VERIFIED C OR A NATIVE PORT.\n * Some roots originate from byte signatures and may be data. All types, boundaries and indirect flows require review. */");
   int exported=0;
   for(Long pc:roots) {
    Function f=getFunctionAt(toAddr(pc));if(f==null)f=getFunctionContaining(toAddr(pc));
    if(f==null){index.println(Long.toHexString(pc)+"\t-\t0\tno_function");continue;}
    long owner=f.getEntryPoint().getOffset();if(!owners.add(owner)){index.println(Long.toHexString(pc)+"\t"+Long.toHexString(owner)+"\t"+f.getBody().getNumAddresses()+"\tshared_owner");continue;}
    DecompileResults r=d.decompileFunction(f,3,monitor);String text=r.decompileCompleted()?r.getDecompiledFunction().getC():"";
    boolean bad=text.contains("halt_baddata") || text.contains("Control flow encountered bad instruction") || text.contains("overlaps instruction");
    index.println(Long.toHexString(pc)+"\t"+Long.toHexString(owner)+"\t"+f.getBody().getNumAddresses()+"\t"+(!r.decompileCompleted()?"decompiler_failed":bad?"tentative_c_with_diagnostics":"tentative_c"));
    c.println("\n/* Candidate owner 0x"+Long.toHexString(owner)+"; "+(bad?"HAS BAD-INSTRUCTION DIAGNOSTICS":"UNVERIFIED")+". */");
    if(r.decompileCompleted())c.println(text);else c.println("/* Decompiler did not produce C. */");
    index.flush();c.flush();if(++exported%100==0)println("DECOMPILE exported_owners="+exported);
   }
   asm.println("address\tinstruction");InstructionIterator all=currentProgram.getListing().getInstructions(true);
   while(all.hasNext()) {
    Instruction i=all.next();asm.println(i.getAddress()+"\t"+i);
    if(i.getFlowType().isComputed() && !tableDone.contains(i.getAddress().getOffset()))unknown.add(i.getAddress()+"\tcomputed_flow\t"+i);
    if(i.getFlowType().isCall())for(Address t:i.getFlows())if(t.getOffset()>=0x100000 || (t.getOffset()&1)!=0)unknown.add(i.getAddress()+"\texternal_or_invalid_call\t"+t);
   }
  }finally{d.dispose();}
  try(PrintWriter pending=new PrintWriter(new File(out,"pending-seeds.txt"));PrintWriter u=new PrintWriter(new File(out,"unresolved.tsv"));PrintWriter t=new PrintWriter(new File(out,"trampolines.tsv"))) {
   for(Long pc:queue)if(!done.contains(pc))pending.println(Long.toHexString(pc));u.println("source\tkind\ttarget");for(String row:unknown)u.println(row);t.println("dispatch\tindex\tstub\ttarget");for(String row:tables)t.println(row);
  }
  println("EXPORT_END roots="+done.size()+" owners="+owners.size()+" decoded="+seen.size()+" unknown="+unknown.size());
 }
}
