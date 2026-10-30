/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x018b10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

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

uint candidate_18b10(void)

{
  code *pcVar1;
  short sVar2;
  ushort uVar3;
  int iVar4;
  ushort *puVar5;
  ushort uVar6;
  ushort extraout_D1w;
  ushort uVar10;
  int iVar7;
  int iVar8;
  uint uVar9;
  byte bVar11;
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
  undefined1 uVar12;
  bool bVar13;
  undefined4 uStack_4;

  sVar2 = *(short *)(unaff_A6 + 0x1f);
  *(short *)(unaff_A6 + 0x1f) = sVar2 + -1;
  if (sVar2 == 0) {
    uStack_4 = 0x18b24;
    candidate_c8eaa();
    unaff_D3w = 0x1e;
    uStack_4 = 0x18b2c;
    candidate_9c0a();
    *(undefined2 *)(unaff_A6 + 0x1f) = 0x1e;
    *unaff_A6 = 0x18b38;
    sVar2 = *(short *)(unaff_A6 + 0x1f);
    *(short *)(unaff_A6 + 0x1f) = sVar2 + -1;
    if (sVar2 == 0) {
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
      sVar2 = *(short *)(unaff_A6 + 0x1f);
      *(short *)(unaff_A6 + 0x1f) = sVar2 + -1;
      if (sVar2 == 0) {
        uStack_4 = 0x18bc8;
        candidate_c8eaa();
        DAT_0010109f = 0;
        uVar10 = *(ushort *)(unaff_A5 + 0x700c);
        *(ushort *)(unaff_A5 + 0x700c) = uVar10 + 0x3ef;
        if (0xfc10 < uVar10) {
          uVar10 = *(ushort *)(unaff_A5 + 0x700a);
          *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
          uVar6 = uRam002fffe8;
          uVar3 = ~uVar10 & 0xfff;
          bVar11 = -((*(byte *)((short)(uVar10 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                    0);
          uRam002fffe0 = (ushort)(bVar11 & -(0x7ff < uVar3));
          uRam002fffe2 = 0;
          uRam002fffe4 = 0;
          uRam002fffe6 = 0;
          *(short *)(unaff_A5 + 0x700a) =
               ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU)
               + *(short *)(unaff_A5 + 0x700a);
          if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (bVar11 ^ uVar6) & 1) != 0) {
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
      uStack_4 = 0x182ae;
      candidate_1824a();
      *(undefined4 *)((int)unaff_A6 + 0x6e) = unaff_A4;
                    /* WARNING: Could not recover jumptable at 0x000182b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar9 = (*(code *)(**(short **)((int)unaff_A6 + 0x6e) + 0x182bc))();
      return uVar9;
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
        uVar10 = extraout_D1w;
        if ((short)unaff_D3w < (short)extraout_D1w) {
          uVar10 = unaff_D3w;
          unaff_D3w = extraout_D1w;
        }
        uVar3 = ((short)(unaff_D3w + uVar10) >> 1) - *(short *)(unaff_A5 + 0x69ba);
        if ((int)((uint)uVar3 << 0x10) < 0) {
          uVar10 = unaff_D3w - 0x1df;
          if ((int)((uint)uVar10 << 0x10) < 0) {
            if ((short)uVar10 < (short)uVar3) {
              uVar10 = uVar3;
            }
          }
          else {
            uVar10 = 0;
          }
        }
        else if ((int)((uint)uVar10 << 0x10) < 0) {
          uVar10 = 0;
        }
        else if ((short)uVar3 < (short)uVar10) {
          uVar10 = uVar3;
        }
        if (((DAT_0010f000 & 8) != 0) || ((DAT_0010f000 & 0x10) != 0)) {
          uVar10 = uVar10 & 0xfff8;
        }
        iVar7 = (short)uVar10 * 0x3000;
        iVar8 = *(int *)((int)unaff_A6 + 0x76);
        if ((iVar7 - iVar8 == 0 || iVar7 < iVar8) && (iVar4 = -iVar8, iVar8 = iVar7, iVar7 < iVar4))
        {
          iVar8 = iVar4;
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
      uVar10 = *puRam0010e964;
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
    sVar2 = *(short *)(unaff_A6 + 0x1f) + -1;
    *(short *)(unaff_A6 + 0x1f) = sVar2;
  } while (sVar2 != 0);
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
LAB_000c83f8:
  puRam0010e964 = (ushort *)((int)(short)puRam0010e964[1] + (int)puRam0010e964);
  uVar10 = *puRam0010e964;
LAB_000c83ec:
  puVar5 = puRam0010e964;
  uVar9 = (uint)uVar10;
  if (0xb < uVar10) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc83f8;
    (*(code *)((short)uVar10 + 0xc844a))();
    goto LAB_000c83f8;
  }
                    /* WARNING: Switch is manually overridden */
  switch(uVar10) {
  case 0:
    uVar10 = puRam0010e964[2];
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84b2;
    candidate_c7ebc();
    if ((uint)uVar10 * 0x10000 <= _DAT_0010e99e) goto LAB_000c83f8;
    goto LAB_000c840c;
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
      if (!(bool)uVar12) goto LAB_000c83f8;
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
    if ((bool)uVar12) goto LAB_000c83f8;
    goto LAB_000c840c;
  case 8:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8568;
    candidate_c7ebc();
LAB_000c840c:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8418;
    func_0x000c748e();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8424;
    func_0x000c748e();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8430;
    func_0x000c748e();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc843c;
    uVar9 = func_0x000c748e();
    return uVar9;
  case 0xc:
    break;
  case 0x10:
    break;
  case 0x14:
    break;
  case 0x18:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc85e6;
    uVar9 = candidate_9d8c();
    return uVar9;
  case 0x1c:
    uRam0010e9b6 = puRam0010e964[2];
    return uVar9;
  case 0x20:
    iRam0010e9a6 = (uint)puRam0010e964[2] << 0x10;
    iRam0010e9aa = (uint)puRam0010e964[3] << 0x10;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc860c;
    candidate_c7c70();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8620;
    candidate_c6ff2();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc862c;
    candidate_c6ff2();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8638;
    candidate_c6ff2();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8644;
    uVar9 = candidate_c6ff2();
    return uVar9;
  case 0x24:
    if (uRam0010e9d0 == 0) {
      *(ushort **)((int)register0x0000003c + -4) = puRam0010e964;
      *(undefined4 *)((int)register0x0000003c + -8) = 0xc866e;
      func_0x000c748e();
      iVar8 = *(int *)((int)register0x0000003c + -4);
    }
    else {
      *(ushort **)((int)register0x0000003c + -4) = puRam0010e964;
      *(undefined4 *)((int)register0x0000003c + -8) = 0xc8680;
      candidate_c741a();
      iVar8 = *(int *)((int)register0x0000003c + -4);
    }
    if ((((*(int *)(iVar8 + 0xc) != 0) && (*(int *)(*(int *)(iVar8 + 0xc) + 4) != 0)) &&
        (*(int *)(iVar8 + 4) != 0x10c72e)) && (*(int *)(iVar8 + 4) != 0x10d7b6)) {
      __m68k_trap(0xf);
    }
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc86c6;
    uVar9 = candidate_c6f34();
    return uVar9;
  case 0x28:
    iVar8 = *(int *)(puRam0010e964 + 2);
    *(ushort *)(iVar8 + 0x26) = puRam0010e964[4];
    *(ushort *)(iVar8 + 0x28) = puVar5[5];
    bVar11 = (*(byte *)(puVar5 + 6) & 3) << 1;
    *(byte *)(iVar8 + 0x82) = bVar11 | *(byte *)(iVar8 + 0x82) & 0xf9;
    if (*(char *)((int)puVar5 + 0xd) == '\0') {
      *(byte *)(iVar8 + 0x82) = *(byte *)(iVar8 + 0x82) & 0xf7;
    }
    else {
      *(byte *)(iVar8 + 0x82) = *(byte *)(iVar8 + 0x82) | 8;
    }
    return (uint)CONCAT11((char)(uVar10 >> 8),bVar11);
  case 0x2c:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8714;
    uVar9 = candidate_c741a();
    return uVar9;
  case 0x30:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8732;
    uVar9 = candidate_c6cfc();
    return uVar9;
  case 0x34:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc873e;
    uVar9 = candidate_c6d04();
    return uVar9;
  case 0x38:
    goto LAB_000c8764;
  case 0x3c:
    uRam0010e9ae = puRam0010e964[2];
    uRam0010e9b0 = puRam0010e964[3];
    uRam0010e9b2 = puRam0010e964[4];
    uRam0010e9b4 = puRam0010e964[5];
    return 0;
  case 0x40:
    uRam0010e9b8 = *(undefined4 *)(puRam0010e964 + 2);
    return uVar9;
  case 0x44:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc85b6;
    uVar9 = candidate_2802();
    return uVar9;
  case 0x48:
    bRam0010e956 = *(undefined1 *)(puRam0010e964 + 2);
    bRam0010e957 = *(undefined1 *)((int)puRam0010e964 + 5);
    bRam0010e958 = *(undefined1 *)(puRam0010e964 + 3);
    bRam0010e959 = *(undefined1 *)((int)puRam0010e964 + 7);
    return uVar9;
  case 0x4c:
    bRam0010e95a = *(undefined1 *)(puRam0010e964 + 2);
    return uVar9;
  case 0x50:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8658;
    uVar9 = candidate_c6ff2();
    return uVar9;
  case 0x54:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc8726;
    uVar9 = candidate_c73ce();
    return uVar9;
  case 0x58:
    *(ushort **)((int)register0x0000003c + -4) = puRam0010e964;
    *(undefined4 *)((int)register0x0000003c + -8) = 0xc8586;
    uVar9 = candidate_1aa28();
    return uVar9;
  case 0x5c:
    uRam0010c00e = *(undefined4 *)(puRam0010e964 + 2);
    return uVar9;
  case 0x60:
    if ((uint)uRam0010e9d0 << 0x10 < uRam0010e9a2) {
      return (uint)uRam0010e9d0 << 0x10;
    }
    goto LAB_000c8764;
  case 100:
    if (uRam0010e9a2 <= (uint)uRam0010e9d0 << 0x10) {
      return (uint)uRam0010e9d0 << 0x10;
    }
LAB_000c8764:
    *(ushort **)((int)register0x0000003c + -4) = puRam0010e964;
    pcVar1 = *(code **)(puRam0010e964 + 2);
    *(undefined4 *)((int)register0x0000003c + -8) = 0xc8770;
    uVar9 = (*pcVar1)();
    return uVar9;
  }
  return uVar9;
}
