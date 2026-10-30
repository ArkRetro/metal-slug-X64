/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072cac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72cac(void)

{
  byte bVar1;
  uint in_D0;
  ushort uVar2;
  char cVar5;
  ushort uVar3;
  short sVar4;
  ushort uVar6;
  int unaff_A6;
  bool bVar7;

  uVar2 = (ushort)((in_D0 & 4) != 0);
  if ((in_D0 & 8) != 0) {
    uVar2 = uVar2 | 2;
  }
  if ((in_D0 & 2) != 0) {
    uVar2 = uVar2 | 4;
  }
  if ((char)uVar2 == '\0') {
    uVar2 = (ushort)*(byte *)(unaff_A6 + 0x76);
  }
  else {
    cVar5 = *(char *)((short)uVar2 + 0x72d4a) - *(char *)(unaff_A6 + 0x74);
    if (cVar5 != '\0') {
      bVar7 = cVar5 >> 7 < '\0';
      cVar5 = (-bVar7 ^ 0x10 - (*(byte *)(unaff_A6 + 0x74) & 8)) + bVar7;
    }
    bVar1 = *(char *)(unaff_A6 + 0x74) + cVar5;
    uVar2 = (ushort)bVar1;
    if (-1 < (short)((ushort)bVar1 << 8)) {
      uVar2 = -uVar2;
    }
    *(char *)(unaff_A6 + 0x76) = (char)uVar2;
  }
  uVar2 = uVar2 * 0x100 - *(short *)(unaff_A6 + 0x74);
  if (uVar2 != 0) {
    uVar3 = (ushort)-((short)uVar2 < 0);
    uVar2 = (uVar3 ^ uVar2) - uVar3;
    uVar6 = 0x800;
    if (uVar2 < 0x800) {
      uVar6 = uVar2;
    }
    *(short *)(unaff_A6 + 0x74) = ((uVar3 ^ uVar6) - uVar3) + *(short *)(unaff_A6 + 0x74);
  }
  sVar4 = *(short *)(unaff_A6 + 0x74);
  if (-1 < sVar4) {
    sVar4 = -sVar4;
  }
  *(short *)(unaff_A6 + 0x74) = sVar4;
  return;
}
