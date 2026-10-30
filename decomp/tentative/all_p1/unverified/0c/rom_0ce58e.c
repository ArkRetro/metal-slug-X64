/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce58e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_ce58e(void)

{
  undefined2 uVar1;
  int unaff_A6;

  *(short *)(unaff_A6 + 0x98) = *(short *)(unaff_A6 + 0x98) + -1;
  if (*(short *)(unaff_A6 + 0x98) < 1) {
    uVar1 = 2;
    if (*(char *)(unaff_A6 + 0x73) < '\x02') {
      uVar1 = candidate_260c8();
      if (*(char *)(unaff_A6 + 0x73) != '\x01') {
        uVar1 = candidate_260c8();
      }
    }
    return uVar1;
  }
  return 0;
}
