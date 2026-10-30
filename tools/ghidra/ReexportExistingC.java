// Re-export stored function models without text substitutions or new analysis.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
public class ReexportExistingC extends GhidraScript {
 public void run() throws Exception {
  File out=new File(getScriptArgs()[0]);boolean audio=getScriptArgs()[1].equals("audio");
  LinkedHashSet<Long> owners=new LinkedHashSet<>();
  try(BufferedReader in=new BufferedReader(new FileReader(new File(out,"functions.tsv")))) {
   in.readLine();for(String line;(line=in.readLine())!=null;) {String[] f=line.split("\t");if(!f[1].equals("-") && !f[f.length-1].equals("shared_owner"))owners.add(Long.parseLong(f[1],16));}
  }
  DecompInterface d=new DecompInterface();d.openProgram(currentProgram);
  try(PrintWriter c=new PrintWriter(new File(out,audio?"TENTATIVE_AUDIO_C.c":"TENTATIVE_ROM_C.c"));PrintWriter status=new PrintWriter(new File(out,"reexport-status.tsv"))) {
   c.println("/* UNVERIFIED AUTOMATIC DECOMPILATION FROM SUPPLIED GAME ROM. No native-port or complete-source claim. */");status.println("owner\tstatus");int count=0;
   for(Long owner:owners) {
    Function f=getFunctionAt(toAddr(owner));if(f==null){status.println(Long.toHexString(owner)+"\tno_function");continue;}
    DecompileResults r=d.decompileFunction(f,10,monitor);String text=r.decompileCompleted()?r.getDecompiledFunction().getC():"";
    boolean bad=text.contains("halt_baddata") || text.contains("Control flow encountered bad instruction") || text.contains("overlaps instruction");
    String label=!r.decompileCompleted()?"decompiler_failed":bad?"tentative_c_with_diagnostics":"tentative_c";
    if(audio)c.println("\n/* Initial-map Z80 owner 0x"+Long.toHexString(owner)+". */");
    else c.println("\n/* Candidate owner 0x"+Long.toHexString(owner)+"; "+label+". */");
    if(r.decompileCompleted())c.println(text);else c.println("/* Decompiler did not produce C. */");
    status.println(Long.toHexString(owner)+"\t"+label);c.flush();status.flush();if(++count%100==0)println("REEXPORTED "+count);
   }
  }finally{d.dispose();}
 }
}
