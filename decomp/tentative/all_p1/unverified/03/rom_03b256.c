/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03b256.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3b256(void)

{
  ushort uVar1;
  short sVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  int unaff_A6;

  sVar2 = *(short *)(unaff_A6 + 0x6e) >> 3;
  *(short *)(unaff_A6 + 0x6e) = sVar2;
  sVar4 = (short)-(*(short *)(unaff_A6 + 0x70) + -0x1ff) >> 3;
  *(short *)(unaff_A6 + 0x70) = sVar4;
  sVar2 = sVar2 + -0x14;
  if (sVar2 < 0) {
    sVar2 = -sVar2;
  }
  uVar1 = sVar2 + 0x14;
  sVar4 = sVar4 + -0x10;
  if (sVar4 < 0) {
    sVar4 = -sVar4;
  }
  uVar5 = sVar4 + 0x10;
  uVar3 = uVar1;
  if (uVar5 < uVar1) {
    uVar3 = uVar5;
    uVar5 = uVar1;
  }
  *(char *)(unaff_A6 + 0x74) = (char)uVar5 + (char)(uVar3 >> 1) + '\v';
  return;
}
