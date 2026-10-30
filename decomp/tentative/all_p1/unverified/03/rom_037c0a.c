/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037c0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37c0a(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  short sVar4;
  int iVar5;
  int *piVar6;
  int unaff_A6;
  bool bVar7;

  iVar5 = *(int *)(unaff_A6 + 0xc);
  if (*(char *)(unaff_A6 + 0x97) == '\x02') {
    iVar5 = *(int *)(iVar5 + 0xc);
  }
  piVar6 = (int *)(iVar5 + 0x76);
  sVar4 = 1;
  uVar3 = 0xffff;
  bVar7 = false;
  while (!bVar7) {
    if ((*piVar6 != 0) && (iVar5 = *piVar6, (*(byte *)(iVar5 + 0x5f) & 0x80) == 0)) {
      sVar1 = *(short *)(iVar5 + 0x28) - *(short *)(unaff_A6 + 0x28);
      if (sVar1 < 0) {
        sVar1 = -sVar1;
      }
      sVar2 = *(short *)(iVar5 + 0x2c) - *(short *)(unaff_A6 + 0x2c);
      if (sVar2 < 0) {
        sVar2 = -sVar2;
      }
      if ((ushort)(sVar2 + sVar1) <= uVar3) {
        uVar3 = sVar2 + sVar1;
      }
    }
    piVar6 = piVar6 + 1;
    sVar4 = sVar4 + -1;
    bVar7 = sVar4 == 0;
  }
  return;
}
