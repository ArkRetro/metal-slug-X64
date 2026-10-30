/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01945c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1945c(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  short sVar4;
  int iVar5;
  short sVar6;
  int iVar7;
  int iVar8;
  short sVar9;
  short sVar10;
  short *in_A0;
  int unaff_A5;
  int unaff_A6;

  piVar2 = *(int **)(thunk_FUN_00004486 + unaff_A5);
  iVar1 = *piVar2;
  iVar3 = *(int *)(unaff_A6 + 0x3c);
  iVar5 = *(int *)(unaff_A6 + 0x40);
  iVar7 = *(int *)(unaff_A6 + 0x44);
  iVar8 = *(int *)(unaff_A6 + 0x48);
  if (iVar1 != 0) {
    iVar3 = iVar3 - *(int *)(iVar1 + 0x14);
    iVar5 = iVar5 - *(int *)(iVar1 + 0x18);
    iVar7 = iVar7 - *(int *)(iVar1 + 0x1c);
    iVar8 = iVar8 - *(int *)(iVar1 + 0x20);
  }
  sVar4 = *in_A0;
  sVar6 = in_A0[1];
  sVar10 = sVar4 + in_A0[2];
  sVar9 = sVar4;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar9 = -sVar10;
    sVar10 = -sVar4;
  }
  if (iVar7 != iVar3) {
    sVar4 = (short)((uint)iVar3 >> 0x10);
    if (iVar7 - iVar3 < 0) {
      if (-1 < (short)((sVar4 + sVar9) - *(short *)(piVar2 + 2))) {
        return;
      }
    }
    else if ((short)((sVar4 + sVar10) - *(short *)(piVar2 + 1)) < 0) {
      return;
    }
  }
  sVar4 = sVar6 + in_A0[3];
  sVar10 = sVar6;
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    sVar10 = -sVar4;
    sVar4 = -sVar6;
  }
  if (iVar8 != iVar5) {
    sVar6 = (short)((uint)iVar5 >> 0x10);
    if (iVar8 - iVar5 < 0) {
      if (-1 < (short)((sVar6 + sVar10) - *(short *)((int)piVar2 + 10))) {
        return;
      }
    }
    else if ((short)((sVar6 + sVar4) - *(short *)((int)piVar2 + 6)) < 0) {
      return;
    }
  }
  return;
}
