/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x014b90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_14b90(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x70) - (*(short *)(unaff_A6 + 0x70) >> 8);
  iVar4 = *(int *)(unaff_A6 + 0x62) << 4;
  sVar2 = (short)((uint)iVar4 >> 0x10);
  if (iVar4 < 0) {
    sVar2 = -sVar2;
  }
  sVar1 = *(short *)(unaff_A6 + 0x90) - *(short *)(unaff_A6 + 0x6e);
  if (sVar1 < 0) {
    sVar3 = sVar3 - (sVar2 + 0xa0);
    if (-1 < sVar3) {
      sVar3 = 0;
    }
    if ((short)(sVar3 - sVar1) < 0) {
      sVar3 = sVar1;
    }
  }
  else {
    sVar3 = sVar2 + 0xa0 + sVar3;
    if (sVar3 < 0) {
      sVar3 = 0;
    }
    if (-1 < (short)(sVar3 - sVar1)) {
      sVar3 = sVar1;
    }
  }
  sVar2 = sVar3 + *(short *)(unaff_A6 + 0x6e);
  if (sVar2 < 0x2001) {
    if (sVar2 < -0x2000) {
      sVar2 = -0x2000;
      sVar3 = 0;
    }
  }
  else {
    sVar2 = 0x2000;
    sVar3 = 0;
  }
  *(short *)(unaff_A6 + 0x70) = sVar3;
  *(short *)(unaff_A6 + 0x6e) = sVar2;
  return;
}
