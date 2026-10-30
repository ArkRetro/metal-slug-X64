/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccfe8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccfe8(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_18f92();
  unaff_A6[0xc] = 0x20000;
  unaff_A6[0xd] = 0xffff8000;
  *(undefined1 *)((int)unaff_A6 + 0x73) = 3;
  candidate_1da40();
  *(undefined2 *)(extraout_A0 + 0x66) = 0xfff1;
  *(undefined2 *)(extraout_A0 + 0x1e) = 0xfff1;
  candidate_4256();
  *unaff_A6 = 0xcd030;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xd] = 0;
  }
  if (399 < *(short *)(unaff_A6 + 0xf)) {
    candidate_1b1dc();
    *unaff_A6 = candidate_cd08e;
  }
  if (*(short *)(unaff_A6 + 0x10) < 0x141) {
    unaff_A6[0xd] = 0x4000;
  }
  if (0x16f < (short)*(ushort *)(unaff_A6 + 0x10)) {
    unaff_A6[0xd] = 0xffffc000;
  }
  bVar1 = *(ushort *)(unaff_A6 + 0x10) < 0x170;
  candidate_ce420();
  if (bVar1) {
    *unaff_A6 = candidate_cd08e;
  }
  return;
}
