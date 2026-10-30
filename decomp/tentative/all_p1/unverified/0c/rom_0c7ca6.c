/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c7ca6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x000c7cea) */

void candidate_c7ca6(void)

{
  int iVar1;
  int iVar2;
  int in_D0;
  int iVar3;
  int in_D1;
  int unaff_D2;
  int unaff_D3;
  ushort uVar4;
  short sVar5;
  int extraout_A0;
  int unaff_A5;
  undefined8 uVar6;
  int iStack_10;
  int iStack_c;
  int iStack_8;
  int iStack_4;

  candidate_c7ece();
  iStack_10 = (uint)*(ushort *)(unaff_A5 + 0x69ae) << 0x10;
  iStack_c = (uint)*(ushort *)(unaff_A5 + 0x69b2) << 0x10;
  iStack_8 = (uint)*(ushort *)(unaff_A5 + 0x69b0) << 0x10;
  iStack_4 = (uint)*(ushort *)(unaff_A5 + 0x69b4) << 0x10;
  if (*(int *)(unaff_A5 + 0x69bc) != -1) {
    if (*(ushort *)(unaff_A5 + 0x69c0) != 0x8000) {
      iStack_10 = (uint)*(ushort *)(unaff_A5 + 0x69c0) << 0x10;
    }
    if (*(ushort *)(unaff_A5 + 0x69c4) != 0x8000) {
      iStack_c = (uint)*(ushort *)(unaff_A5 + 0x69c4) << 0x10;
    }
    if (*(ushort *)(unaff_A5 + 0x69c2) != 0x8000) {
      iStack_8 = (uint)*(ushort *)(unaff_A5 + 0x69c2) << 0x10;
    }
    if (*(ushort *)(unaff_A5 + 0x69c6) != 0x8000) {
      iStack_4 = (uint)*(ushort *)(unaff_A5 + 0x69c6) << 0x10;
    }
  }
  if (*(int *)(unaff_A5 + 0x699e) == 0) {
    *(undefined4 *)(unaff_A5 + 0x699e) = 0;
  }
  uVar6 = candidate_c7f3a();
  iVar3 = (int)((ulonglong)uVar6 >> 0x20);
  iRam0010e974 = (int)uVar6;
  if (iStack_10._0_2_ != -0x8000) {
    iStack_10 = unaff_D2 + iStack_10;
  }
  if (iStack_c._0_2_ != -0x8000) {
    iStack_c = unaff_D2 + iStack_c;
  }
  if (iStack_8._0_2_ != -0x8000) {
    iStack_8 = unaff_D3 + iStack_8;
  }
  if (iStack_4._0_2_ != -0x8000) {
    iStack_4 = unaff_D3 + iStack_4;
  }
  if (in_D0 < 0) {
    if (((iStack_10 != -0x80000000) && (iVar3 - iStack_10 < 0)) &&
       (iVar3 = *(int *)(candidate_698c + unaff_A5 + 2), -1 < iVar3 - iStack_10)) {
      iVar3 = iStack_10;
    }
  }
  else if (((iStack_c != -0x80000000) && (-1 < iVar3 - iStack_c)) &&
          (iVar3 = *(int *)(candidate_698c + unaff_A5 + 2),
          *(int *)(candidate_698c + unaff_A5 + 2) - iStack_c < 0)) {
    iVar3 = iStack_c;
  }
  if (in_D1 < 0) {
    if (((iStack_8 != -0x80000000) && (iRam0010e974 - iStack_8 < 0)) &&
       (iRam0010e974 = *(int *)(unaff_A5 + 0x6992), -1 < iRam0010e974 - iStack_8)) {
      iRam0010e974 = iStack_8;
    }
  }
  else if (((iStack_4 != -0x80000000) && (-1 < iRam0010e974 - iStack_4)) &&
          (iRam0010e974 = *(int *)(unaff_A5 + 0x6992), *(int *)(unaff_A5 + 0x6992) - iStack_4 < 0))
  {
    iRam0010e974 = iStack_4;
  }
  iVar1 = *(int *)(candidate_698c + unaff_A5 + 2);
  iVar2 = *(int *)(unaff_A5 + 0x6992);
  *(int *)(candidate_698c + unaff_A5 + 2) = iVar3;
  *(int *)(unaff_A5 + 0x6992) = iRam0010e974;
  iRam0010e974 = iRam0010e974 - iVar2;
  *(int *)(unaff_A5 + 0x6996) = iVar3 - iVar1;
  *(int *)(unaff_A5 + 0x699a) = iRam0010e974;
  iRam0010e970 = -(iVar3 - iVar1);
  iRam0010e974 = -iRam0010e974;
  candidate_1ac38();
  *(int *)(extraout_A0 + 0x18) = *(int *)(unaff_A5 + 0x69cc) + *(int *)(extraout_A0 + 0x18);
  uVar4 = (ushort)((uint)iStack_10 >> 0x10);
  if ((uVar4 != 0x8000) &&
     (uVar4 = uVar4 - *(short *)(candidate_698c + unaff_A5 + 2), -1 < (int)((uint)uVar4 << 0x10))) {
    uVar4 = 0;
  }
  *(ushort *)(unaff_A5 + 0x69d2) = uVar4;
  sVar5 = (short)((uint)iStack_c >> 0x10);
  if (sVar5 != -0x8000) {
    uVar4 = sVar5 - *(short *)(candidate_698c + unaff_A5 + 2);
    if ((int)((uint)uVar4 << 0x10) < 0) {
      uVar4 = 0;
    }
    sVar5 = uVar4 + 0x140;
  }
  *(short *)(unaff_A5 + 0x69d4) = sVar5 + -1;
  sVar5 = (short)((uint)iStack_8 >> 0x10);
  if (sVar5 != -0x8000) {
    uVar4 = sVar5 - *(short *)(unaff_A5 + 0x6992);
    if (-1 < (int)((uint)uVar4 << 0x10)) {
      uVar4 = 0;
    }
    sVar5 = uVar4 + 0x100;
  }
  *(short *)(unaff_A5 + 0x69d6) = sVar5;
  sVar5 = (short)((uint)iStack_4 >> 0x10);
  if (sVar5 != -0x8000) {
    uVar4 = sVar5 - *(short *)(unaff_A5 + 0x6992);
    if ((int)((uint)uVar4 << 0x10) < 0) {
      uVar4 = 0;
    }
    sVar5 = uVar4 + 0x200;
  }
  *(short *)(unaff_A5 + 0x69d8) = sVar5 + -1;
  return;
}
