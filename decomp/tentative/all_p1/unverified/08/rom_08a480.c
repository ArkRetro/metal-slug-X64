/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a480.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_8a480(void)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1af9a();
  if ((bool)in_CF) {
    sVar1 = *(short *)(unaff_A6 + 0x3c);
  }
  else {
    sVar1 = *(short *)(extraout_A0 + 0x3c);
    if (*(short *)(unaff_A6 + 0x76) == 2) {
      if ((*(byte *)(extraout_A0 + 0x15) & 1) == 0) {
        sVar1 = sVar1 + -0x10;
      }
      else {
        sVar1 = sVar1 + 0x10;
      }
    }
  }
  sVar1 = *(short *)(unaff_A6 + 0x7c) + sVar1;
  if (sVar1 < 0x20) {
    sVar1 = 0x20;
  }
  if (0x120 < sVar1) {
    sVar1 = 0x120;
  }
  uVar2 = sVar1 - *(short *)(unaff_A6 + 0x3c);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = -uVar2;
  }
  uVar3 = uVar2;
  if ((short)uVar2 < 0) {
    uVar3 = -uVar2;
  }
  if (uVar3 < 0xc) {
    uVar2 = 0;
  }
  return uVar2;
}
