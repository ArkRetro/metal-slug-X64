/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039ed0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39ed0(void)

{
  short sVar1;
  byte bVar2;
  short sVar3;
  short sVar4;
  int unaff_A6;
  undefined1 uVar5;

  uVar5 = false;
  bVar2 = candidate_1ae8e(0x3a04c);
  if (!(bool)uVar5) {
    if (((bVar2 & 1) != 0) && (bVar2 = candidate_1af3c(), (bool)uVar5)) {
      uVar5 = 0;
      bVar2 = bVar2 ^ 1;
    }
    if (((bVar2 & 2) != 0) && (bVar2 = candidate_1af3c(), (bool)uVar5)) {
      bVar2 = bVar2 ^ 2;
    }
  }
  if (bVar2 != 0) {
    if (bVar2 == 1) {
      return;
    }
    if (bVar2 != 2) {
      sVar1 = *(short *)(unaff_A6 + 0x3c);
      sVar3 = sRam00101500 - sVar1;
      if (sRam00101500 < sVar1) {
        sVar3 = -sVar3;
      }
      sVar4 = sRam001015b0 - sVar1;
      if (sRam001015b0 < sVar1) {
        sVar4 = -sVar4;
      }
      if (sVar3 <= sVar4) {
        return;
      }
    }
    return;
  }
  return;
}
