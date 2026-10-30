/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa610.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_aa610(void)

{
  ushort uVar2;
  uint uVar1;
  ushort uVar3;
  short sVar4;
  int unaff_A5;
  int unaff_A6;
  undefined1 in_CF;
  char cVar5;
  char cVar6;

  candidate_18f24();
  if ((bool)in_CF) {
    return 0xffffffff;
  }
  cVar5 = '\0';
  candidate_aa6b6();
  cVar6 = '\0';
  candidate_aa6b6();
  sVar4 = CONCAT11(-cVar5 >> 7,-cVar6);
  if (sVar4 == 0) {
    return 0xffffffff;
  }
  if (sVar4 == 0xff) {
    return 1;
  }
  if (sVar4 == -0x100) {
    return 0;
  }
  uVar2 = *(short *)(unaff_A5 + -0x6b14) - *(short *)(unaff_A6 + 0x28);
  if ((short)uVar2 < 0) {
    uVar2 = -uVar2;
  }
  uVar3 = *(short *)(unaff_A5 + -0x6a64) - *(short *)(unaff_A6 + 0x28);
  if ((short)uVar3 < 0) {
    uVar3 = -uVar3;
  }
  if (uVar3 == uVar2) {
    uVar1 = candidate_29c6();
    return uVar1 & 0xffff0001;
  }
  if (uVar3 < uVar2) {
    return 1;
  }
  return 0;
}
