// Targeted static recovery of the PC-indexed JSR at 01A5D6.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
public class RecoverIndexedCall extends GhidraScript {
 public void run() throws Exception {
  File out=new File(getScriptArgs()[0]);out.mkdirs();
  long base=0x1a5de;LinkedHashSet<Long> targets=new LinkedHashSet<>();
  try(PrintWriter table=new PrintWriter(new File(out,"table.tsv"))) {
   table.println("call_site\tindex\tstub\ttarget\tstatus");
   for(int n=0;n<256;n++) {
    long stub=base+4*n;
    if((getShort(toAddr(stub))&65535)!=0x6000)break;
    long target=stub+2+getShort(toAddr(stub+2));
    if(target<0x100 || target>=0x100000 || (target&1)!=0)throw new IOException("Invalid BRA.W target");
    disassemble(toAddr(stub));disassemble(toAddr(target));targets.add(target);
    table.printf("01a5d6\t%d\t%06x\t%06x\tstatic_BRA_W_not_runtime_verified%n",n,stub,target);
   }
  }
  for(long target:targets)if(getFunctionAt(toAddr(target))==null && getFunctionContaining(toAddr(target))==null)createFunction(toAddr(target),"table_handler_"+Long.toHexString(target));
  analyzeChanges(currentProgram);
  DecompInterface d=new DecompInterface();d.openProgram(currentProgram);
  try(PrintWriter index=new PrintWriter(new File(out,"functions.tsv"))) {
   index.println("entry\towner\tstatus\tfile");Set<Long> exported=new HashSet<>();
   for(long target:targets) {
    Function f=getFunctionAt(toAddr(target));if(f==null)f=getFunctionContaining(toAddr(target));
    if(f==null){index.printf("%06x\t-\tno_function\t-%n",target);continue;}
    long owner=f.getEntryPoint().getOffset();String name=String.format("rom_%06x.c",owner);
    if(!exported.add(owner)){index.printf("%06x\t%06x\tshared_owner\t%s%n",target,owner,name);continue;}
    DecompileResults r=d.decompileFunction(f,30,monitor);
    boolean available=r.decompileCompleted() && r.getDecompiledFunction()!=null;
    String text=available?r.getDecompiledFunction().getC():"";
    boolean diag=text.contains("halt_baddata") || text.contains("Control flow encountered bad instruction") || text.contains("overlaps instruction");
    String label=available?(diag?"tentative_c_with_diagnostics_and_missing_ccr":"tentative_c_missing_ccr_contract"):"decompiler_failed";
    index.printf("%06x\t%06x\t%s\t%s%n",target,owner,label,available?name:"-");
    if(available)try(PrintWriter c=new PrintWriter(new File(out,name))) {
     c.printf("/* UNVERIFIED AUTOMATIC C. Owner 0x%06x, reached from table at 0x01A5DE.\n * UNSAFE: observed callers consume CCR.C, which this automatic C does not expose.\n * Not a usable translation; ABI, types, boundaries and runtime index range unverified. */%n",owner);
     c.println(text);
    }
   }
  }finally{d.dispose();}
 }
}
