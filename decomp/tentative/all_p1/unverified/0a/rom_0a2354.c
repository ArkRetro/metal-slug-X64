/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2354.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2354(void)

{
  short sVar1;
  ushort uVar2;
  byte bVar3;
  int unaff_A6;

  bVar3 = *(byte *)(unaff_A6 + 0x88);
  uVar2 = *(byte *)(unaff_A6 + 0x8a) & 3;
  if (uVar2 != 3) {
    sVar1 = *(short *)(*(int *)((short)(uVar2 << 2) + 0xa2cf2) +
                      (int)(short)((*(byte *)(unaff_A6 + 0x89) & 3) << 1));
    if (sVar1 == 0) {
      if (bVar3 < 0x38) {
        bVar3 = 0x30;
      }
      else if (bVar3 < 0x49) {
        bVar3 = 0x40;
      }
      else {
        bVar3 = 0x50;
      }
    }
    else if (sVar1 == 1) {
      if (bVar3 < 0x28) {
        bVar3 = 0x20;
      }
      else if (bVar3 < 0x59) {
        bVar3 = (bVar3 - 0x28 & 0xf0) + 0x30;
      }
      else {
        bVar3 = 0x60;
      }
    }
    else if (bVar3 < 0x24) {
      bVar3 = 0x20;
    }
    else if (bVar3 < 0x5d) {
      bVar3 = (bVar3 - 0x24 & 0xf8) + 0x28;
    }
    else {
      bVar3 = 0x60;
    }
  }
  *(byte *)(unaff_A6 + 0x88) = bVar3;
  return;
}
