// Export whole stored Ghidra model for dependency-led recovery; no new analysis.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import java.util.regex.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.lang.Register;
import ghidra.program.model.pcode.*;
public class ExportProgramGraph extends GhidraScript {
 private String reg(Varnode v) {
  if(v==null || !v.isRegister())return null;
  Register r=currentProgram.getRegister(v.getAddress(),v.getSize());
  return r==null?v.getAddress().toString():r.getBaseRegister().getName();
 }
 public void run() throws Exception {
  File out=new File(getScriptArgs()[0]);out.mkdirs();
  try(PrintWriter models=new PrintWriter(new File(out,"function-models.tsv"));
      PrintWriter ranges=new PrintWriter(new File(out,"model-ranges.tsv"));
      PrintWriter edges=new PrintWriter(new File(out,"model-edges.tsv"));
      PrintWriter returns=new PrintWriter(new File(out,"return-consumers.tsv"));
      PrintWriter isa=new PrintWriter(new File(out,"isa-review.tsv"))) {
   models.println("entry\tname\tbody_bytes\tranges\tdirect_calls\ttail_jumps\tcomputed_sites\tregister_reads\tregister_writes");
   ranges.println("owner\tstart\tend_inclusive");edges.println("owner\tsite\ttarget\ttarget_owner\tkind");
   returns.println("owner\tcall_site\tnext\tmnemonic\tregister_reads");
   isa.println("owner\tsite\tinstruction\tstatus");
   Pattern state=Pattern.compile("move\\.l #0x([0-9a-f]+),\\((?:0x7c,)?A6\\)");
   Set<String> laterIsa=new HashSet<>(Arrays.asList("bfchg","bfclr","bfexts","bfextu","bfffo","bfins","bfset","bftst","callm","cas","cas2","chk2","cmp2","extb","pack","unpk","rtd","rtm","movec","moves"));
   FunctionIterator functions=currentProgram.getFunctionManager().getFunctions(true);
   while(functions.hasNext()) {
    Function f=functions.next();long entry=f.getEntryPoint().getOffset();if(entry>=0x100000)continue;
    int rangeCount=0,calls=0,tails=0,computed=0;Set<String> reads=new TreeSet<>(),writes=new TreeSet<>();
    AddressRangeIterator ri=f.getBody().getAddressRanges();
    while(ri.hasNext()){AddressRange r=ri.next();rangeCount++;ranges.printf("%06x\t%06x\t%06x%n",entry,r.getMinAddress().getOffset(),r.getMaxAddress().getOffset());}
    InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
    while(ii.hasNext()) {
     Instruction i=ii.next();
     for(PcodeOp op:i.getPcode()) {
      for(Varnode input:op.getInputs()){String name=reg(input);if(name!=null)reads.add(name);}
      String name=reg(op.getOutput());if(name!=null)writes.add(name);
     }
     String mnemonic=i.getMnemonicString().toLowerCase().split("\\.")[0];
     if(laterIsa.contains(mnemonic))isa.printf("%06x\t%06x\t%s\tcanonical_M68000_review_required%n",entry,i.getAddress().getOffset(),i);
     Matcher match=state.matcher(i.toString());
     if(match.matches()) {
      long target=Long.parseLong(match.group(1),16);
      if(target>=0x100 && target<0x100000 && (target&1)==0) {
       Function owner=currentProgram.getFunctionManager().getFunctionContaining(toAddr(target));
       edges.printf("%06x\t%06x\t%06x\t%s\tstate_assignment_candidate%n",entry,i.getAddress().getOffset(),target,owner==null?"-":String.format("%06x",owner.getEntryPoint().getOffset()));
      }
     }
     boolean isCall=i.getFlowType().isCall(),isJump=i.getFlowType().isJump();
     if(isCall && i.getFallThrough()!=null) {
      Instruction next=getInstructionAt(i.getFallThrough());
      if(next!=null) {
       Set<String> nextReads=new TreeSet<>();
       for(PcodeOp op:next.getPcode())for(Varnode input:op.getInputs()){String name=reg(input);if(name!=null)nextReads.add(name);}
       returns.printf("%06x\t%06x\t%06x\t%s\t%s%n",entry,i.getAddress().getOffset(),next.getAddress().getOffset(),next.getMnemonicString(),String.join(",",nextReads));
      }
     }
     if(i.getFlowType().isComputed()){computed++;edges.printf("%06x\t%06x\t-\t-\tcomputed_%s%n",entry,i.getAddress().getOffset(),isCall?"call":"jump");}
     if(!isCall && !isJump)continue;
     for(Address target:i.getFlows()) {
      Function owner=currentProgram.getFunctionManager().getFunctionContaining(target);
      if(!isCall && owner==f)continue;
      if(isCall)calls++;else tails++;
      edges.printf("%06x\t%06x\t%06x\t%s\t%s%n",entry,i.getAddress().getOffset(),target.getOffset(),owner==null?"-":String.format("%06x",owner.getEntryPoint().getOffset()),isCall?"call":"tail_jump");
     }
    }
    models.printf("%06x\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%s%n",entry,f.getName(),f.getBody().getNumAddresses(),rangeCount,calls,tails,computed,String.join(",",reads),String.join(",",writes));
   }
  }
 }
}
