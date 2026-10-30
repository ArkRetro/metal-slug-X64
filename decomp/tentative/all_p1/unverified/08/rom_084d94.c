/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x084d94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_84d94(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  ushort unaff_D7w;
  int unaff_A6;
  bool bVar4;

  uVar1 = *(undefined4 *)(unaff_A6 + 0x28);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x2c);
  uVar3 = *(undefined4 *)(unaff_A6 + 0x38);
  *(undefined4 *)(unaff_A6 + 0x72) = *(undefined4 *)(unaff_A6 + 0x34);
  *(undefined1 *)(unaff_A6 + 0x98) = *(undefined1 *)(unaff_A6 + 0x15);
  candidate_143dc();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      bVar4 = unaff_D7w < 0x4000;
    }
    else {
      bVar4 = 0x4000 < (short)unaff_D7w;
    }
    if (bVar4) {
      *(undefined4 *)(unaff_A6 + 0x38) = uVar3;
      *(undefined4 *)(unaff_A6 + 0x2c) = uVar2;
      *(undefined4 *)(unaff_A6 + 0x28) = uVar1;
      candidate_1abc8();
      return;
    }
    if ((*(byte *)(unaff_A6 + 0x98) & 0x40) == 0) {
      *(int *)(unaff_A6 + 0x34) = -(*(int *)(unaff_A6 + 0x72) >> 2);
      *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) >> 1;
      candidate_120f8();
    }
  }
  return;
}
