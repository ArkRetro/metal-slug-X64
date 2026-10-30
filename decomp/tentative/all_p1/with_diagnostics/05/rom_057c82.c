/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057c82.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_57c82(void)

{
  undefined4 uVar1;
  int in_A0;
  short sVar2;

  if ('\x03' < *(char *)(in_A0 + 7)) {
    __m68k_trap(0xf);
    __m68k_trap(0xb);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  uVar1 = candidate_57c18();
  if (10 < (ushort)uVar1) {
    return;
  }
  if (10 < (short)(ushort)uVar1) {
    __m68k_trap(0xf);
    __m68k_trap(0xb);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  sVar2 = 10;
  while( true ) {
    candidate_57d06(uVar1,sVar2,in_A0);
    if (sVar2 == (short)uVar1) break;
    sVar2 = sVar2 + -1;
  }
  candidate_57a98();
  return;
}
