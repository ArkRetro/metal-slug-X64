/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0122f6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_122f6(void)

{
  ushort *puVar1;
  short *psVar2;
  short sVar3;
  byte bVar4;
  undefined3 uVar5;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined4 uVar6;
  uint uVar7;
  undefined4 *unaff_A6;

  candidate_3d244();
  candidate_3d244();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x20;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x20;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
  *unaff_A6 = 0x12356;
  uVar6 = candidate_3d244();
  bVar4 = DAT_0010c35d;
  uVar5 = (undefined3)((uint)uVar6 >> 8);
  if ((DAT_0010c35d & 0x40) == 0) {
    if ((DAT_0010c35d & 0x80) == 0) {
      if ((DAT_0010c35d & 1) == 0) {
        if ((DAT_0010c35d & 2) == 0) {
          if ((DAT_0010c35d & 4) == 0) {
            if ((DAT_0010c35d & 8) != 0) {
              sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
              psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
              *psVar2 = *psVar2 + 0x10;
              if (0xff < *(short *)((int)unaff_A6 + sVar3 + 0x6e)) {
                puVar1 = (ushort *)((int)unaff_A6 + sVar3 + 0x6e);
                *puVar1 = *puVar1 & 0xf;
                psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
                *psVar2 = *psVar2 + 0x20;
              }
            }
          }
          else {
            sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
            psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
            *psVar2 = *psVar2 + -0x10;
            if (*(short *)((int)unaff_A6 + sVar3 + 0x6e) < 0x20) {
              puVar1 = (ushort *)((int)unaff_A6 + sVar3 + 0x6e);
              *puVar1 = *puVar1 & 0xf;
              psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
              *psVar2 = *psVar2 + 0xf0;
            }
          }
        }
        else {
          sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
          psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
          *psVar2 = *psVar2 + -1;
          if (*(short *)((int)unaff_A6 + sVar3 + 0x6e) < 0x20) {
            *(undefined2 *)((int)unaff_A6 + sVar3 + 0x6e) = 0xff;
          }
        }
      }
      else {
        sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
        psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
        *psVar2 = *psVar2 + 1;
        if (0xff < *(short *)((int)unaff_A6 + sVar3 + 0x6e)) {
          *(undefined2 *)((int)unaff_A6 + sVar3 + 0x6e) = 0x20;
        }
      }
      if ((bVar4 & 0x20) != 0) {
        sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
        psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x76);
        *psVar2 = *psVar2 + 1;
        if (2 < *(short *)((int)unaff_A6 + sVar3 + 0x76)) {
          *(undefined2 *)((int)unaff_A6 + sVar3 + 0x76) = 0;
        }
      }
      if ((bVar4 & 0x10) != 0) {
        *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 1;
        *(ushort *)((int)unaff_A6 + 0x7e) = *(ushort *)((int)unaff_A6 + 0x7e) & 3;
      }
    }
    else {
      candidate_1209c();
      uVar5 = extraout_var_00;
    }
  }
  else {
    candidate_1209c();
    uVar5 = extraout_var;
  }
  uVar7 = CONCAT31(uVar5,DAT_0010c363);
  if ((DAT_0010c363 & 1) == 0) {
    if ((DAT_0010c363 & 2) == 0) {
      uVar7 = CONCAT31(uVar5,DAT_0010c364);
      if ((DAT_0010c364 & 8) == 0) {
        if ((DAT_0010c364 & 4) == 0) {
          uVar7 = CONCAT31(uVar5,DAT_0010c363);
          if ((DAT_0010c363 & 0x10) != 0) {
            candidate_120f8();
          }
        }
        else {
          sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
          psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
          *psVar2 = *psVar2 + -1;
          if (*(short *)((int)unaff_A6 + sVar3 + 0x6e) < 0x20) {
            *(undefined2 *)((int)unaff_A6 + sVar3 + 0x6e) = 0xff;
          }
        }
      }
      else {
        sVar3 = *(short *)((int)unaff_A6 + 0x7e) * 2;
        psVar2 = (short *)((int)unaff_A6 + sVar3 + 0x6e);
        *psVar2 = *psVar2 + 1;
        if (0xff < *(short *)((int)unaff_A6 + sVar3 + 0x6e)) {
          *(undefined2 *)((int)unaff_A6 + sVar3 + 0x6e) = 0x20;
        }
      }
    }
    else {
      *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 1;
      *(ushort *)((int)unaff_A6 + 0x7e) = *(ushort *)((int)unaff_A6 + 0x7e) & 3;
    }
  }
  else {
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    *(ushort *)((int)unaff_A6 + 0x7e) = *(ushort *)((int)unaff_A6 + 0x7e) & 3;
  }
  if ((uVar7 & 0x20) != 0) {
    candidate_120f8();
  }
  if ((uVar7 & 0x40) != 0) {
    candidate_120f8();
  }
  if ((uVar7 & 0x80) != 0) {
    candidate_120f8(uVar7);
  }
  candidate_1264e();
  candidate_1264e();
  candidate_1264e();
  candidate_1264e();
  candidate_3d244();
  return;
}
