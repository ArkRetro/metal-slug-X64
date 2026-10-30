/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013db2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_13db2(void)

{
  int in_D0;
  int iVar1;
  int in_D1;
  undefined4 unaff_D3;
  uint unaff_D6;
  undefined4 unaff_D7;
  short sVar2;
  int in_A0;
  undefined4 extraout_A0;
  undefined4 extraout_A0_00;
  undefined4 uVar3;
  int extraout_A0_01;
  int extraout_A1;
  int extraout_A1_00;
  int unaff_A5;
  undefined8 uVar4;
  int iStack_4;

  *(int *)(unaff_A5 + 0x4034) = in_D0;
  *(int *)(unaff_A5 + 0x4038) = in_D1;
  *(int *)(unaff_A5 + 0x403c) = in_A0;
  *(undefined4 *)(unaff_A5 + 0x405c) = unaff_D3;
  *(undefined4 *)(unaff_A5 + 0x4064) = 0x4000;
  *(undefined2 *)(unaff_A5 + 0x4012) = 0;
  if (in_A0 != 0) {
    in_D0 = *(int *)(in_A0 + 0x14) + in_D0;
    in_D1 = *(int *)(in_A0 + 0x18) + in_D1;
  }
  *(int *)(unaff_A5 + 0x402c) = in_D0;
  *(int *)(unaff_A5 + 0x4030) = in_D1;
  candidate_1381c();
  candidate_1382a();
  iVar1 = extraout_A1;
  do {
    if (iVar1 == 0) {
      uVar3 = *(undefined4 *)(unaff_A5 + 0x4064);
      iVar1 = candidate_12e64();
      if ((short)((uint)uVar3 >> 0x10) == 9) {
        if (extraout_A0_01 != 0) {
          iVar1 = *(int *)(extraout_A0_01 + 0x14) + iVar1;
        }
        iVar1 = iVar1 - *(int *)(unaff_A5 + 0x697c);
      }
      return iVar1;
    }
    *(undefined4 *)(unaff_A5 + 0x4016) = 0;
    iStack_4 = 0;
    candidate_12938();
    *(undefined4 *)(unaff_A5 + 0x4016) = unaff_D7;
    candidate_129ec();
    uVar4 = candidate_12cba();
    uVar3 = extraout_A0;
    if (unaff_D6 < 0x80000001) {
      iStack_4 = -0x80000;
      do {
        candidate_12938();
        candidate_129ec();
        sVar2 = (short)unaff_D7;
        if ((((*(short *)(unaff_A5 + 0x4018) != 0x3c) || (sVar2 != 0x3c)) &&
            (*(short *)(unaff_A5 + 0x4018) = sVar2, sVar2 != 0)) &&
           (uVar4 = candidate_12cba(), uVar3 = extraout_A0_00, unaff_D6 != 0x80000000))
        goto LAB_00013e72;
        iStack_4 = iStack_4 + -0x80000;
      } while (*(int *)(unaff_A5 + 0x405c) <= iStack_4);
    }
    else {
LAB_00013e72:
      iVar1 = unaff_D6 + (int)uVar4;
      unaff_D6 = iStack_4 + unaff_D6;
      if (*(uint *)(unaff_A5 + 0x405c) < unaff_D6) {
        *(uint *)(unaff_A5 + 0x405c) = unaff_D6;
        *(undefined4 *)(unaff_A5 + 0x4064) = unaff_D7;
        *(int *)(unaff_A5 + 0x4034) = (int)((ulonglong)uVar4 >> 0x20);
        *(int *)(unaff_A5 + 0x4038) = iVar1 + 1;
        *(undefined4 *)(unaff_A5 + 0x403c) = uVar3;
        *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 1;
      }
    }
    candidate_1382a();
    iVar1 = extraout_A1_00;
  } while( true );
}
