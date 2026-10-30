//@category MetalSlugX
import java.io.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
public class ExportSingleC extends GhidraScript {
 public void run() throws Exception {
  String[] a=getScriptArgs();Function f=getFunctionAt(toAddr(Long.parseLong(a[0],16)));if(f==null)throw new IOException("No function at requested address");
  DecompInterface d=new DecompInterface();d.toggleSyntaxTree(false);if(a.length>2)d.setSimplificationStyle(a[2]);d.openProgram(currentProgram);
  try(PrintWriter c=new PrintWriter(new File(a[1]+".c"));PrintWriter report=new PrintWriter(new File(a[1]+".txt"))) {
   DecompileResults r=d.decompileFunction(f,a.length>2?30:120,monitor);
   report.println("complete="+r.decompileCompleted());report.println("error="+r.getErrorMessage());
   boolean available=r.decompileCompleted() && r.getDecompiledFunction()!=null;
   report.println("c_available="+available);
   if(available)c.println(r.getDecompiledFunction().getC());
  }finally{d.dispose();}
 }
}
