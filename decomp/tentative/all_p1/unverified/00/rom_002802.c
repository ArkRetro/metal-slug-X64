/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002802.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2802(void)

{
  short in_D0w;
  int unaff_A5;

  DAT_00100012 = (char)in_D0w + -1;
  if (in_D0w == 0) {
    DAT_00100012 = 0;
    *(byte *)(unaff_A5 + -0x7fed) = *(byte *)(unaff_A5 + -0x7fed) | 8;
    _DAT_003c0006 = *(undefined2 *)(unaff_A5 + -0x7fee);
    return;
  }
  *(byte *)(unaff_A5 + -0x7fed) = *(byte *)(unaff_A5 + -0x7fed) & 0xf7;
  _DAT_003c0006 = *(undefined2 *)(unaff_A5 + -0x7fee);
  return;
}
