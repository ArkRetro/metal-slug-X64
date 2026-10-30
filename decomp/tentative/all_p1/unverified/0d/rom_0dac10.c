/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dac10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dac10(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(char *)((int)unaff_A6 + 0x61) = *(char *)((int)unaff_A6 + 0x61) + '\x10';
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x10;
    candidate_1da40();
    candidate_1da40();
    candidate_1da40();
    *(short *)(extraout_A0 + 0x28) = *(short *)(extraout_A0 + 0x28) + 0x20;
    *(short *)(extraout_A0 + 0x2c) = *(short *)(extraout_A0 + 0x2c) + 0x20;
    *unaff_A6 = candidate_daca8;
  }
  bVar1 = false;
  candidate_da488();
  if (bVar1) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *unaff_A6 = 0xdacde;
  }
  if (*(char *)((int)unaff_A6 + 0x72) != *(char *)((int)unaff_A6 + 0x61)) {
    candidate_dad10();
  }
  *(undefined1 *)((int)unaff_A6 + 0x72) = *(undefined1 *)((int)unaff_A6 + 0x61);
  return;
}
