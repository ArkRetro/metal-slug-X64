/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x018600.
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

void candidate_18600(void)

{
  code *pcVar1;
  ushort uVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  short sVar6;
  uint extraout_D1;
  ushort extraout_D1w;
  int iVar7;
  int iVar8;
  uint uVar9;
  byte bVar10;
  ushort unaff_D3w;
  short unaff_D7w;
  short *psVar11;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  undefined4 unaff_A4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar12;
  bool bVar13;
  undefined4 uStack_4;

  uStack_4 = 0x18606;
  candidate_16502();
  *(undefined1 *)(unaff_A5 + 0x4302) = 0xff;
  if (DAT_00100088 != '\x02') {
    uStack_4 = 0x18626;
    candidate_3b028();
  }
  if (-1 < *(short *)(*(int *)((int)unaff_A6 + 0x6e) + 0xe)) {
    if ((*(short *)(unaff_A5 + 0x42fa) == 0) && (DAT_00100088 == '\x02')) {
      uStack_4 = 0x18656;
      candidate_40bc();
    }
    uStack_4 = 0x18666;
    candidate_40bc();
    DAT_0010c2e6 = 0xff;
    uStack_4 = 0x18674;
    candidate_c8e50();
    uStack_4 = 0x1867a;
    candidate_c8ce6();
    uStack_4 = 0x18680;
    candidate_15422();
  }
  uStack_4 = 0x18690;
  candidate_4256();
  if (*(short *)(FUN_000042f4 + unaff_A5 + 2) == 0x2c) {
    *unaff_A6 = candidate_18ac6;
    if ((bRam00101473 & 0x80) != 0) {
LAB_00018b18:
      uStack_4 = 0x18b24;
      candidate_c8eaa();
      unaff_D3w = 0x1e;
      uStack_4 = 0x18b2c;
      candidate_9c0a();
      *(undefined2 *)(unaff_A6 + 0x1f) = 0x1e;
      *unaff_A6 = 0x18b38;
      sVar6 = *(short *)(unaff_A6 + 0x1f);
      *(short *)(unaff_A6 + 0x1f) = sVar6 + -1;
      if (sVar6 == 0) {
        uStack_4 = 0x18b46;
        candidate_4192();
        uStack_4 = 0x18b52;
        candidate_420c();
        uStack_4 = 0x18b58;
        candidate_c0cd6();
        uStack_4 = 0x18b5e;
        candidate_2852();
        uStack_4 = 0x18b64;
        candidate_16502();
        uStack_4 = 0x18b6a;
        candidate_3d4ee();
        uStack_4 = 0x18b74;
        candidate_1aa28();
        uStack_4 = 0x18b7e;
        candidate_c81a6();
        uStack_4 = 0x18b84;
        candidate_16502();
        unaff_D3w = 0x1e;
        uStack_4 = 0x18b94;
        candidate_9c0a();
        if (*(short *)(FUN_000042f4 + unaff_A5 + 2) == 0x2c) {
          uStack_4 = 0x18bae;
          candidate_40bc();
        }
        *(undefined2 *)(unaff_A6 + 0x1f) = 100;
        *unaff_A6 = 0x18bba;
        sVar6 = *(short *)(unaff_A6 + 0x1f);
        *(short *)(unaff_A6 + 0x1f) = sVar6 + -1;
        if (sVar6 == 0) {
          uStack_4 = 0x18bc8;
          candidate_c8eaa();
          DAT_0010109f = 0;
          uVar5 = *(ushort *)(unaff_A5 + 0x700c);
          *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
          if (0xfc10 < uVar5) {
            uVar5 = *(ushort *)(unaff_A5 + 0x700a);
            *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
            uVar4 = uRam002fffe8;
            uVar2 = ~uVar5 & 0xfff;
            bVar10 = -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0);
            uRam002fffe0 = (ushort)(bVar10 & -(0x7ff < uVar2));
            uRam002fffe2 = 0;
            uRam002fffe4 = 0;
            uRam002fffe6 = 0;
            *(short *)(unaff_A5 + 0x700a) =
                 ((short)-(uVar2 == 0x400) &
                 _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
                 *(short *)(unaff_A5 + 0x700a);
            if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (bVar10 ^ uVar4) & 1) != 0) {
              *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
              _DAT_002ffff0 = 1;
              do {
              } while (DAT_002fffff != '\x01');
              uRam00400000 = 0;
              uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
              uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
              uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
              uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
              uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
              uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
              uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
              uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
              uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
              uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
              uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
              uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
              uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
              uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
              uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
              _DAT_00401ffe = 0;
              uStack_4 = 0x3d2c;
              func_0x0003cf12();
              uStack_4 = 0x3d36;
              candidate_3e2a8();
              do {
                    /* WARNING: Do nothing block with infinite loop */
              } while( true );
            }
          }
          goto code_r0x00004188;
        }
      }
    }
  }
  else {
    *unaff_A6 = 100000;
    if (*(char *)(unaff_A5 + 0x4300) != '\0') {
      DAT_0010109f = 0;
    }
    if (DAT_00100088 == '\x02') {
      if (DAT_0010c2e7 == '\0') {
        if ((DAT_0010fd8a == '\0') || (*(short *)((int)unaff_A6 + 0x7e) == 0)) {
          bVar13 = false;
          uStack_4 = 0x186ec;
          candidate_c8e02();
          if (bVar13) {
            uStack_4 = 0x186f6;
            candidate_275a();
            if (!bVar13) goto LAB_00018a7c;
          }
        }
      }
      else if (*(short *)(unaff_A5 + 0x430a) != 0) {
LAB_00018a7c:
        if (DAT_0010c2e7 == '\0') {
          uStack_4 = 0x18ae0;
          candidate_12244();
          uStack_4 = 0x18ae6;
          candidate_12190();
          uStack_4 = 0x18af0;
          candidate_120f8();
          uStack_4 = 0x18af6;
          candidate_15418();
          unaff_D3w = 0x30;
          uStack_4 = 0x18b04;
          candidate_9c0a();
          *(undefined2 *)(unaff_A6 + 0x1f) = 0x20;
          *unaff_A6 = candidate_18b10;
          sVar6 = *(short *)(unaff_A6 + 0x1f);
          *(short *)(unaff_A6 + 0x1f) = sVar6 + -1;
          if (sVar6 == 0) goto LAB_00018b18;
        }
        else {
          DAT_0010109f = 0;
          uStack_4 = 0x18a96;
          candidate_15418();
          *(undefined2 *)(unaff_A6 + 0x1f) = 0x10;
          *unaff_A6 = candidate_18aa2;
          sVar6 = *(short *)(unaff_A6 + 0x1f) + -1;
          *(short *)(unaff_A6 + 0x1f) = sVar6;
          if (sVar6 == 0) {
            uStack_4 = 0x18ab0;
            candidate_15418();
            uStack_4 = 0x18ab6;
            candidate_154f8();
            *unaff_A6 = 0x18cf8;
          }
        }
        goto LAB_00018cf8;
      }
      uVar12 = 0;
      uStack_4 = 0x18704;
      candidate_c8fdc();
      uStack_4 = 0x18708;
      candidate_193fe();
      if ((bool)uVar12) {
        if (-1 < *(char *)(unaff_A5 + 0x4303)) {
          if (*(char *)(unaff_A5 + 0x7d8a) != '\0') {
            *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x9ff;
          }
          uStack_4 = 0x18726;
          candidate_18234();
          if (*(byte *)(unaff_A5 + 0x4303) == 0) {
            *(short *)(unaff_A5 + 0x42fa) = *(short *)(unaff_A5 + 0x42fa) + 1;
            psVar11 = (short *)((int)*(short *)(*(int *)((int)unaff_A6 + 0x6e) + 2) +
                               *(int *)((int)unaff_A6 + 0x6e));
            *(short **)((int)unaff_A6 + 0x6e) = psVar11;
          }
          else {
            *(ushort *)(unaff_A5 + 0x42fa) = (ushort)*(byte *)(unaff_A5 + 0x4303);
LAB_000182a4:
            uStack_4 = 0x182ae;
            candidate_1824a();
            *(undefined4 *)((int)unaff_A6 + 0x6e) = unaff_A4;
            psVar11 = *(short **)((int)unaff_A6 + 0x6e);
          }
                    /* WARNING: Could not recover jumptable at 0x000182b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (*(code *)(*psVar11 + 0x182bc))();
          return;
        }
        if ((DAT_0010c2e7 == '\0') && (bVar13 = false, *(char *)(unaff_A5 + 0x4300) == '\0')) {
          uStack_4 = 0x18752;
          FUN_000c8cfc();
          if ((!bVar13) &&
             ((*(char *)(unaff_A5 + 0x7003) != '\0' &&
              (uVar5 = *(ushort *)((int)unaff_A6 + 0x7a), uVar5 != 0xffff)))) {
            uVar2 = uVar5 - 2;
            if (uVar5 < 2 || uVar2 == 0) {
              *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
              uStack_4 = 0x1877c;
              candidate_c8fdc();
              uStack_4 = 0x18782;
              candidate_c0a2c();
              *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
              uStack_4 = 0x18a20;
              candidate_4256();
              *(undefined2 *)(unaff_A6 + 0xf) = 0xa0;
              *(undefined2 *)(unaff_A6 + 0x10) = 0x180;
              *(undefined2 *)(unaff_A6 + 0x1f) = 0x5a;
              *unaff_A6 = candidate_18a38;
              *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
              uStack_4 = 0x18a44;
              candidate_12022();
              uStack_4 = 0x18a4a;
              candidate_427a();
              sVar6 = *(short *)(unaff_A6 + 0x1f);
              *(short *)(unaff_A6 + 0x1f) = sVar6 + -1;
              if (sVar6 == 0) {
                uStack_4 = 0x18a58;
                uVar12 = candidate_3586();
                *(undefined1 *)((int)unaff_A6 + 0x7a) = uVar12;
                *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
                uStack_4 = 0x18a72;
                candidate_4256();
                *unaff_A6 = 100000;
              }
              goto LAB_00018cf8;
            }
            *(ushort *)((int)unaff_A6 + 0x7a) = uVar2;
            if ((uVar2 < 0xa00) && (0xfb < (byte)((char)uVar2 * '\x02'))) {
              uStack_4 = 0x187a4;
              candidate_120f8();
            }
          }
        }
      }
      if (*(short *)((int)unaff_A6 + 0x7e) != 0) {
        if (*(ushort *)((int)unaff_A6 + 0x7e) < 0x780) {
          uStack_4 = 0x187bc;
          candidate_190f0();
          if ((extraout_D1 & 0x20) == 0) goto LAB_000187da;
          uVar5 = (ushort)CONCAT31((uint3)(byte)((ushort)*(undefined2 *)((int)unaff_A6 + 0x7e) >> 8)
                                   ,0x80);
          sVar6 = uVar5 - 0x100;
          if (uVar5 < 0x100) {
            sVar6 = 0;
          }
        }
        else {
LAB_000187da:
          sVar6 = *(ushort *)((int)unaff_A6 + 0x7e) - 5;
          if (*(ushort *)((int)unaff_A6 + 0x7e) < 5) {
            sVar6 = 0;
          }
        }
        *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)((int)unaff_A6 + 0x7e);
        *(short *)((int)unaff_A6 + 0x7e) = sVar6;
        if ((((*(char *)(unaff_A5 + 0x7d8a) != '\0') && (*(short *)((int)unaff_A6 + 0x7e) != 0)) &&
            (*(char *)(unaff_A5 + 0x7db6) != '\x01')) &&
           ((*(char *)(unaff_A5 + 0x7db7) != '\x01' &&
            (bVar13 = false, *(char *)(unaff_A5 + 0x7003) != '\0')))) {
          uStack_4 = 0x18822;
          FUN_000c8cfc();
          if (!bVar13) {
            if (*(char *)(unaff_A6 + 0x1f) != *(char *)((int)unaff_A6 + 0x7e)) {
              uStack_4 = 0x1883c;
              candidate_120f8();
            }
            uStack_4 = 0x18842;
            candidate_427a();
          }
        }
      }
    }
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
      *(undefined2 *)(unaff_A6 + 0x20) = 0xffff;
      goto LAB_000182a4;
    }
    if (*(char *)(unaff_A5 + 0x4309) == '\0') {
      uVar12 = false;
      if (*(char *)(unaff_A5 + 0x4300) == '\0') {
        uStack_4 = 0x18d3e;
        func_0x000040ac();
        if ((bool)uVar12) {
          if (*(char *)(extraout_A0 + 0x7b) != '\0') {
            uVar12 = false;
            uStack_4 = 0x18d52;
            candidate_ca2f8();
            if (!(bool)uVar12) goto LAB_00018d56;
          }
          goto LAB_00018d8e;
        }
LAB_00018d56:
        uStack_4 = 0x18d60;
        func_0x000040ac();
        if ((bool)uVar12) {
          if (*(char *)(extraout_A0_00 + 0x7b) != '\0') {
            bVar13 = false;
            uStack_4 = 0x18d74;
            candidate_ca2f8();
            if (!bVar13) goto LAB_00018d78;
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
        uVar12 = false;
      }
      else {
        uVar5 = extraout_D1w;
        if ((short)unaff_D3w < (short)extraout_D1w) {
          uVar5 = unaff_D3w;
          unaff_D3w = extraout_D1w;
        }
        uVar2 = ((short)(unaff_D3w + uVar5) >> 1) - *(short *)(unaff_A5 + 0x69ba);
        if ((int)((uint)uVar2 << 0x10) < 0) {
          uVar5 = unaff_D3w - 0x1df;
          if ((int)((uint)uVar5 << 0x10) < 0) {
            if ((short)uVar5 < (short)uVar2) {
              uVar5 = uVar2;
            }
          }
          else {
            uVar5 = 0;
          }
        }
        else if ((int)((uint)uVar5 << 0x10) < 0) {
          uVar5 = 0;
        }
        else if ((short)uVar2 < (short)uVar5) {
          uVar5 = uVar2;
        }
        if (((DAT_0010f000 & 8) != 0) || ((DAT_0010f000 & 0x10) != 0)) {
          uVar5 = uVar5 & 0xfff8;
        }
        iVar7 = (short)uVar5 * 0x3000;
        iVar8 = *(int *)((int)unaff_A6 + 0x76);
        if ((iVar7 - iVar8 == 0 || iVar7 < iVar8) && (iVar3 = -iVar8, iVar8 = iVar7, iVar7 < iVar3))
        {
          iVar8 = iVar3;
        }
        uVar12 = false;
        if (iVar8 != 0) {
          if (iVar8 < 0) {
            uVar9 = -*(int *)(unaff_A5 + 0x6974);
            if (*(int *)(unaff_A5 + 0x6974) < 1) {
              uVar9 = 0;
            }
            uVar12 = CARRY4((int)(iVar8 - uVar9) >> 2,uVar9);
          }
          else {
            uVar9 = -*(int *)(unaff_A5 + 0x6974);
            if (0 < *(int *)(unaff_A5 + 0x6974)) {
              uVar9 = 0;
            }
            uVar12 = CARRY4((int)(iVar8 - uVar9) >> 2,uVar9);
          }
        }
        register0x0000003c = (BADSPACEBASE *)&uStack_4;
        uStack_4 = 0x18f10;
      }
      if ((bRam0010e95a & 1) != 0) {
        uVar12 = false;
      }
      if ((bRam0010e95a & 2) != 0) {
        uVar12 = false;
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc81e6;
      candidate_c7ca6();
      if ((bRam0010e956 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8238;
        candidate_c6f9e();
        uVar9 = *(uint *)(extraout_A0_01 + 0x18);
        uVar12 = CARRY4(uRam0010e9cc,uVar9);
        *(uint *)(extraout_A0_01 + 0x18) = uRam0010e9cc + uVar9;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc81fc;
        candidate_1ac38();
      }
      if ((bRam0010e957 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8292;
        candidate_c6f9e();
        uVar9 = *(uint *)(extraout_A0_02 + 0x18);
        uVar12 = CARRY4(uRam0010e9cc,uVar9);
        *(uint *)(extraout_A0_02 + 0x18) = uRam0010e9cc + uVar9;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8256;
        func_0x0001ac3c();
      }
      if ((bRam0010e958 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc82ec;
        candidate_c6f9e();
        uVar9 = *(uint *)(extraout_A0_03 + 0x18);
        uVar12 = CARRY4(uRam0010e9cc,uVar9);
        *(uint *)(extraout_A0_03 + 0x18) = uRam0010e9cc + uVar9;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc82b0;
        func_0x0001ac3c();
      }
      if ((bRam0010e959 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8346;
        candidate_c6f9e();
        uVar9 = *(uint *)(extraout_A0_04 + 0x18);
        uVar12 = CARRY4(uRam0010e9cc,uVar9);
        *(uint *)(extraout_A0_04 + 0x18) = uRam0010e9cc + uVar9;
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
          if ((bool)uVar12) goto LAB_000c8386;
        }
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8368;
        candidate_c8abc();
        if (!(bool)uVar12) goto LAB_000c836c;
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
      sVar6 = *psRam0010e964;
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
    sVar6 = *(short *)(unaff_A6 + 0x1f) + -1;
    *(short *)(unaff_A6 + 0x1f) = sVar6;
  } while (sVar6 != 0);
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
code_r0x00004188:
  uStack_4 = 0x418a;
  candidate_4126();
  iVar8 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar8 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
LAB_000c83ec:
  switch(sVar6) {
  case 0:
    uVar5 = psRam0010e964[2];
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84b2;
    candidate_c7ebc();
    if (_DAT_0010e99e < (uint)uVar5 * 0x10000) goto LAB_000c840c;
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
    uVar12 = false;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84e4;
    func_0x000040ac();
    if ((bool)uVar12) {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84ec;
      candidate_c8524();
      if (!(bool)uVar12) goto LAB_000c840c;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84fa;
      func_0x000040ac();
      if (!(bool)uVar12) break;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8502;
      candidate_c8524();
    }
    else {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8514;
      func_0x000040ac();
      if (!(bool)uVar12) goto LAB_000c840c;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc851c;
      candidate_c8524();
    }
    if (!(bool)uVar12) {
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
    (*(code *)(sVar6 + 0xc844a))();
  }
  psRam0010e964 = (short *)((int)psRam0010e964[1] + (int)psRam0010e964);
  sVar6 = *psRam0010e964;
  goto LAB_000c83ec;
}
