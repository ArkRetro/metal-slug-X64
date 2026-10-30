/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cc9ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cc9ca(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
  *(undefined2 *)(unaff_A6 + 0x26) = 0x78;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x20;
  *(undefined1 *)((int)unaff_A6 + 0xa1) = 1;
  candidate_1b1dc();
  *(undefined1 *)(unaff_A6 + 0x28) = 2;
  *unaff_A6 = 0xcc9f2;
  candidate_1abc8();
  sVar1 = *(short *)(unaff_A6 + 10) - (*(short *)(unaff_A6 + 0xf) + -0x198);
  *(short *)(unaff_A6 + 10) = sVar1;
  bVar2 = sVar1 == 0;
  candidate_d15be();
  if ((bVar2) &&
     (*(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1,
     *(short *)(unaff_A6 + 0x26) < 1)) {
    *unaff_A6 = candidate_cca24;
  }
  return;
}
