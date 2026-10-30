/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091638.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_91638(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  int extraout_A0;
  int unaff_A6;

  sVar1 = candidate_2eb4();
  sVar3 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
  if (sVar3 < 0) {
    sVar3 = -sVar3;
  }
  if (sVar3 < 0x30) {
    if (sVar1 < 0x30) {
      sVar1 = 0x30;
    }
    else if (0x50 < sVar1) {
      sVar1 = 0x50;
    }
  }
  uVar2 = (ushort)(byte)(*(char *)(unaff_A6 + 0x92) * ' ' + -0x20 + ((char)sVar1 + 8U & 0xf0));
  if (0x80 < uVar2) {
    if (uVar2 < 0xc0) {
      uVar2 = 0x80;
    }
    else {
      uVar2 = 0;
    }
  }
  uVar2 = uVar2 >> 4;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    uVar2 = (ushort)*(byte *)((short)uVar2 + 0x92490);
  }
  return uVar2;
}
