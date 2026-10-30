/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080f44.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_80f44(void)

{
  short sVar1;
  byte bVar2;
  short sVar3;
  undefined3 uVar4;
  uint in_D0;
  char cVar6;
  uint uVar5;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x8a);
  sVar3 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x8a) = sVar3;
  if (sVar3 != 0 && 0 < sVar1) {
    return in_D0;
  }
  uVar4 = (undefined3)(in_D0 >> 8);
  if ((*(byte *)(unaff_A6 + 0x91) & 7) == 0) {
    cVar6 = -1;
    uVar4 = 0xffffff;
    if (*(char *)(unaff_A6 + 0x91) == '\0') {
      cVar6 = '\x01';
      uVar4 = 0;
    }
    *(char *)(unaff_A6 + 0x92) = cVar6;
  }
  else {
    cVar6 = *(char *)(unaff_A6 + 0x92);
  }
  bVar2 = *(char *)(unaff_A6 + 0x91) + cVar6;
  *(byte *)(unaff_A6 + 0x91) = bVar2;
  uVar5 = CONCAT31(uVar4,bVar2) & 0xffffff07;
  if ((bVar2 & 7) == 0) {
    sVar1 = *(short *)(unaff_A6 + 0x88) + 1;
    uVar5 = CONCAT22((short)((uint3)uVar4 >> 8),sVar1);
    *(short *)(unaff_A6 + 0x8a) = sVar1;
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x8a) = *(undefined2 *)(unaff_A6 + 0x86);
  }
  return uVar5;
}
