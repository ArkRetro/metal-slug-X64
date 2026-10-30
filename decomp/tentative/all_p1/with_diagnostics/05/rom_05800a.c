/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05800a.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_5800a(void)

{
  uint uVar1;
  short sVar2;
  char unaff_D2b;
  short unaff_D7w;
  code *extraout_A1;
  uint *puVar3;

  if (9 < unaff_D7w) {
    __m68k_trap(0xf);
    __m68k_trap(0xb);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  puVar3 = (uint *)0x10f010;
  uVar1 = 1;
  sVar2 = 1;
  while( true ) {
    if (*(uint *)((short)(unaff_D7w * 8) + 0x10f010) < *puVar3) {
      uVar1 = (uint)(ushort)(sVar2 + 1);
    }
    if (sVar2 == (short)(unaff_D7w + 1)) break;
    puVar3 = puVar3 + 2;
    sVar2 = sVar2 + 1;
    if (10 < sVar2) {
      _DAT_00000001 = 1;
      __m68k_trap(0xf);
      candidate_3e2a8();
      candidate_3e2a8();
      while( true ) {
        uVar1 = candidate_3d1b8();
        if (unaff_D2b == '\0') break;
        (*extraout_A1)();
      }
      return uVar1;
    }
  }
  return uVar1;
}
