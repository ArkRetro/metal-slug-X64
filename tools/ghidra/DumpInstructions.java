//@category MetalSlugX
import java.io.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
public class DumpInstructions extends GhidraScript {
 public void run() throws Exception {
  try(PrintWriter p=new PrintWriter(new File(getScriptArgs()[0]))) {
   InstructionIterator it=currentProgram.getListing().getInstructions(true);p.println("address\tinstruction");
   while(it.hasNext()){Instruction i=it.next();p.println(i.getAddress()+"\t"+i);}
  }
 }
}
