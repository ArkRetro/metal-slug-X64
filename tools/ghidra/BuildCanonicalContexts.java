// Seed verified entry contexts; all generated function/flow models remain tentative.
//@category MetalSlugX
import java.io.*;
import java.util.*;
import java.util.regex.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
public class BuildCanonicalContexts extends GhidraScript {
 private String id(Address address) {
  MemoryBlock b=currentProgram.getMemory().getBlock(address);
  String context=b==null?(address.getOffset()>=0xc00000 && address.getOffset()<0xc20000?"PLATFORM_MAIN_BIOS":"UNMAPPED_"+address.getAddressSpace().getName()):b.getName();
  return context+":"+String.format("%08x",address.getOffset());
 }
 public void run() throws Exception {
  String[] args=getScriptArgs();File input=new File(args[0]),out=new File(args[1]);String selected=args[2];out.mkdirs();
  ArrayList<String[]> jobs=new ArrayList<>();
  try(BufferedReader reader=new BufferedReader(new FileReader(new File(input,"contexts.tsv")))) {
   reader.readLine();for(String line;(line=reader.readLine())!=null;)jobs.add(line.split("\t"));
  }
  boolean main=selected.equals("M68K_FIXED_P1");String[] chosen=null;
  for(String[] job:jobs)if(job[0].equals(selected))chosen=job;
  if(chosen==null)throw new IOException("No context "+selected);
  MemoryBlock imported=currentProgram.getMemory().getBlock(toAddr(0));imported.setName(selected);imported.setRead(true);imported.setWrite(false);imported.setExecute(chosen[5].equals("1"));
  Map<String,MemoryBlock> blocks=new LinkedHashMap<>();blocks.put(selected,imported);
  if(main) {
   MemoryBlock ram=currentProgram.getMemory().createUninitializedBlock("M68K_RAM_UNVERSIONED",toAddr(0x100000),0x10000,false);ram.setRead(true);ram.setWrite(true);ram.setExecute(false);
   MemoryBlock bank=currentProgram.getMemory().createUninitializedBlock("M68K_BANK_UNSELECTED",toAddr(0x200000),0x100000,false);bank.setRead(true);bank.setWrite(false);bank.setExecute(false);
   for(String[] job:jobs) {
    if(!job[1].equals("m68000") || job[0].equals(selected))continue;
    try(InputStream stream=new FileInputStream(new File(input,job[4]))) {
     MemoryBlock b=currentProgram.getMemory().createInitializedBlock(job[0],toAddr(Long.parseLong(job[2],16)),stream,Long.parseLong(job[3],16),monitor,true);
     b.setRead(true);b.setWrite(job[0].startsWith("RAM_COPY_"));b.setExecute(job[5].equals("1"));blocks.put(job[0],b);
    }
   }
   for(long[] r:new long[][]{{0,0x100},{0x42aa,0x12},{0x4438,0xe},{0x1a074,0x12},{0xc3152,8},{0xc32e4,16}})
    createData(toAddr(r[0]),new ArrayDataType(ByteDataType.dataType,(int)r[1],1));
  }else {
   MemoryBlock ram=currentProgram.getMemory().createUninitializedBlock("Z80_RAM_UNVERSIONED",toAddr(0xf800),0x800,false);ram.setRead(true);ram.setWrite(true);ram.setExecute(false);
  }
  List<Address> rootAddresses=new ArrayList<>();List<String> rootContexts=new ArrayList<>();
  for(String[] job:jobs) {
   if((main?!job[1].equals("m68000"):!job[0].equals(selected)) || job[6].equals("-"))continue;
   MemoryBlock block=blocks.get(job[0]);if(block==null)throw new IOException("Missing block "+job[0]);
   for(String raw:job[6].split(",")) {
    long pc=Long.parseLong(raw,16);Address address=block.getStart().add(pc-block.getStart().getOffset());
    disassemble(address);Function f=getFunctionAt(address);if(f==null && getFunctionContaining(address)==null)createFunction(address,"root_"+job[0]+"_"+raw);
    rootAddresses.add(address);rootContexts.add(job[0]);
   }
  }
  if(!rootAddresses.isEmpty())analyzeChanges(currentProgram);
  List<String> runtimeSeeds=new ArrayList<>();
  File runtimeSeedFile=new File(input,"runtime-seeds.tsv");
  if(main && runtimeSeedFile.isFile())try(BufferedReader reader=new BufferedReader(new FileReader(runtimeSeedFile))) {
   reader.readLine();for(String line;(line=reader.readLine())!=null;) {
    String[] seed=line.split("\t");
    if(!seed[0].equals("M68K_FIXED_P1") || !seed[2].equals("4e75"))throw new IOException("Unsupported runtime leaf seed");
    Address address=toAddr(Long.parseLong(seed[1],16));
    if(!HexFormat.of().formatHex(getBytes(address,2)).equals(seed[2]))throw new IOException("Runtime seed bytes changed");
    disassemble(address);if(getFunctionContaining(address)==null)createFunction(address,"observed_idle_"+seed[1]);
    runtimeSeeds.add(line);
   }
  }
  try(PrintWriter extra=new PrintWriter(new File(out,"runtime-seeds.tsv"))) {
   extra.println("context\tpc\topcode\tevidence_file\tevidence_sha256\tstatus");for(String seed:runtimeSeeds)extra.println(seed);
  }
  // 100818 reads (A4) into A0 and 10081A calls A0 in this initial RAM image.
  // Follow in-container constant stores to that descriptor slot as candidates.
  // This does not close the indirect domain or prove those stores execute.
  List<String> seeds=new ArrayList<>(),allocatorSeeds=new ArrayList<>();Set<String> seededSites=new HashSet<>();
  if(main) {
   MemoryBlock ramCopy=blocks.get("RAM_COPY_100000");
   Address call=ramCopy.getStart().add(0x81a);
   if(getInstructionAt(call)==null || !getInstructionAt(call).toString().equals("jsr (A0)"))throw new IOException("RAM descriptor call site changed");
   Pattern store=Pattern.compile("move\\.l #0x([0-9a-f]+),\\(A4\\)");
   Pattern argument=Pattern.compile("lea \\(0x([0-9a-f]+)\\)\\.l,A0");
   Address callbackStore=ramCopy.getStart().add(0x8be);
   for(int pass=0;pass<ramCopy.getSize()/2;pass++) {
    List<Address> pending=new ArrayList<>();
    InstructionIterator existing=currentProgram.getListing().getInstructions(ramCopy.getStart(),true);
    while(existing.hasNext()) {
     Instruction i=existing.next();if(!ramCopy.contains(i.getAddress()))break;
     Matcher match=store.matcher(i.toString());boolean direct=match.matches();
     Instruction next=null;
     if(!direct) {
      match=argument.matcher(i.toString());if(!match.matches())continue;
      next=getInstructionAt(i.getAddress().add(i.getLength()));
      if(next==null || !next.toString().equals("bsr.w 0x0010082e"))continue;
      if(getInstructionAt(callbackStore)==null || !getInstructionAt(callbackStore).toString().equals("move.l A0,(A1)"))throw new IOException("Allocator callback store changed");
     }
     long pc=Long.parseLong(match.group(1),16);
     if(pc<ramCopy.getStart().getOffset() || pc>ramCopy.getEnd().getOffset() || pc%2!=0)continue;
     Address target=ramCopy.getStart().add(pc-ramCopy.getStart().getOffset());
     if(!seededSites.add(id(i.getAddress())))continue;
     if(direct)seeds.add(id(i.getAddress())+"\t"+id(target)+"\t"+pass+"\tconstant_descriptor_slot_store_candidate");
     else allocatorSeeds.add(id(i.getAddress())+"\t"+id(next.getAddress())+"\t"+id(callbackStore)+"\t"+id(target)+"\t"+pass+"\tconstant_A0_allocator_callback_candidate");
     pending.add(target);
    }
    if(pending.isEmpty())break;
    for(Address target:pending) {
     disassemble(target);
     if(getFunctionContaining(target)==null)createFunction(target,"candidate_descriptor_"+String.format("%06x",target.getOffset()));
    }
    analyzeChanges(currentProgram);
   }
  }
  try(PrintWriter extra=new PrintWriter(new File(out,"descriptor-seeds.tsv"))) {
   extra.println("store_site\ttarget\tanalysis_pass\tstatus");for(String seed:seeds)extra.println(seed);
  }
  try(PrintWriter extra=new PrintWriter(new File(out,"allocator-seeds.tsv"))) {
   extra.println("load_site\tcall_site\tcallback_store_site\ttarget\tanalysis_pass\tstatus");for(String seed:allocatorSeeds)extra.println(seed);
  }
  try(PrintWriter memory=new PrintWriter(new File(out,"memory.tsv"));PrintWriter roots=new PrintWriter(new File(out,"roots.tsv"));PrintWriter functions=new PrintWriter(new File(out,"functions.tsv"));PrintWriter edges=new PrintWriter(new File(out,"edges.tsv"));PrintWriter code=new PrintWriter(new File(out,"instructions.tsv"))) {
   memory.println("context\taddress_space\tstart\tend_inclusive\tinitialized\twritable\texecutable");
   for(MemoryBlock b:currentProgram.getMemory().getBlocks())memory.printf("%s\t%s\t%08x\t%08x\t%b\t%b\t%b%n",b.getName(),b.getStart().getAddressSpace().getName(),b.getStart().getOffset(),b.getEnd().getOffset(),b.isInitialized(),b.isWrite(),b.isExecute());
   roots.println("context\tpc\tlocation\towner\tinstruction_present");
   for(int n=0;n<rootAddresses.size();n++){Address a=rootAddresses.get(n);Function f=getFunctionContaining(a);roots.printf("%s\t%08x\t%s\t%s\t%b%n",rootContexts.get(n),a.getOffset(),id(a),f==null?"-":id(f.getEntryPoint()),getInstructionAt(a)!=null);}
   functions.println("entry\tname\tmodel_body_bytes\tinstruction_count");edges.println("owner\tsite\ttarget\ttarget_owner\tkind");code.println("owner\tlocation\tlength\tinstruction");
   FunctionIterator all=currentProgram.getFunctionManager().getFunctions(true);
   while(all.hasNext()) {
    Function f=all.next();if(f.isExternal())continue;MemoryBlock b=currentProgram.getMemory().getBlock(f.getEntryPoint());if(b==null || !b.isInitialized())continue;
    String owner=id(f.getEntryPoint());int count=0;InstructionIterator instructions=currentProgram.getListing().getInstructions(f.getBody(),true);
    while(instructions.hasNext()) {
     Instruction i=instructions.next();count++;code.printf("%s\t%s\t%d\t%s%n",owner,id(i.getAddress()),i.getLength(),i);
     boolean call=i.getFlowType().isCall(),jump=i.getFlowType().isJump();
     if(i.getFlowType().isComputed())edges.printf("%s\t%s\t-\t-\tcomputed_%s%n",owner,id(i.getAddress()),call?"call":"jump");
     if(!call && !jump)continue;
     for(Address target:i.getFlows()) {
      Function to=getFunctionContaining(target);if(!call && to!=null && to.getEntryPoint().equals(f.getEntryPoint()))continue;
      edges.printf("%s\t%s\t%s\t%s\t%s%n",owner,id(i.getAddress()),id(target),to==null?"-":id(to.getEntryPoint()),call?"call":"tail_jump");
     }
    }
    functions.printf("%s\t%s\t%d\t%d%n",owner,f.getName(),f.getBody().getNumAddresses(),count);
   }
   // A discovered pointer store can be in an unowned listing fragment. Keep
   // its instruction as evidence without inventing a containing function.
   Set<String> evidenceSites=new LinkedHashSet<>();
   for(String seed:seeds)evidenceSites.add(seed.split("\t")[0]);
   for(String seed:allocatorSeeds){String[] parts=seed.split("\t");evidenceSites.add(parts[0]);evidenceSites.add(parts[1]);}
   if(main)for(String site:evidenceSites) {
    long pc=Long.parseLong(site.substring(site.indexOf(':')+1),16);
    Address address=blocks.get("RAM_COPY_100000").getStart().add(pc-0x100000);
    if(getFunctionContaining(address)!=null)continue;
    Instruction i=getInstructionAt(address);if(i==null)throw new IOException("Pointer store vanished: "+site);
    code.printf("-\t%s\t%d\t%s%n",site,i.getLength(),i);
   }
  }
 }
}
