// Export a conservative fixed-ROM frontier. Output is TENTATIVE, not a native port.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import java.util.regex.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.pcode.JumpTable;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;
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
  Set<String> unknown=new TreeSet<>();Set<Long> recoveredTables=new HashSet<>();
  List<String> tableRows=new ArrayList<>();
  Long constructorTarget=null;long constructorLoad=0;
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
     Instruction i=all.next();long at=i.getAddress().getOffset();if(at>=0x100000)continue;
     boolean fresh=inspected.add(at);
     if(fresh)asm.println(i.getAddress()+"\t"+i);
     // 0040BC/00410E receive the actor's initial code address in A0.
     // Do not treat pointers passed to animation helpers as code.
     if(i.getNumOperands()>1 && i.getDefaultOperandRepresentation(i.getNumOperands()-1).equals("A0")) {
      constructorTarget=null;constructorLoad=at;
      for(Reference r:i.getReferencesFrom())if(r.getOperandIndex()==0) {
       long t=r.getToAddress().getOffset();if(t>=0x100 && t<0x100000 && (t&1)==0)constructorTarget=t;
      }
      Matcher literal=Pattern.compile("movea\\.l #0x([0-9a-f]+),A0").matcher(i.toString());
      if(literal.matches())constructorTarget=Long.parseLong(literal.group(1),16);
     }
     if(i.getFlowType().isCall()) {
      boolean allocator=false;
      for(Address t:i.getFlows())if(t.getOffset()==0x40bc || t.getOffset()==0x410e)allocator=true;
      if(allocator && constructorTarget!=null && at>=constructorLoad && at-constructorLoad<=64 && constructorTarget>=0x100 && constructorTarget<0x100000 && (constructorTarget&1)==0)queue.add(constructorTarget);
      constructorTarget=null;
     }
     if(i.getFlowType().isComputed() && !recoveredTables.contains(at)) {
      Matcher table=Pattern.compile("jmp \\(0x([0-9a-f]+),PC,.*\\)").matcher(i.toString());
      if(table.matches()) {
       long base=Long.parseLong(table.group(1),16);ArrayList<Address> destinations=new ArrayList<>();ArrayList<Long> targets=new ArrayList<>();
       long entry=base;
       while(destinations.size()<256 && entry+4<=0x100000 && (currentProgram.getMemory().getShort(toAddr(entry))&65535)==0x6000) {
        long target=entry+2+currentProgram.getMemory().getShort(toAddr(entry+2));
        if(target<0x100 || target>=0x100000 || (target&1)!=0)break;
        destinations.add(toAddr(entry));targets.add(target);entry+=4;
       }
       Function owner=getFunctionContaining(i.getAddress());
       if(destinations.size()>=2 && destinations.size()<256 && owner!=null) {
        for(int k=0;k<destinations.size();k++) {
         Address stub=destinations.get(k);disassemble(stub);queue.add(targets.get(k));
         i.addOperandReference(0,stub,RefType.COMPUTED_JUMP,SourceType.ANALYSIS);
         tableRows.add(i.getAddress()+"\t"+k+"\t"+stub+"\t"+Long.toHexString(targets.get(k)));
        }
        new JumpTable(i.getAddress(),destinations,true).writeOverride(owner);
        CreateFunctionCmd.fixupFunctionBody(currentProgram,owner,monitor);
        recoveredTables.add(at);unknown.remove(i.getAddress()+"\tcomputed_flow\t"+i);
       }
      }
     }
     if(!fresh)continue;
     if(i.getFlowType().isCall()) {
      Address[] flow=i.getFlows();if(flow.length==0)unknown.add(i.getAddress()+"\tcomputed_call\t"+i);
      for(Address target:flow) {long t=target.getOffset();if(t>=0x100 && t<0x100000 && (t&1)==0)queue.add(t);else unknown.add(i.getAddress()+"\texternal_call\t"+target);}
     } else if(i.getFlowType().isComputed() && !recoveredTables.contains(at))unknown.add(i.getAddress()+"\tcomputed_flow\t"+i);
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
    if(result.decompileCompleted())c.println(result.getDecompiledFunction().getC());else c.println("/* No C: "+result.getErrorMessage().replace("*/","* /")+" */");
    index.flush();asm.flush();c.flush();
    if(processed.size()%50==0)println("FRONTIER processed="+processed.size()+" discovered="+queue.size()+" owners="+owners.size());
   }
   decompiler.flushCache();owners.clear();
   try(PrintWriter finalIndex=new PrintWriter(new File(out,"functions-final.tsv"));PrintWriter finalC=new PrintWriter(new File(out,"TENTATIVE_ROM_C-final.c"))) {
    finalIndex.println("seed\towner\tbody_bytes\tstatus");
    finalC.println("/* AUTOMATIC TENTATIVE GHIDRA C after direct-flow and BRA.W trampoline recovery. Not a native port; unverified types, register state and indirect calls remain. */");
    for(Long pc:processed) {
     Function function=getFunctionAt(toAddr(pc));if(function==null)function=getFunctionContaining(toAddr(pc));
     if(function==null){finalIndex.println(Long.toHexString(pc)+"\t-\t0\tno_function");continue;}
     long entry=function.getEntryPoint().getOffset();
     if(!owners.add(entry)){finalIndex.println(Long.toHexString(pc)+"\t"+Long.toHexString(entry)+"\t"+function.getBody().getNumAddresses()+"\tshared_owner");continue;}
     DecompileResults result=decompiler.decompileFunction(function,3,monitor);
     finalIndex.println(Long.toHexString(pc)+"\t"+Long.toHexString(entry)+"\t"+function.getBody().getNumAddresses()+"\t"+(result.decompileCompleted()?"tentative_c":"decompiler_failed"));
     finalC.println("\n/* Owner 0x"+Long.toHexString(entry)+". */");
     if(result.decompileCompleted())finalC.println(result.getDecompiledFunction().getC());
    }
   }
  }finally{decompiler.dispose();}
  java.nio.file.Files.move(new File(out,"functions-final.tsv").toPath(),new File(out,"functions.tsv").toPath(),java.nio.file.StandardCopyOption.REPLACE_EXISTING);
  java.nio.file.Files.move(new File(out,"TENTATIVE_ROM_C-final.c").toPath(),new File(out,"TENTATIVE_ROM_C.c").toPath(),java.nio.file.StandardCopyOption.REPLACE_EXISTING);
  try(PrintWriter tables=new PrintWriter(new File(out,"trampolines.tsv"))) {tables.println("dispatch\tindex\tstub\ttarget");for(String row:tableRows)tables.println(row);}

  try(PrintWriter pending=new PrintWriter(new File(out,"pending-seeds.txt"));PrintWriter unresolved=new PrintWriter(new File(out,"unresolved.tsv"))) {
   for(Long pc:queue)if(!processed.contains(pc))pending.println(Long.toHexString(pc));
   unresolved.println("source\tkind\ttarget_or_instruction");for(String row:unknown)unresolved.println(row);
  }
  println("EXPORTED processed="+processed.size()+" discovered="+queue.size()+" tentative_owners="+owners.size()+" unresolved="+unknown.size());
 }
}
