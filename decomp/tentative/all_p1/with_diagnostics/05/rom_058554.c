/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058554.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

code candidate_58554(void)

{
  short in_D0w;
  int extraout_A1;
  int unaff_A5;
  bool bVar1;

  if (in_D0w < 2) {
    if (in_D0w == 0) {
      return FUN_0000435c[unaff_A5 + 2];
    }
    return candidate_4364[unaff_A5];
  }
  __m68k_trap(0xf);
  if (in_D0w != 1) {
    bVar1 = false;
    func_0x000585df();
    if (!bVar1) {
      *(short *)(extraout_A1 + -2) = *(short *)(extraout_A1 + -2) + -7;
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
