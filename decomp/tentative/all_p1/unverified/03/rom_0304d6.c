/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0304d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_304d6(void)

{
  int iVar1;
  undefined4 in_D0;
  undefined1 uVar2;
  undefined4 in_D1;
  short sVar3;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x8a) + *(short *)(unaff_A6 + 0x88);
  if (6 < sVar3) {
    sVar3 = sVar3 + -7;
  }
  iVar1 = *(int *)(unaff_A6 + 0x6e) + (int)(short)(sVar3 << 3);
  *(undefined4 *)(iVar1 + 0x6e) = in_D0;
  *(undefined4 *)(iVar1 + 0x72) = in_D1;
  uVar2 = candidate_3012c();
  *(undefined1 *)(unaff_A6 + sVar3 + 0x91) = uVar2;
  if (*(short *)(unaff_A6 + 0x8a) < 7) {
    *(short *)(unaff_A6 + 0x8a) = *(short *)(unaff_A6 + 0x8a) + 1;
  }
  else {
    *(short *)(unaff_A6 + 0x88) = *(short *)(unaff_A6 + 0x88) + 1;
    if (6 < *(short *)(unaff_A6 + 0x88)) {
      *(undefined2 *)(unaff_A6 + 0x88) = 0;
    }
  }
  return;
}
