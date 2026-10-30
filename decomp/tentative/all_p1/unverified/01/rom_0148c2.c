/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0148c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_148c2(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  short sVar4;
  ushort uVar5;
  uint uVar6;
  short extraout_D1w;
  int iVar7;
  short unaff_D2w;
  int iVar8;
  short unaff_D3w;
  uint unaff_D6;
  undefined4 uVar9;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A1;
  int extraout_A1_00;
  int extraout_A1_01;
  int extraout_A1_02;
  int iVar10;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar11;
  undefined8 uVar12;

  sVar4 = candidate_128da();
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  uVar11 = CONCAT44(uRam0010c034._0_4_,uRam0010c034._4_4_);
  iVar7 = (uint)(ushort)(unaff_D2w - sVar4) * 0x10000;
  iVar8 = (uint)(ushort)(unaff_D3w - extraout_D1w) * 0x10000;
  if (0xffffff < iVar7) {
    iVar7 = 0xffffff;
  }
  if (0xffffff < iVar8) {
    iVar8 = 0xffffff;
  }
  if (iVar7 < -0x1000000) {
    iVar7 = -0x1000000;
  }
  if (iVar8 < -0x1000000) {
    iVar8 = -0x1000000;
  }
  uRam0010c024 = 0;
  uRam0010c012 = 0;
  iRam0010c044 = CONCAT22(0xffff,iRam0010c044._2_2_);
  iRam0010c050 = CONCAT22(0xffff,iRam0010c050._2_2_);
  iRam0010c060 = 0;
  uRam0010c05c = 0x80000000;
  uRam0010c064 = 0x4000;
LAB_000139c0:
  uVar9 = 0;
  uRam0010c034 = uVar11;
  uVar11 = candidate_1ac4c();
  uRam0010c02c = uVar11;
  candidate_138a0();
  if ((char)unaff_D6 != '\0') {
    uVar11 = uRam0010c034;
    if ((uRam0010c024._0_2_ == -1) || (uRam0010c024._2_2_ == -1)) goto LAB_00013a08;
    iVar10 = iVar7;
    iVar1 = iVar8;
    if (extraout_A0 == iRam0010c044) {
      if ((iVar7 == iRam0010c048) && (iVar8 == iRam0010c04c)) goto LAB_00013a08;
    }
    else {
      iVar10 = iRam0010c048;
      iVar1 = iRam0010c04c;
      if (iRam0010c044 < 0) {
        iRam0010c044 = extraout_A0;
        iVar10 = iVar7;
        iVar1 = iVar8;
      }
    }
    iRam0010c04c = iVar1;
    iRam0010c048 = iVar10;
    iRam0010c050 = CONCAT22(0xffff,iRam0010c050._2_2_);
  }
  candidate_1381c();
  candidate_1382a();
  if (extraout_A1 != 0) {
    do {
      uVar12 = candidate_1ac4c();
      uVar11 = uRam0010c034;
      if (extraout_A1_00 != iRam0010c050) {
        iRam0010c03c = iVar7;
        iRam0010c040 = iVar8;
        uRam0010c034 = uVar12;
        candidate_13776();
        if (((cRam0010c009 == '\0') || ((uRam0010c012 & 0x20) == 0)) &&
           (uVar9 = uRam0010c016, (short)uRam0010c016 != 0)) {
          candidate_129ec();
          uVar12 = candidate_12cba();
          iVar10 = extraout_A0_00;
          uVar11 = uRam0010c034;
          if (unaff_D6 < 0x80000001) goto LAB_00013aa2;
LAB_00013ae2:
          if ((int)uRam0010c05c <= (int)unaff_D6) {
            uRam0010c05c = unaff_D6;
            iRam0010c060 = iVar10;
            uRam0010c064 = uVar9;
          }
        }
        else {
LAB_00013aa2:
          uVar11 = candidate_12938();
          uVar3 = (uint)((ulonglong)uVar11 >> 0x20);
          uVar5 = (ushort)((ulonglong)uVar11 >> 0x10);
          uVar6 = CONCAT22(uVar5,(short)uVar11 + 8) << 0x10 | (uint)uVar5;
          uVar3 = uVar3 << 0x10 | uVar3 >> 0x10;
          iVar10 = extraout_A0_01;
          uVar11 = uRam0010c034;
          uVar12 = CONCAT44(uVar3,uVar6);
          if (((short)uVar9 != 0) &&
             ((uRam0010c016._2_2_ != 0x3c || (uVar12 = CONCAT44(uVar3,uVar6), (short)uVar9 != 0x3c))
             )) {
            candidate_129ec();
            uVar12 = candidate_12cba();
            iVar10 = extraout_A0_02;
            uVar11 = uRam0010c034;
            if (unaff_D6 != 0x80000000) {
              unaff_D6 = unaff_D6 - 0x80000;
              goto LAB_00013ae2;
            }
          }
        }
        uRam0010c034._0_4_ = (int)((ulonglong)uVar11 >> 0x20);
        uRam0010c034._4_4_ = (int)uVar11;
        iVar7 = (int)((ulonglong)uVar12 >> 0x20) - uRam0010c034._0_4_;
        iVar8 = (int)uVar12 - uRam0010c034._4_4_;
        if ((iVar7 != iRam0010c03c) || (iVar8 != iRam0010c040)) goto LAB_00013b12;
      }
      uRam0010c034 = uVar11;
      candidate_1382a();
      if (extraout_A1_01 == 0) break;
    } while( true );
  }
  goto LAB_00013b82;
LAB_00013b12:
  if ((uRam0010c024._0_2_ == -1) || (uRam0010c024._2_2_ == -1)) goto LAB_00013a08;
  iVar1 = iVar7;
  iVar2 = iVar8;
  if (iVar10 == iRam0010c044) {
    if ((iVar7 == iRam0010c048) && (iVar8 == iRam0010c04c)) goto LAB_00013a08;
  }
  else {
    iVar1 = iRam0010c048;
    iVar2 = iRam0010c04c;
    if (iRam0010c044 < 0) {
      iRam0010c044 = iVar10;
      iVar1 = iVar7;
      iVar2 = iVar8;
    }
  }
  iRam0010c04c = iVar2;
  iRam0010c048 = iVar1;
  uRam0010c054 = uRam0010c064;
  uRam0010c058 = uRam0010c05c;
  iRam0010c050 = iVar10;
  goto LAB_000139c0;
LAB_00013a08:
  uRam0010c012 = uRam0010c012 | 0x80;
  uRam0010c034 = uVar11;
LAB_00013b82:
  uVar9 = uRam0010c064;
  uVar3 = uRam0010c05c;
  uVar11 = candidate_12e64();
  iVar8 = (int)((ulonglong)uVar11 >> 0x20);
  iVar7 = (int)uVar11;
  if (uVar3 == 0xffffffff) {
LAB_00013bba:
    uRam0010c012 = uRam0010c012 | 1;
    iVar10 = extraout_A1_02;
    if ((short)((uint)uVar9 >> 0x10) != 9) goto LAB_00013bd2;
  }
  else if ((cRam0010c009 != '\0') && (uVar3 != 0x80000000)) {
    iVar7 = uVar3 + 1 + iVar7;
    goto LAB_00013bba;
  }
  iVar10 = 0x10e968;
LAB_00013bd2:
  if (extraout_A0_03 != 0) {
    iVar8 = *(int *)(extraout_A0_03 + 0x14) + iVar8;
    iVar7 = *(int *)(extraout_A0_03 + 0x18) + iVar7;
  }
  if (iVar10 != 0) {
    iVar8 = iVar8 - *(int *)(iVar10 + 0x14);
    iVar7 = iVar7 - *(int *)(iVar10 + 0x18);
  }
  return CONCAT44(iVar8,iVar7);
}
