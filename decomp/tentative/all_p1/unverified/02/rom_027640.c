/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027640.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_27640(void)

{
  undefined1 uVar1;
  int unaff_A6;

  if ((_DAT_0010000e & 2) == 0) {
    *(undefined1 *)(unaff_A6 + 0x8c) = 1;
    uVar1 = *(undefined1 *)(unaff_A6 + 0x1a);
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x8c) = 2;
    uVar1 = *(undefined1 *)(unaff_A6 + 0x1b);
  }
  *(undefined1 *)(unaff_A6 + 0x14) = uVar1;
  return;
}
