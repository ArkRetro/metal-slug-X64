/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0fea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0fea(void)

{
  short sVar1;
  uint uVar2;
  uint uVar3;
  ushort uVar4;
  uint *extraout_A0;
  int unaff_A5;
  int unaff_A6;

  candidate_ca2ce();
  uVar4 = 0xffff;
  sVar1 = *(short *)(unaff_A6 + 0x70);
  if (1 < *(byte *)(unaff_A5 + 0x4718 + (int)sVar1)) {
    uVar4 = 0xffbf;
  }
  if (((*(byte *)(unaff_A6 + 0x9a) & 1) == 0) &&
     (*(byte *)(unaff_A5 + 0x471c + (int)sVar1) <= *(byte *)(unaff_A5 + 0x471a + (int)sVar1))) {
    uVar4 = uVar4 & 0xffef;
  }
  uVar2 = *extraout_A0 & CONCAT22(uVar4,uVar4);
  uVar3 = uVar2 >> 0x10;
  if ((uVar3 & 1) == 0) {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xdf;
  }
  else {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x20;
  }
  if ((uVar3 & 2) == 0) {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xef;
  }
  else {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x10;
  }
  *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) & 0xf7;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if ((uVar3 & 4) != 0) {
      *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) | 8;
    }
  }
  else if ((uVar3 & 8) != 0) {
    *(byte *)(unaff_A6 + 0x9a) = *(byte *)(unaff_A6 + 0x9a) | 8;
  }
  *(byte *)(unaff_A6 + 0x9b) =
       *(byte *)(unaff_A6 + 0x9c) & (byte)(uVar2 >> 0x10) | *(byte *)(unaff_A6 + 0x9b);
  return;
}
