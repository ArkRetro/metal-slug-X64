/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019504.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_19504(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  short sVar4;
  int iVar5;
  short sVar6;
  short sVar7;
  short sVar8;
  short *in_A0;
  int unaff_A5;
  int unaff_A6;

  piVar2 = *(int **)(thunk_FUN_00004486 + unaff_A5);
  iVar1 = *piVar2;
  iVar3 = *(int *)(unaff_A6 + 0x3c);
  iVar5 = *(int *)(unaff_A6 + 0x40);
  if (iVar1 != 0) {
    iVar3 = iVar3 - *(int *)(iVar1 + 0x14);
    iVar5 = iVar5 - *(int *)(iVar1 + 0x18);
  }
  sVar4 = *in_A0;
  sVar6 = in_A0[1];
  sVar8 = sVar4 + in_A0[2];
  sVar7 = sVar4;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar7 = -sVar8;
    sVar8 = -sVar4;
  }
  sVar4 = (short)((uint)iVar3 >> 0x10);
  if ((-1 < (short)((sVar4 + sVar8) - *(short *)(piVar2 + 1))) &&
     ((short)((sVar4 + sVar7) - *(short *)(piVar2 + 2)) < 0)) {
    sVar4 = sVar6 + in_A0[3];
    sVar8 = sVar6;
    if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
      sVar8 = -sVar4;
      sVar4 = -sVar6;
    }
    sVar6 = (short)((uint)iVar5 >> 0x10);
    if ((-1 < (short)((sVar6 + sVar4) - *(short *)((int)piVar2 + 6))) &&
       ((short)((sVar6 + sVar8) - *(short *)((int)piVar2 + 10)) < 0)) {
      return;
    }
  }
  return;
}
