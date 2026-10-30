/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aaa88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined6 candidate_aaa88(void)

{
  undefined1 extraout_var;
  char in_D0b;
  undefined4 extraout_D1;
  short *psVar1;
  int unaff_A5;
  bool bVar2;

  bVar2 = false;
  if (in_D0b == '\0') {
    psVar1 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    psVar1 = (short *)(unaff_A5 + 0x462c);
  }
  func_0x000040ac();
  if ((bVar2) && (*psVar1 != 0)) {
    if (*(char *)((int)psVar1 + 0x21) == '\0') {
      __m68k_trap(0xf);
    }
    return CONCAT24(CONCAT11(extraout_var,*(char *)((int)psVar1 + 0x21)),
                    CONCAT31((int3)((uint)extraout_D1 >> 8),*(undefined1 *)((int)psVar1 + 0x1f)));
  }
  return 0;
}
