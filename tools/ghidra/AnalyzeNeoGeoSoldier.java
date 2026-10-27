// Build a private, observed Neo Geo memory view and export addressed analysis.
// Decompiler output is explicitly tentative, never a verified native port.
//@category MetalSlugX
import java.io.*;
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.data.*;

public class AnalyzeNeoGeoSoldier extends GhidraScript {
    private void loaded(String name,long address,File file,boolean writable,boolean executable) throws Exception {
        try (InputStream in=new FileInputStream(file)) {
            MemoryBlock block=currentProgram.getMemory().createInitializedBlock(name,toAddr(address),in,file.length(),monitor,false);
            block.setRead(true);block.setWrite(writable);block.setExecute(executable);
        }
    }
    private void io(String name,long address,long length) throws Exception {
        MemoryBlock block=currentProgram.getMemory().createUninitializedBlock(name,toAddr(address),length,false);
        block.setRead(true);block.setWrite(true);block.setExecute(false);block.setVolatile(true);
    }
    @Override public void run() throws Exception {
        String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("capture-directory output-directory");
        File captures=new File(args[0]),out=new File(args[1]);out.mkdirs();
        MemoryBlock fixed=currentProgram.getMemory().getBlock(toAddr(0));
        fixed.setName("OBSERVED_CART_FIXED");fixed.setRead(true);fixed.setWrite(false);fixed.setExecute(true);
        loaded("OBSERVED_WORK_RAM_FRAME2360",0x100000,new File(captures,"ram2360.bin"),true,false);
        loaded("OBSERVED_BANK_WINDOW_FRAME2360",0x200000,new File(captures,"program-bank.bin"),false,true);
        loaded("OBSERVED_BIOS_FRAME2360",0xc00000,new File(captures,"bios.bin"),false,true);
        loaded("OBSERVED_PALETTE_FRAME2360",0x400000,new File(captures,"palette2360.bin"),true,false);
        // Base addresses and extents follow the MAME 0.276 Neo Geo driver;
        // mirrored aliases and cartridge protection are not flattened into ROM.
        File physical=new File(captures,"rom-physical-bank1.bin");
        if(physical.isFile())try(InputStream in=new FileInputStream(physical)) {
            MemoryBlock block=currentProgram.getMemory().createInitializedBlock("PHYSICAL_ROM_BANK1_OVERLAY",toAddr(0x100000),in,physical.length(),monitor,true);
            block.setRead(true);block.setWrite(false);block.setExecute(false);
        }
        io("IO_INPUT_WATCHDOG",0x300000,2);io("IO_AUDIO_COIN",0x320000,2);
        io("IO_SYSTEM",0x380000,0x100);io("IO_SYSTEM_LATCH",0x3a0000,0x20);
        io("IO_VIDEO",0x3c0000,0x10);
        createLabel(toAddr(0x101f14),"OBSERVED_SLOT_101F14",true,SourceType.USER_DEFINED);
        createLabel(toAddr(0x101414),"OBSERVED_LINK_ANCHOR_101414",true,SourceType.USER_DEFINED);
        // Bytes after the animation error TRAP are diagnostic text, not a CPU
        // fallthrough path. Define them before disassembly to prevent invented
        // branches from text bytes into the middle of actual instructions.
        createData(toAddr(0x42aa),new ArrayDataType(ByteDataType.dataType,0x12,1));
        long[] seeds={0x2976,0x29c6,0x2e98,0x40bc,0x410e,0x4240,0x4256,0x427a,0x42c0,0x42c4,0x42c8,0x42d4,0x42e0,0x42e8,0x435c,0x4364,0x4470,0x44da,0x143e4,0x1456a,0x145bc,0x1adaa,0xaef14,0xaef30,0xaf17e,0xaf2b2,0xaf65a,0xaf6ba,0xaf6c0,0xaf890,0xaf8a6,0xaf380,0xb0354};
        DecompInterface decompiler=new DecompInterface();decompiler.openProgram(currentProgram);
        try (PrintWriter report=new PrintWriter(new File(out,"instructions.tsv"));
             PrintWriter functions=new PrintWriter(new File(out,"functions.tsv"));
             PrintWriter blocks=new PrintWriter(new File(out,"memory-map.tsv"))) {
            report.println("seed\taddress\tmnemonic\toperands");functions.println("seed\tfunction_entry\tname\tdecompile_complete");
            blocks.println("name\tstart\tend\twrite\texecute\tvolatile");
            for(MemoryBlock block:currentProgram.getMemory().getBlocks())
                blocks.println(block.getName()+"\t"+block.getStart()+"\t"+block.getEnd()+"\t"+block.isWrite()+"\t"+block.isExecute()+"\t"+block.isVolatile());
            for(long seed:seeds) {
                Address address=toAddr(seed);createLabel(address,"ANALYSIS_SEED_"+Long.toHexString(seed),false,SourceType.USER_DEFINED);
                disassemble(address);
                Function f=getFunctionAt(address);
                if(f==null)f=getFunctionContaining(address);
                if(f==null)f=createFunction(address,"analysis_"+Long.toHexString(seed));
                InstructionIterator instructions=currentProgram.getListing().getInstructions(address,true);
                for(int n=0;n<160 && instructions.hasNext();n++) {
                    Instruction instruction=instructions.next();
                    if(instruction.getAddress().getOffset()>seed+0x300)break;
                    String text=instruction.toString();int space=text.indexOf(' ');
                    report.println(Long.toHexString(seed)+"\t"+instruction.getAddress()+"\t"+instruction.getMnemonicString()+"\t"+(space>=0?text.substring(space+1):""));
                }
                if(f==null){functions.println(Long.toHexString(seed)+"\tNONE\tNONE\tfalse");continue;}
                DecompileResults result=decompiler.decompileFunction(f,30,monitor);
                functions.println(Long.toHexString(seed)+"\t"+f.getEntryPoint()+"\t"+f.getName()+"\t"+result.decompileCompleted());
                try(PrintWriter c=new PrintWriter(new File(out,"TENTATIVE_"+Long.toHexString(seed)+".c"))) {
                    c.println("/* TENTATIVE GHIDRA OUTPUT: types, names, branches and semantics require validation. */");
                    if(result.decompileCompleted())c.println(result.getDecompiledFunction().getC());
                    else c.println("/* "+result.getErrorMessage()+" */");
                }
            }
        } finally {decompiler.dispose();}
        println("Observed Neo Geo memory map and addressed soldier analysis exported to "+out);
    }
}
