/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039f0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39f0c(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int extraout_A0;
  int unaff_A6;
  bool bVar4;

  if (*(short *)(unaff_A6 + 0x72) != 0) {
    return;
  }
  if (*(short *)(unaff_A6 + 0x8a) < 0) {
    sVar1 = *(short *)(unaff_A6 + 0x8c) - *(short *)(unaff_A6 + 0x28);
    if (-1 < sVar1) {
      sVar1 = -sVar1;
    }
    if (sVar1 <= *(short *)(unaff_A6 + 0x88)) {
      return;
    }
  }
  sVar1 = 1;
  sVar2 = -0x8000;
  bVar4 = false;
  do {
    *(short *)((int)register0x0000003c + -4) = sVar2;
    *(short *)((int)register0x0000003c + -8) = sVar1;
    *(undefined4 *)((int)register0x0000003c + -0xc) = 0x39f4e;
    candidate_1aeb0();
    sVar1 = *(short *)((int)register0x0000003c + -8);
    sVar2 = *(short *)((int)register0x0000003c + -6);
    register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
    if (bVar4) {
      sVar3 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
      if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
        sVar3 = -sVar3;
      }
      if (sVar2 < sVar3) {
        sVar2 = sVar3;
      }
    }
    bVar4 = sVar1 == 0;
    sVar1 = sVar1 + -1;
  } while (!bVar4);
  if (((sVar2 != -0x8000) && (*(short *)(unaff_A6 + 0x88) < sVar2)) &&
     (*(short *)(unaff_A6 + 0x8a) = sVar2, sVar2 < 0)) {
    *(undefined2 *)(unaff_A6 + 0x8c) = *(undefined2 *)(unaff_A6 + 0x28);
  }
  return;
}
