/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0db52c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_db52c(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x3c;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  unaff_A6[0x14] = 0xdc0dc;
  *unaff_A6 = 0xdb546;
  uVar1 = 0;
  candidate_1abc8();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x10;
    candidate_1da40();
    candidate_1da40();
    candidate_1da40();
    *(short *)(extraout_A0 + 0x28) = *(short *)(extraout_A0 + 0x28) + 0x20;
    *(short *)(extraout_A0 + 0x2c) = *(short *)(extraout_A0 + 0x2c) + 0x30;
    *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x10';
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *unaff_A6 = candidate_db5ce;
  }
  bVar2 = false;
  candidate_da488();
  if (bVar2) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *(byte *)(unaff_A6[3] + 0x61) = *(byte *)(unaff_A6[3] + 0x61) | 0xf0;
    *unaff_A6 = 0xdbec6;
  }
  return;
}
