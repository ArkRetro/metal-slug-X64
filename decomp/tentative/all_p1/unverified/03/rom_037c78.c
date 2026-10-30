/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037c78.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37c78(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  short sVar4;
  int *piVar5;
  int iVar6;
  int unaff_A6;
  bool bVar7;

  iVar6 = *(int *)(unaff_A6 + 0xc);
  if (*(char *)(unaff_A6 + 0x97) == '\x02') {
    iVar6 = *(int *)(iVar6 + 0xc);
  }
  piVar5 = (int *)(iVar6 + 0x7e);
  sVar4 = 1;
  uVar3 = 0xffff;
  bVar7 = false;
  while (!bVar7) {
    if ((*piVar5 != 0) && (iVar6 = *piVar5, (*(byte *)(iVar6 + 0x5f) & 0x80) == 0)) {
      sVar1 = *(short *)(iVar6 + 0x28) - *(short *)(unaff_A6 + 0x28);
      if (sVar1 < 0) {
        sVar1 = -sVar1;
      }
      sVar2 = *(short *)(iVar6 + 0x2c) - *(short *)(unaff_A6 + 0x2c);
      if (sVar2 < 0) {
        sVar2 = -sVar2;
      }
      if ((ushort)(sVar2 + sVar1) <= uVar3) {
        uVar3 = sVar2 + sVar1;
      }
    }
    piVar5 = piVar5 + 1;
    sVar4 = sVar4 + -1;
    bVar7 = sVar4 == 0;
  }
  return;
}
