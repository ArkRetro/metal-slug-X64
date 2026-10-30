/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x018aa2.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Possible PIC construction at 0x00018f0a: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x00018f10) */
/* WARNING: Removing unreachable block (ram,0x0001406a) */
/* WARNING: Removing unreachable block (ram,0x00014090) */
/* WARNING: Removing unreachable block (ram,0x000140a2) */
/* WARNING: Removing unreachable block (ram,0x000140b0) */
/* WARNING: Removing unreachable block (ram,0x00014098) */
/* WARNING: Removing unreachable block (ram,0x000140b4) */
/* WARNING: Removing unreachable block (ram,0x000140be) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_18aa2(void)

{
  code *pcVar1;
  ushort uVar2;
  int iVar3;
  short sVar4;
  ushort extraout_D1w;
  ushort uVar8;
  int iVar5;
  int iVar6;
  uint uVar7;
  ushort unaff_D3w;
  short unaff_D7w;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  undefined4 unaff_A4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;
  bool bVar10;
  undefined4 uStack_4;

  sVar4 = *(short *)(unaff_A6 + 0x1f) + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar4;
  if (sVar4 == 0) {
    uStack_4 = 0x18ab0;
    candidate_15418();
    uStack_4 = 0x18ab6;
    candidate_154f8();
    *unaff_A6 = 0x18cf8;
  }
  while( true ) {
    uStack_4 = 0x18cfe;
    candidate_ca21e();
    uStack_4 = 0x18d04;
    candidate_469c();
    uStack_4 = 0x18d0a;
    candidate_8bfd6();
    uStack_4 = 0x18d10;
    candidate_19658();
    uStack_4 = 0x18d16;
    candidate_19e40();
    uStack_4 = 0x18d1c;
    candidate_401e8();
    if (*(char *)(unaff_A5 + 0x4308) != '\0') {
      uStack_4 = 0x1828e;
      candidate_18234();
      *(undefined1 *)(unaff_A5 + 0x4308) = 0;
      *(undefined2 *)(FUN_000042f4 + unaff_A5 + 4) = *(undefined2 *)(unaff_A5 + 0x42fc);
      *(undefined2 *)(unaff_A5 + 0x42fa) = *(undefined2 *)(unaff_A5 + 0x42fe);
      *(undefined2 *)(unaff_A6 + 0x20) = 0xffff;
      uStack_4 = 0x182ae;
      candidate_1824a();
      *(undefined4 *)((int)unaff_A6 + 0x6e) = unaff_A4;
                    /* WARNING: Could not recover jumptable at 0x000182b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(**(short **)((int)unaff_A6 + 0x6e) + 0x182bc))();
      return;
    }
    if (*(char *)(unaff_A5 + 0x4309) == '\0') break;
    DAT_0010c309 = 0;
    uStack_4 = 0x18c6c;
    candidate_c09d8();
    unaff_D3w = 0xc;
    uStack_4 = 0x18c7a;
    candidate_9c0a();
    *(undefined2 *)(unaff_A6 + 0x1f) = 0x14;
    *unaff_A6 = 0x18c86;
    sVar4 = *(short *)(unaff_A6 + 0x1f) + -1;
    *(short *)(unaff_A6 + 0x1f) = sVar4;
    if (sVar4 == 0) {
      uStack_4 = 0x18c94;
      candidate_4192();
      uStack_4 = 0x18ca0;
      candidate_420c();
      uStack_4 = 0x18cac;
      candidate_41e8();
      uStack_4 = 0x18cb8;
      candidate_41e8();
      uStack_4 = 0x18cc6;
      candidate_27c4();
      uStack_4 = 0x18cd4;
      candidate_27c4();
      uStack_4 = 0x18cda;
      candidate_c0cd6();
      uStack_4 = 0x18ce0;
      candidate_16502();
      uStack_4 = 0x18cec;
      candidate_1aa28();
      uStack_4 = 0x18cf2;
      candidate_c8eaa();
      uStack_4 = 0x418a;
      candidate_4126();
      iVar6 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar6 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
  }
  uVar9 = false;
  if (*(char *)(unaff_A5 + 0x4300) == '\0') {
    uStack_4 = 0x18d3e;
    func_0x000040ac();
    if ((bool)uVar9) {
      if (*(char *)(extraout_A0 + 0x7b) != '\0') {
        uVar9 = false;
        uStack_4 = 0x18d52;
        candidate_ca2f8();
        if (!(bool)uVar9) goto LAB_00018d56;
      }
      goto LAB_00018d8e;
    }
LAB_00018d56:
    uStack_4 = 0x18d60;
    func_0x000040ac();
    if ((bool)uVar9) {
      if (*(char *)(extraout_A0_00 + 0x7b) != '\0') {
        bVar10 = false;
        uStack_4 = 0x18d74;
        candidate_ca2f8();
        if (!bVar10) goto LAB_00018d78;
      }
      goto LAB_00018d8e;
    }
LAB_00018d78:
    if (DAT_0010109e == '\0') goto LAB_00018d8e;
    DAT_0010109f = 0xff;
  }
  else {
LAB_00018d8e:
    DAT_0010109f = 0;
  }
  uStack_4 = 0x18d9a;
  candidate_1903a();
  if (unaff_D7w == 0) {
    uVar9 = false;
  }
  else {
    uVar8 = extraout_D1w;
    if ((short)unaff_D3w < (short)extraout_D1w) {
      uVar8 = unaff_D3w;
      unaff_D3w = extraout_D1w;
    }
    uVar2 = ((short)(unaff_D3w + uVar8) >> 1) - *(short *)(unaff_A5 + 0x69ba);
    if ((int)((uint)uVar2 << 0x10) < 0) {
      uVar8 = unaff_D3w - 0x1df;
      if ((int)((uint)uVar8 << 0x10) < 0) {
        if ((short)uVar8 < (short)uVar2) {
          uVar8 = uVar2;
        }
      }
      else {
        uVar8 = 0;
      }
    }
    else if ((int)((uint)uVar8 << 0x10) < 0) {
      uVar8 = 0;
    }
    else if ((short)uVar2 < (short)uVar8) {
      uVar8 = uVar2;
    }
    if (((DAT_0010f000 & 8) != 0) || ((DAT_0010f000 & 0x10) != 0)) {
      uVar8 = uVar8 & 0xfff8;
    }
    iVar5 = (short)uVar8 * 0x3000;
    iVar6 = *(int *)((int)unaff_A6 + 0x76);
    if ((iVar5 - iVar6 == 0 || iVar5 < iVar6) && (iVar3 = -iVar6, iVar6 = iVar5, iVar5 < iVar3)) {
      iVar6 = iVar3;
    }
    uVar9 = false;
    if (iVar6 != 0) {
      if (iVar6 < 0) {
        uVar7 = -*(int *)(unaff_A5 + 0x6974);
        if (*(int *)(unaff_A5 + 0x6974) < 1) {
          uVar7 = 0;
        }
        uVar9 = CARRY4((int)(iVar6 - uVar7) >> 2,uVar7);
      }
      else {
        uVar7 = -*(int *)(unaff_A5 + 0x6974);
        if (0 < *(int *)(unaff_A5 + 0x6974)) {
          uVar7 = 0;
        }
        uVar9 = CARRY4((int)(iVar6 - uVar7) >> 2,uVar7);
      }
    }
    register0x0000003c = (BADSPACEBASE *)&uStack_4;
    uStack_4 = 0x18f10;
  }
  if ((bRam0010e95a & 1) != 0) {
    uVar9 = false;
  }
  if ((bRam0010e95a & 2) != 0) {
    uVar9 = false;
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc81e6;
  candidate_c7ca6();
  if ((bRam0010e956 & 0x80) == 0) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8238;
    candidate_c6f9e();
    uVar7 = *(uint *)(extraout_A0_01 + 0x18);
    uVar9 = CARRY4(uRam0010e9cc,uVar7);
    *(uint *)(extraout_A0_01 + 0x18) = uRam0010e9cc + uVar7;
  }
  else {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc81fc;
    candidate_1ac38();
  }
  if ((bRam0010e957 & 0x80) == 0) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8292;
    candidate_c6f9e();
    uVar7 = *(uint *)(extraout_A0_02 + 0x18);
    uVar9 = CARRY4(uRam0010e9cc,uVar7);
    *(uint *)(extraout_A0_02 + 0x18) = uRam0010e9cc + uVar7;
  }
  else {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8256;
    func_0x0001ac3c();
  }
  if ((bRam0010e958 & 0x80) == 0) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc82ec;
    candidate_c6f9e();
    uVar7 = *(uint *)(extraout_A0_03 + 0x18);
    uVar9 = CARRY4(uRam0010e9cc,uVar7);
    *(uint *)(extraout_A0_03 + 0x18) = uRam0010e9cc + uVar7;
  }
  else {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc82b0;
    func_0x0001ac3c();
  }
  if ((bRam0010e959 & 0x80) == 0) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8346;
    candidate_c6f9e();
    uVar7 = *(uint *)(extraout_A0_04 + 0x18);
    uVar9 = CARRY4(uRam0010e9cc,uVar7);
    *(uint *)(extraout_A0_04 + 0x18) = uRam0010e9cc + uVar7;
  }
  else {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc830a;
    func_0x0001ac3c();
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc8352;
  candidate_c804a();
  if ((DAT_0010f000 & 8) == 0) {
LAB_000c836c:
    if ((DAT_0010f000 & 0x10) == 0) goto LAB_000c83b6;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8382;
    candidate_c8abc();
    if (!(bool)uVar9) goto LAB_000c83b6;
  }
  else {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8368;
    candidate_c8abc();
    if (!(bool)uVar9) goto LAB_000c836c;
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc838a;
  candidate_c8af8();
  if ((DAT_0010f000 & 8) != 0) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc83a0;
    candidate_c8b0e();
  }
  if ((DAT_0010f000 & 0x10) != 0) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc83b6;
    candidate_c8b0e();
  }
LAB_000c83b6:
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc83c2;
  candidate_c7480();
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc83ce;
  candidate_c7480();
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc83da;
  candidate_c7480();
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc83e6;
  candidate_c7480();
  sVar4 = *psRam0010e964;
  do {
    switch(sVar4) {
    case 0:
      uVar8 = psRam0010e964[2];
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84b2;
      candidate_c7ebc();
      if (_DAT_0010e99e < (uint)uVar8 * 0x10000) goto LAB_000c840c;
      break;
    case 1:
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    case 2:
      *(ushort *)((int)unaff_A6 + -2) = *(ushort *)((int)unaff_A6 + -2) | 0x6000;
    case 6:
      uRam000c855a = uRam000c855a | 0x6000;
      return;
    case 3:
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    case 4:
      uVar9 = false;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84e4;
      func_0x000040ac();
      if ((bool)uVar9) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc84ec;
        candidate_c8524();
        if (!(bool)uVar9) goto LAB_000c840c;
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc84fa;
        func_0x000040ac();
        if (!(bool)uVar9) break;
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8502;
        candidate_c8524();
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8514;
        func_0x000040ac();
        if (!(bool)uVar9) goto LAB_000c840c;
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc851c;
        candidate_c8524();
      }
      if (!(bool)uVar9) {
LAB_000c840c:
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8418;
        func_0x000c748e();
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8424;
        func_0x000c748e();
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8430;
        func_0x000c748e();
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc843c;
        func_0x000c748e();
        return;
      }
      break;
    case 5:
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    case 7:
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    case 8:
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8568;
      candidate_c7ebc();
      goto LAB_000c840c;
    case 9:
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    case 10:
      return;
    case 0xb:
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    default:
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc83f8;
      (*(code *)(sVar4 + 0xc844a))();
    }
    psRam0010e964 = (short *)((int)psRam0010e964[1] + (int)psRam0010e964);
    sVar4 = *psRam0010e964;
  } while( true );
}
