/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04beec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4beec(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  *(undefined2 *)((int)unaff_A6 + 0x26) = 1;
  *(undefined2 *)(unaff_A6 + 0x22) = 2;
  *unaff_A6 = 0x4bf14;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    if (*(short *)((int)unaff_A6 + 0x26) == 0x100) {
      candidate_120f8();
      *unaff_A6 = candidate_4bf80;
      uVar2 = false;
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x22) = 2;
      sVar1 = *(short *)((int)unaff_A6 + 0x26);
      uVar2 = sVar1 < 0;
      *(short *)((int)unaff_A6 + 0x26) = sVar1 << 1;
    }
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x22);
    uVar2 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
  }
  candidate_4d9de();
  candidate_4d74a();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  return;
}
