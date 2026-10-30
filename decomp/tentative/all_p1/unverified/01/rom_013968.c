/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013968.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_13968(void)

{
  uint uVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  int unaff_D2;
  int iVar5;
  int unaff_D3;
  uint unaff_D6;
  undefined4 uVar6;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A1;
  int extraout_A1_00;
  int extraout_A1_01;
  int extraout_A1_02;
  int iVar7;
  undefined8 uVar8;
  undefined8 uVar9;

  if (0xffffff < unaff_D2) {
    unaff_D2 = 0xffffff;
  }
  if (0xffffff < unaff_D3) {
    unaff_D3 = 0xffffff;
  }
  if (unaff_D2 < -0x1000000) {
    unaff_D2 = -0x1000000;
  }
  if (unaff_D3 < -0x1000000) {
    unaff_D3 = -0x1000000;
  }
  uRam0010c024 = 0;
  uRam0010c012 = 0;
  iRam0010c044 = CONCAT22(0xffff,iRam0010c044._2_2_);
  iRam0010c050 = CONCAT22(0xffff,iRam0010c050._2_2_);
  iRam0010c060 = 0;
  uRam0010c05c = 0x80000000;
  uRam0010c064 = 0x4000;
  uVar8 = uRam0010c034;
LAB_000139c0:
  uVar6 = 0;
  uRam0010c034 = uVar8;
  uVar8 = candidate_1ac4c();
  uRam0010c02c = uVar8;
  candidate_138a0();
  if ((char)unaff_D6 != '\0') {
    uVar8 = uRam0010c034;
    if ((uRam0010c024._0_2_ == -1) || (uRam0010c024._2_2_ == -1)) goto LAB_00013a08;
    iVar4 = unaff_D2;
    iVar5 = unaff_D3;
    if (extraout_A0 == iRam0010c044) {
      if ((unaff_D2 == iRam0010c048) && (unaff_D3 == iRam0010c04c)) goto LAB_00013a08;
    }
    else {
      iVar4 = iRam0010c048;
      iVar5 = iRam0010c04c;
      if (iRam0010c044 < 0) {
        iRam0010c044 = extraout_A0;
        iVar4 = unaff_D2;
        iVar5 = unaff_D3;
      }
    }
    iRam0010c04c = iVar5;
    iRam0010c048 = iVar4;
    iRam0010c050 = CONCAT22(0xffff,iRam0010c050._2_2_);
  }
  candidate_1381c();
  candidate_1382a();
  if (extraout_A1 != 0) {
    do {
      uVar9 = candidate_1ac4c();
      uVar8 = uRam0010c034;
      if (extraout_A1_00 != iRam0010c050) {
        iRam0010c03c = unaff_D2;
        iRam0010c040 = unaff_D3;
        uRam0010c034 = uVar9;
        candidate_13776();
        if (((cRam0010c009 == '\0') || ((uRam0010c012 & 0x20) == 0)) &&
           (uVar6 = uRam0010c016, (short)uRam0010c016 != 0)) {
          candidate_129ec();
          uVar9 = candidate_12cba();
          iVar4 = extraout_A0_00;
          uVar8 = uRam0010c034;
          if (unaff_D6 < 0x80000001) goto LAB_00013aa2;
LAB_00013ae2:
          if ((int)uRam0010c05c <= (int)unaff_D6) {
            uRam0010c05c = unaff_D6;
            iRam0010c060 = iVar4;
            uRam0010c064 = uVar6;
          }
        }
        else {
LAB_00013aa2:
          uVar8 = candidate_12938();
          uVar1 = (uint)((ulonglong)uVar8 >> 0x20);
          uVar2 = (ushort)((ulonglong)uVar8 >> 0x10);
          uVar3 = CONCAT22(uVar2,(short)uVar8 + 8) << 0x10 | (uint)uVar2;
          uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
          iVar4 = extraout_A0_01;
          uVar8 = uRam0010c034;
          uVar9 = CONCAT44(uVar1,uVar3);
          if (((short)uVar6 != 0) &&
             ((uRam0010c016._2_2_ != 0x3c || (uVar9 = CONCAT44(uVar1,uVar3), (short)uVar6 != 0x3c)))
             ) {
            candidate_129ec();
            uVar9 = candidate_12cba();
            iVar4 = extraout_A0_02;
            uVar8 = uRam0010c034;
            if (unaff_D6 != 0x80000000) {
              unaff_D6 = unaff_D6 - 0x80000;
              goto LAB_00013ae2;
            }
          }
        }
        uRam0010c034._0_4_ = (int)((ulonglong)uVar8 >> 0x20);
        uRam0010c034._4_4_ = (int)uVar8;
        unaff_D2 = (int)((ulonglong)uVar9 >> 0x20) - uRam0010c034._0_4_;
        unaff_D3 = (int)uVar9 - uRam0010c034._4_4_;
        if ((unaff_D2 != iRam0010c03c) || (unaff_D3 != iRam0010c040)) goto LAB_00013b12;
      }
      uRam0010c034 = uVar8;
      candidate_1382a();
      if (extraout_A1_01 == 0) break;
    } while( true );
  }
  goto LAB_00013b82;
LAB_00013b12:
  if ((uRam0010c024._0_2_ == -1) || (uRam0010c024._2_2_ == -1)) goto LAB_00013a08;
  iVar5 = unaff_D2;
  iVar7 = unaff_D3;
  if (iVar4 == iRam0010c044) {
    if ((unaff_D2 == iRam0010c048) && (unaff_D3 == iRam0010c04c)) goto LAB_00013a08;
  }
  else {
    iVar5 = iRam0010c048;
    iVar7 = iRam0010c04c;
    if (iRam0010c044 < 0) {
      iRam0010c044 = iVar4;
      iVar5 = unaff_D2;
      iVar7 = unaff_D3;
    }
  }
  iRam0010c04c = iVar7;
  iRam0010c048 = iVar5;
  uRam0010c054 = uRam0010c064;
  uRam0010c058 = uRam0010c05c;
  iRam0010c050 = iVar4;
  goto LAB_000139c0;
LAB_00013a08:
  uRam0010c012 = uRam0010c012 | 0x80;
  uRam0010c034 = uVar8;
LAB_00013b82:
  uVar6 = uRam0010c064;
  uVar1 = uRam0010c05c;
  uVar8 = candidate_12e64();
  iVar5 = (int)((ulonglong)uVar8 >> 0x20);
  iVar4 = (int)uVar8;
  if (uVar1 == 0xffffffff) {
LAB_00013bba:
    uRam0010c012 = uRam0010c012 | 1;
    iVar7 = extraout_A1_02;
    if ((short)((uint)uVar6 >> 0x10) != 9) goto LAB_00013bd2;
  }
  else if ((cRam0010c009 != '\0') && (uVar1 != 0x80000000)) {
    iVar4 = uVar1 + 1 + iVar4;
    goto LAB_00013bba;
  }
  iVar7 = 0x10e968;
LAB_00013bd2:
  if (extraout_A0_03 != 0) {
    iVar5 = *(int *)(extraout_A0_03 + 0x14) + iVar5;
    iVar4 = *(int *)(extraout_A0_03 + 0x18) + iVar4;
  }
  if (iVar7 != 0) {
    iVar5 = iVar5 - *(int *)(iVar7 + 0x14);
    iVar4 = iVar4 - *(int *)(iVar7 + 0x18);
  }
  return CONCAT44(iVar5,iVar4);
}
