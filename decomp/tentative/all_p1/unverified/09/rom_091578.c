/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091578.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined1 candidate_91578(void)

{
  int iVar1;
  undefined1 in_D0b;
  undefined1 uVar2;
  short sVar3;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x92) == '\x01') {
    iVar1 = *(int *)(unaff_A6 + 0x76);
    sVar3 = *(short *)(iVar1 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if (sVar3 < 0) {
      sVar3 = -sVar3;
    }
    if (((0x30 < sVar3) &&
        (sVar3 = *(short *)(iVar1 + 0x40) - *(short *)(unaff_A6 + 0x40), -9 < sVar3)) &&
       (sVar3 < 0x29)) {
      sVar3 = *(short *)(iVar1 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
      if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
        sVar3 = -sVar3;
      }
      if (sVar3 < 0) {
        uVar2 = 8;
      }
      else {
        uVar2 = 0;
      }
      return uVar2;
    }
  }
  return in_D0b;
}
