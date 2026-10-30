/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x0066; tentative_c. */

/* WARNING: This function may have set the stack pointer */

void NMI_ISR(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
            undefined2 param_5)

{
  undefined2 in_AF;
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;

  if (cIo0000 == '\x01') {
    disableMaskableInterrupts();
    cVar1 = '\x04';
    uRamffec = 0x1f1a;
    z80_candidate_8(4,0x7ff,param_4,param_5,param_2,param_3,param_1);
    uRamffec = 0x1f1e;
    z80_candidate_8(0x1100);
    uRamffec = 0x1f22;
    z80_candidate_8(0x1001);
    uRamffec = 0x1f26;
    z80_candidate_8(0xb500);
    uRamffec = 0x1f28;
    z80_candidate_8();
    uRamffec = 0x1f2b;
    z80_candidate_8(cVar1 + '\x02');
    uRamffec = 0x1f2d;
    z80_candidate_8();
    uRamffec = 0x1f31;
    z80_candidate_8(0xff);
    uRamffec = 0x1f35;
    z80_candidate_8(0x800);
    uRamffec = 0x1f37;
    z80_candidate_8();
    uRamffec = 0x1f39;
    z80_candidate_8();
    uRamffec = 0x1f3b;
    z80_candidate_8();
    uRamffec = 0x1f3d;
    z80_candidate_8();
    uRamffec = 7999;
    z80_candidate_8();
    cVar1 = -0x80;
    cVar2 = '\b';
    puVar4 = (undefined1 *)0xf800;
    do {
      do {
        puVar3 = puVar4;
        *puVar3 = 0;
        puVar4 = puVar3 + 1;
        cVar1 = cVar1 + -1;
      } while (cVar1 != '\0');
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    *(undefined1 **)(puVar3 + -1) = puVar4;
    *puVar4 = 0xd3;
    puVar3[2] = 0xc;
    puVar3[3] = 0x18;
    puVar3[4] = 0xfe;
    cIo0008 = 1;
    return;
  }
  if (cIo0000 != '\x03') {
    *(char *)((DAT_ram_ffef & 0xf) - 0x10) = cIo0000;
    DAT_ram_ffef = (DAT_ram_ffef & 0xf) + 1 & 0xf;
    cIo0008 = (char)((ushort)in_AF >> 8);
    return;
  }
  disableMaskableInterrupts();
  uRamffec = 0;
  cIo0008 = cIo0000;
  return;
}
