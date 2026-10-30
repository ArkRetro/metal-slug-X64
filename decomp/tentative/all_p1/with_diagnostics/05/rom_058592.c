/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058592.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_58592(void)

{
  short in_D0w;
  undefined4 uVar1;
  int *piVar2;
  int unaff_D6;
  int extraout_A0;
  int extraout_A1;
  bool bVar3;

  if (in_D0w < 2) {
    if (in_D0w == 0) {
      return;
    }
    return;
  }
  __m68k_trap(0xf);
  if (in_D0w != 1) {
    bVar3 = false;
                    /* WARNING: Call to offcut address within same function */
    uVar1 = func_0x0005861d();
    if (!bVar3) {
      *(short *)(extraout_A1 + -2) = *(short *)(extraout_A1 + -2) + -7;
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    piVar2 = (int *)CONCAT31((int3)((uint)uVar1 >> 8),(char)uVar1 + '\x04');
    *(char *)(extraout_A0 + -1) = *(char *)(extraout_A0 + -1) + -1;
    if (unaff_D6 < 0 || *piVar2 < unaff_D6) {
      __m68k_trap(6);
    }
    if (unaff_D6 < 0 || *piVar2 < unaff_D6) {
      __m68k_trap(6);
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
