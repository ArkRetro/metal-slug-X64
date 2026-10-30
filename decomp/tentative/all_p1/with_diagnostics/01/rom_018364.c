/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x018364.
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

void candidate_18364(void)

{
  code *pcVar1;
  ushort uVar2;
  int iVar3;
  undefined1 uVar5;
  short sVar4;
  ushort extraout_D1w;
  ushort uVar9;
  int iVar6;
  int iVar7;
  uint uVar8;
  ushort unaff_D3w;
  short unaff_D7w;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int extraout_A0_05;
  undefined4 unaff_A4;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;
  undefined4 uStack_4;

code_r0x00018364:
  if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_00018cf8;
LAB_0001836c:
  unaff_D3w = 0;
  uStack_4 = 0x1837c;
  candidate_9c0a();
LAB_000183d0:
  uStack_4 = 0x183d6;
  candidate_3d4ee();
  uStack_4 = 0x183dc;
  candidate_4192();
  uStack_4 = 0x183e8;
  candidate_420c();
  uStack_4 = 0x183ee;
  candidate_c0cd6();
  if (*(short *)(unaff_A5 + 0x42fa) == 0) {
    uStack_4 = 0x183fa;
    func_0x0001805c();
  }
  else {
    uStack_4 = 0x18402;
    func_0x00018080();
  }
  uStack_4 = 0x1840a;
  candidate_ca2f2();
  uStack_4 = 0x18412;
  candidate_ca2f2();
  sVar4 = *(short *)(*(int *)(*(int *)((int)unaff_A6 + 0x6e) + 10) + 0xc);
  if ((sVar4 != 0) && (sVar4 != *(short *)(unaff_A6 + 0x20))) {
    *(short *)(unaff_A6 + 0x20) = sVar4;
    uStack_4 = 0x18434;
    candidate_120f8();
  }
  *(undefined2 *)(unaff_A5 + 0x42f6) =
       *(undefined2 *)(*(int *)(*(int *)((int)unaff_A6 + 0x6e) + 10) + 0xe);
  uStack_4 = 0x18458;
  candidate_19e04();
  uStack_4 = 0x1846c;
  candidate_c8774();
  unaff_A4 = *(undefined4 *)(*(int *)((int)unaff_A6 + 0x6e) + 10);
  uStack_4 = 0x18482;
  candidate_19604();
  uStack_4 = 0x18488;
  uVar5 = candidate_3586();
  *(undefined1 *)((int)unaff_A6 + 0x7a) = uVar5;
  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xa0000;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0xa0000;
  uStack_4 = 0x184a8;
  candidate_1963a();
  *(undefined2 *)(unaff_A6 + 0x1f) = 1;
  *unaff_A6 = 0x184b4;
  sVar4 = *(short *)(unaff_A6 + 0x1f);
  *(short *)(unaff_A6 + 0x1f) = sVar4 + -1;
  if (sVar4 == 0) {
                    /* WARNING: Could not recover jumptable at 0x000184c4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(*(short *)(*(int *)((int)unaff_A6 + 0x6e) + 6) + 0x184c8))();
    return;
  }
LAB_00018cf8:
  do {
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
      *(undefined2 *)(unaff_A5 + 0x42f8) = *(undefined2 *)(unaff_A5 + 0x42fc);
      *(undefined2 *)(unaff_A5 + 0x42fa) = *(undefined2 *)(unaff_A5 + 0x42fe);
LAB_0001829e:
      *(undefined2 *)(unaff_A6 + 0x20) = 0xffff;
      do {
        uStack_4 = 0x182ae;
        candidate_1824a();
        *(undefined4 *)((int)unaff_A6 + 0x6e) = unaff_A4;
                    /* WARNING: Switch is manually overridden */
        switch(**(undefined2 **)((int)unaff_A6 + 0x6e)) {
        case 0:
          uStack_4 = 0x182ee;
          candidate_c8cd6();
          uStack_4 = 0x182f4;
          candidate_c09d8();
          *(undefined1 *)(unaff_A5 + 0x4302) = 0;
          DAT_0010c2e6 = 0;
          uStack_4 = 0x18308;
          candidate_c8eaa();
                    /* WARNING: Switch is manually overridden */
          switch(*(undefined2 *)(*(int *)((int)unaff_A6 + 0x6e) + 4)) {
          case 0:
            uStack_4 = 0x1832a;
            candidate_164f0();
            unaff_D3w = 0;
            uStack_4 = 0x1833a;
            candidate_9c0a();
            break;
          case 4:
            uStack_4 = 0x18344;
            candidate_15418();
            *(undefined1 *)(unaff_A6 + 0x18) = 0;
            bVar10 = false;
            uStack_4 = 0x1835a;
            candidate_40bc();
            if (!bVar10) goto LAB_0001836c;
            *unaff_A6 = candidate_18364;
            goto code_r0x00018364;
          case 8:
            unaff_D3w = 0x10;
            *(undefined2 *)(unaff_A6 + 0x1f) = 0x10;
            uStack_4 = 0x18394;
            candidate_9c0a();
            *unaff_A6 = candidate_1839a;
            sVar4 = *(short *)(unaff_A6 + 0x1f);
            *(short *)(unaff_A6 + 0x1f) = sVar4 + -1;
            if (sVar4 != 0) goto LAB_00018cf8;
            uStack_4 = 0x183a8;
            candidate_164f0();
            break;
          case 0xc:
            uStack_4 = 0x183bc;
            candidate_40bc();
            *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)(unaff_A5 + 0x4310);
            *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(unaff_A5 + 0x4312);
            *unaff_A6 = 0x183d0;
            return;
          }
          goto LAB_000183d0;
        case 4:
          sVar4 = (*(undefined2 **)((int)unaff_A6 + 0x6e))[2];
          if (sVar4 == 0) goto LAB_00018846;
          *(short *)(unaff_A5 + 0x42fa) = sVar4;
          break;
        case 8:
LAB_00018846:
          uStack_4 = 0x1884c;
          candidate_c8cd6();
          *(undefined1 *)(unaff_A5 + 0x4301) = 0xff;
          uStack_4 = 0x18858;
          candidate_15418();
          if (DAT_0010c2e7 != '\x01') {
            *(undefined2 *)(unaff_A6 + 0x1f) = 0x1e;
            *unaff_A6 = candidate_18870;
            sVar4 = *(short *)(unaff_A6 + 0x1f) + -1;
            *(short *)(unaff_A6 + 0x1f) = sVar4;
            if (sVar4 != 0) goto LAB_00018cf8;
            if (DAT_0010c2e7 == '\0') {
              uStack_4 = 0x1888e;
              candidate_cae6c();
              uStack_4 = 0x18894;
              candidate_15418();
              *(undefined1 *)(unaff_A6 + 0x18) = 0;
              uStack_4 = 0x188aa;
              candidate_40bc();
              *unaff_A6 = 0x188b0;
              if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_00018cf8;
              uStack_4 = 0x188be;
              candidate_15422();
            }
            uStack_4 = 0x188c8;
            candidate_120f8();
            uStack_4 = 0x188d2;
            candidate_120f8();
            *(undefined2 *)(unaff_A6 + 0x1f) = 0xa0;
            *(undefined1 *)(unaff_A6 + 0x18) = 0;
            uStack_4 = 0x188ee;
            candidate_40bc();
            *unaff_A6 = 0x188f4;
            sVar4 = *(short *)(unaff_A6 + 0x1f);
            *(short *)(unaff_A6 + 0x1f) = sVar4 + -1;
            if (sVar4 != 0) goto LAB_00018cf8;
            unaff_D3w = 0x20;
            *(undefined2 *)(unaff_A6 + 0x1f) = 0x20;
            uStack_4 = 0x18910;
            candidate_9c0a();
            *unaff_A6 = 0x18916;
            sVar4 = *(short *)(unaff_A6 + 0x1f);
            *(short *)(unaff_A6 + 0x1f) = sVar4 + -1;
            if (sVar4 != 0) goto LAB_00018cf8;
            sVar4 = *(short *)(FUN_000042f4 + unaff_A5) + 1;
            *(short *)(FUN_000042f4 + unaff_A5) = sVar4;
            if ((byte)sVar4 < 6) goto code_r0x00018930;
            if (DAT_0010c2e7 == '\0') {
              uStack_4 = 0x18954;
              candidate_cae6c();
            }
          }
          *(undefined1 *)(unaff_A5 + 0x430e) = 0xff;
          if (thunk_FUN_0000447c[unaff_A5 + 3] != (code)0x0) {
            unaff_D3w = 6;
            uStack_4 = 0x18972;
            candidate_9c0a();
            uStack_4 = 0x18ab0;
            candidate_15418();
            uStack_4 = 0x18ab6;
            candidate_154f8();
            *unaff_A6 = 0x18cf8;
            goto LAB_00018cf8;
          }
          *(undefined1 *)(unaff_A5 + 0x7003) = 0;
          uStack_4 = 0x1898a;
          candidate_27c4();
          uStack_4 = 0x18998;
          candidate_27c4();
          uStack_4 = 0x1899e;
          candidate_19632();
          uStack_4 = 0x189a2;
          func_0x0001805c();
          uStack_4 = 0x189a8;
          candidate_4192();
          uStack_4 = 0x189b4;
          candidate_420c();
          uStack_4 = 0x189ba;
          candidate_c0cd6();
          uStack_4 = 0x189c0;
          candidate_c8eaa();
          uStack_4 = 0x189c6;
          candidate_16502();
          uStack_4 = 0x189cc;
          candidate_15418();
          uStack_4 = 0x189d8;
          candidate_1aa28();
          bVar10 = false;
          uStack_4 = 0x189e8;
          candidate_40bc();
          *(char *)(unaff_A6 + 0x18) = -!bVar10;
          *unaff_A6 = 0x189f2;
          if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_00018cf8;
          *(undefined2 *)(unaff_A5 + 0x42f8) = 0xc;
          *(undefined2 *)(unaff_A5 + 0x42fa) = 0;
          break;
        }
      } while( true );
    }
    if (*(char *)(unaff_A5 + 0x4309) == '\0') {
      uVar5 = false;
      if (*(char *)(unaff_A5 + 0x4300) == '\0') {
        uStack_4 = 0x18d3e;
        func_0x000040ac();
        if ((bool)uVar5) {
          if (*(char *)(extraout_A0_00 + 0x7b) != '\0') {
            uVar5 = false;
            uStack_4 = 0x18d52;
            candidate_ca2f8();
            if (!(bool)uVar5) goto LAB_00018d56;
          }
          goto LAB_00018d8e;
        }
LAB_00018d56:
        uStack_4 = 0x18d60;
        func_0x000040ac();
        if ((bool)uVar5) {
          if (*(char *)(extraout_A0_01 + 0x7b) != '\0') {
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
        uVar5 = false;
      }
      else {
        uVar9 = extraout_D1w;
        if ((short)unaff_D3w < (short)extraout_D1w) {
          uVar9 = unaff_D3w;
          unaff_D3w = extraout_D1w;
        }
        uVar2 = ((short)(unaff_D3w + uVar9) >> 1) - *(short *)(unaff_A5 + 0x69ba);
        if ((int)((uint)uVar2 << 0x10) < 0) {
          uVar9 = unaff_D3w - 0x1df;
          if ((int)((uint)uVar9 << 0x10) < 0) {
            if ((short)uVar9 < (short)uVar2) {
              uVar9 = uVar2;
            }
          }
          else {
            uVar9 = 0;
          }
        }
        else if ((int)((uint)uVar9 << 0x10) < 0) {
          uVar9 = 0;
        }
        else if ((short)uVar2 < (short)uVar9) {
          uVar9 = uVar2;
        }
        if (((DAT_0010f000 & 8) != 0) || ((DAT_0010f000 & 0x10) != 0)) {
          uVar9 = uVar9 & 0xfff8;
        }
        iVar6 = (short)uVar9 * 0x3000;
        iVar7 = *(int *)((int)unaff_A6 + 0x76);
        if ((iVar6 - iVar7 == 0 || iVar6 < iVar7) && (iVar3 = -iVar7, iVar7 = iVar6, iVar6 < iVar3))
        {
          iVar7 = iVar3;
        }
        uVar5 = false;
        if (iVar7 != 0) {
          if (iVar7 < 0) {
            uVar8 = -*(int *)(unaff_A5 + 0x6974);
            if (*(int *)(unaff_A5 + 0x6974) < 1) {
              uVar8 = 0;
            }
            uVar5 = CARRY4((int)(iVar7 - uVar8) >> 2,uVar8);
          }
          else {
            uVar8 = -*(int *)(unaff_A5 + 0x6974);
            if (0 < *(int *)(unaff_A5 + 0x6974)) {
              uVar8 = 0;
            }
            uVar5 = CARRY4((int)(iVar7 - uVar8) >> 2,uVar8);
          }
        }
        register0x0000003c = (BADSPACEBASE *)&uStack_4;
        uStack_4 = 0x18f10;
      }
      if ((bRam0010e95a & 1) != 0) {
        uVar5 = false;
      }
      if ((bRam0010e95a & 2) != 0) {
        uVar5 = false;
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc81e6;
      candidate_c7ca6();
      if ((bRam0010e956 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8238;
        candidate_c6f9e();
        uVar8 = *(uint *)(extraout_A0_02 + 0x18);
        uVar5 = CARRY4(uRam0010e9cc,uVar8);
        *(uint *)(extraout_A0_02 + 0x18) = uRam0010e9cc + uVar8;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc81fc;
        candidate_1ac38();
      }
      if ((bRam0010e957 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8292;
        candidate_c6f9e();
        uVar8 = *(uint *)(extraout_A0_03 + 0x18);
        uVar5 = CARRY4(uRam0010e9cc,uVar8);
        *(uint *)(extraout_A0_03 + 0x18) = uRam0010e9cc + uVar8;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8256;
        func_0x0001ac3c();
      }
      if ((bRam0010e958 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc82ec;
        candidate_c6f9e();
        uVar8 = *(uint *)(extraout_A0_04 + 0x18);
        uVar5 = CARRY4(uRam0010e9cc,uVar8);
        *(uint *)(extraout_A0_04 + 0x18) = uRam0010e9cc + uVar8;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc82b0;
        func_0x0001ac3c();
      }
      if ((bRam0010e959 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8346;
        candidate_c6f9e();
        uVar8 = *(uint *)(extraout_A0_05 + 0x18);
        uVar5 = CARRY4(uRam0010e9cc,uVar8);
        *(uint *)(extraout_A0_05 + 0x18) = uRam0010e9cc + uVar8;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc830a;
        func_0x0001ac3c();
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8352;
      candidate_c804a();
      if ((DAT_0010f000 & 8) == 0) {
LAB_000c836c:
        if ((DAT_0010f000 & 0x10) != 0) {
          *(undefined4 *)((int)register0x0000003c + -4) = 0xc8382;
          candidate_c8abc();
          if ((bool)uVar5) goto LAB_000c8386;
        }
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8368;
        candidate_c8abc();
        if (!(bool)uVar5) goto LAB_000c836c;
LAB_000c8386:
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
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc83c2;
      candidate_c7480();
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc83ce;
      candidate_c7480();
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc83da;
      candidate_c7480();
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc83e6;
      candidate_c7480();
      sVar4 = *psRam0010e964;
      goto LAB_000c83ec;
    }
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
      iVar7 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar7 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
  } while( true );
code_r0x00018930:
  *(short *)(unaff_A5 + 0x42f8) = sVar4;
  *(undefined2 *)(unaff_A5 + 0x42fa) = 0;
  goto LAB_0001829e;
LAB_000c83ec:
  switch(sVar4) {
  case 0:
    uVar9 = psRam0010e964[2];
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84b2;
    candidate_c7ebc();
    if (_DAT_0010e99e < (uint)uVar9 * 0x10000) goto LAB_000c840c;
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
    uVar5 = false;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84e4;
    func_0x000040ac();
    if ((bool)uVar5) {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84ec;
      candidate_c8524();
      if (!(bool)uVar5) goto LAB_000c840c;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84fa;
      func_0x000040ac();
      if (!(bool)uVar5) break;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8502;
      candidate_c8524();
    }
    else {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8514;
      func_0x000040ac();
      if (!(bool)uVar5) goto LAB_000c840c;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc851c;
      candidate_c8524();
    }
    if (!(bool)uVar5) {
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
  goto LAB_000c83ec;
}
