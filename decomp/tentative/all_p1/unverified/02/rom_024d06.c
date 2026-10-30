/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024d06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24d06(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  int unaff_A6;

  sVar1 = candidate_1b14a();
  if (sVar1 < 0x80) {
    sVar1 = 0x100 - sVar1;
  }
  uVar2 = (*(byte *)(unaff_A6 + 0x9c) + 1 & 0xe) >> 2;
  sVar3 = *(short *)(unaff_A6 + 0x76);
  if (sVar1 < 0xa9) {
    if (uVar2 != 0) {
      if (uVar2 == 2) {
        sVar3 = *(short *)(unaff_A6 + 0x3c) + 0x28;
      }
      else {
        sVar3 = *(short *)(unaff_A6 + 0x3c) + -0x10;
      }
    }
  }
  else if (sVar1 < 0xd8) {
    if (uVar2 != 1) {
      if (uVar2 == 0) {
        sVar3 = *(short *)(unaff_A6 + 0x3c) + -0x28;
      }
      else {
        sVar3 = *(short *)(unaff_A6 + 0x3c) + 0x28;
      }
    }
  }
  else if (uVar2 != 2) {
    if (uVar2 == 0) {
      sVar3 = *(short *)(unaff_A6 + 0x3c) + -0x28;
    }
    else {
      sVar3 = *(short *)(unaff_A6 + 0x3c) + 0x10;
    }
  }
  *(short *)(unaff_A6 + 0x76) = sVar3;
  return;
}
