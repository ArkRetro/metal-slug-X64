/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a9d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a9d4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_120f8();
  candidate_18f92();
  candidate_13f1e();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  *(undefined2 *)(unaff_A6 + 0x22) = 0x80;
  candidate_40bc();
  candidate_1acf2();
  candidate_1da40();
  unaff_A6[0x14] = 0;
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 8;
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 0x20;
  *unaff_A6 = 0x4aa3a;
  candidate_1abc8();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    *unaff_A6 = candidate_4aaae;
    uVar1 = 0;
  }
  else {
    *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
    if (*(short *)(unaff_A6 + 0x22) == 0x40) {
      candidate_1da40();
    }
    if (*(short *)(unaff_A6 + 0x22) == 0x28) {
      candidate_1da40();
    }
    uVar1 = *(ushort *)(unaff_A6 + 0x22) < 0x10;
    if (*(ushort *)(unaff_A6 + 0x22) == 0x10) {
      candidate_1da40();
    }
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  return;
}
