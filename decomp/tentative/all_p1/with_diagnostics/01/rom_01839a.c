/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01839a.
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

void candidate_1839a(void)

{
  code *pcVar1;
  ushort uVar2;
  int iVar3;
  ushort uVar4;
  code **ppcVar5;
  undefined1 uVar8;
  ushort uVar6;
  short sVar7;
  uint extraout_D1;
  ushort extraout_D1w;
  int iVar9;
  int iVar10;
  uint uVar11;
  byte bVar12;
  ushort unaff_D3w;
  short unaff_D7w;
  short *psVar13;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  undefined4 unaff_A4;
  int unaff_A5;
  undefined4 *unaff_A6;
  code **ppcVar14;
  undefined4 *puVar15;
  bool bVar16;
  code *pcStack_4;

  sVar7 = *(short *)(unaff_A6 + 0x1f);
  *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
  if (sVar7 == 0) {
    pcStack_4 = (code *)0x183a8;
    candidate_164f0();
    pcStack_4 = (code *)0x183d6;
    candidate_3d4ee();
    pcStack_4 = (code *)0x183dc;
    candidate_4192();
    pcStack_4 = (code *)0x183e8;
    candidate_420c();
    pcStack_4 = (code *)0x183ee;
    candidate_c0cd6();
    if (*(short *)(unaff_A5 + 0x42fa) == 0) {
      pcStack_4 = (code *)0x183fa;
      func_0x0001805c();
    }
    else {
      pcStack_4 = (code *)0x18402;
      func_0x00018080();
    }
    pcStack_4 = (code *)0x1840a;
    candidate_ca2f2();
    pcStack_4 = (code *)0x18412;
    candidate_ca2f2();
    sVar7 = *(short *)(*(int *)(*(int *)((int)unaff_A6 + 0x6e) + 10) + 0xc);
    if ((sVar7 != 0) && (sVar7 != *(short *)(unaff_A6 + 0x20))) {
      *(short *)(unaff_A6 + 0x20) = sVar7;
      pcStack_4 = (code *)0x18434;
      candidate_120f8();
    }
    *(undefined2 *)(FUN_000042f4 + unaff_A5 + 2) =
         *(undefined2 *)(*(int *)(*(int *)((int)unaff_A6 + 0x6e) + 10) + 0xe);
    pcStack_4 = (code *)0x18458;
    candidate_19e04();
    pcStack_4 = (code *)0x1846c;
    candidate_c8774();
    unaff_A4 = *(undefined4 *)(*(int *)((int)unaff_A6 + 0x6e) + 10);
    pcStack_4 = (code *)0x18482;
    candidate_19604();
    pcStack_4 = (code *)0x18488;
    uVar8 = candidate_3586();
    *(undefined1 *)((int)unaff_A6 + 0x7a) = uVar8;
    *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xa0000;
    *(undefined4 *)((int)unaff_A6 + 0x76) = 0xa0000;
    pcStack_4 = (code *)0x184a8;
    candidate_1963a();
    *(undefined2 *)(unaff_A6 + 0x1f) = 1;
    *unaff_A6 = 0x184b4;
    sVar7 = *(short *)(unaff_A6 + 0x1f);
    *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
    if (sVar7 == 0) {
                    /* WARNING: Switch is manually overridden */
      switch(*(undefined2 *)(*(int *)((int)unaff_A6 + 0x6e) + 6)) {
      case 0:
        pcStack_4 = candidate_16502;
        candidate_36a4();
        unaff_D3w = 0;
        candidate_9c0a();
        ppcVar5 = &pcStack_4;
        break;
      case 4:
        unaff_D3w = 0;
        pcStack_4 = (code *)0x18534;
        candidate_9c0a();
        *(undefined1 *)(unaff_A6 + 0x18) = 0;
        bVar16 = false;
        pcStack_4 = (code *)0x1854a;
        candidate_40bc();
        if ((bVar16) && (*unaff_A6 = candidate_18554, *(char *)(unaff_A6 + 0x18) == '\0'))
        goto LAB_00018cf8;
        pcStack_4 = (code *)0x18562;
        candidate_16502();
        ppcVar5 = (code **)register0x0000003c;
        break;
      case 8:
        ppcVar14 = &pcStack_4;
        register0x0000003c = (BADSPACEBASE *)&pcStack_4;
        pcStack_4 = candidate_16502;
        candidate_36a4();
        unaff_D3w = 0x10;
        *(undefined2 *)(unaff_A6 + 0x1f) = 0x10;
        candidate_9c0a();
        *unaff_A6 = candidate_1851a;
        sVar7 = *(short *)(unaff_A6 + 0x1f);
        *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
        ppcVar5 = ppcVar14;
        if (sVar7 != 0) goto LAB_00018cf8;
        break;
      case 0xc:
        pcStack_4 = (code *)0x18574;
        candidate_9c0a();
        *unaff_A6 = candidate_1857c;
        return;
      }
      register0x0000003c = (BADSPACEBASE *)ppcVar5;
      *(undefined1 *)(unaff_A5 + 0x4302) = 0xff;
      if (DAT_00100088 != '\x02') {
        *(undefined4 *)((int)register0x0000003c + -4) = 0x18626;
        candidate_3b028();
      }
      if (-1 < *(short *)(*(int *)((int)unaff_A6 + 0x6e) + 0xe)) {
        if ((*(short *)(unaff_A5 + 0x42fa) == 0) && (DAT_00100088 == '\x02')) {
          *(undefined4 *)((int)register0x0000003c + -4) = 0x18656;
          candidate_40bc();
        }
        *(undefined4 *)((int)register0x0000003c + -4) = 0x18666;
        candidate_40bc();
        DAT_0010c2e6 = 0xff;
        *(undefined4 *)((int)register0x0000003c + -4) = 0x18674;
        candidate_c8e50();
        *(undefined4 *)((int)register0x0000003c + -4) = 0x1867a;
        candidate_c8ce6();
        *(undefined4 *)((int)register0x0000003c + -4) = 0x18680;
        candidate_15422();
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0x18690;
      candidate_4256();
      if (*(short *)(FUN_000042f4 + unaff_A5 + 2) == 0x2c) {
        *unaff_A6 = candidate_18ac6;
        if ((bRam00101473 & 0x80) != 0) {
LAB_00018b18:
          *(undefined4 *)((int)register0x0000003c + -4) = 0x18b24;
          candidate_c8eaa();
          unaff_D3w = 0x1e;
          *(undefined4 *)((int)register0x0000003c + -4) = 0x18b2c;
          candidate_9c0a();
          *(undefined2 *)(unaff_A6 + 0x1f) = 0x1e;
          *unaff_A6 = 0x18b38;
          sVar7 = *(short *)(unaff_A6 + 0x1f);
          *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
          if (sVar7 == 0) {
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b46;
            candidate_4192();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b52;
            candidate_420c();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b58;
            candidate_c0cd6();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b5e;
            candidate_2852();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b64;
            candidate_16502();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b6a;
            candidate_3d4ee();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b74;
            candidate_1aa28();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b7e;
            candidate_c81a6();
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b84;
            candidate_16502();
            unaff_D3w = 0x1e;
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18b94;
            candidate_9c0a();
            if (*(short *)(FUN_000042f4 + unaff_A5 + 2) == 0x2c) {
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18bae;
              candidate_40bc();
            }
            *(undefined2 *)(unaff_A6 + 0x1f) = 100;
            *unaff_A6 = 0x18bba;
            sVar7 = *(short *)(unaff_A6 + 0x1f);
            *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
            if (sVar7 == 0) {
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18bc8;
              candidate_c8eaa();
              DAT_0010109f = 0;
              uVar6 = *(ushort *)(unaff_A5 + 0x700c);
              *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
              if (0xfc10 < uVar6) {
                uVar6 = *(ushort *)(unaff_A5 + 0x700a);
                *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
                uVar4 = uRam002fffe8;
                uVar2 = ~uVar6 & 0xfff;
                bVar12 = -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)
                           ) != 0);
                uRam002fffe0 = (ushort)(bVar12 & -(0x7ff < uVar2));
                uRam002fffe2 = 0;
                uRam002fffe4 = 0;
                uRam002fffe6 = 0;
                *(short *)(unaff_A5 + 0x700a) =
                     ((short)-(uVar2 == 0x400) &
                     _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
                     *(short *)(unaff_A5 + 0x700a);
                if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (bVar12 ^ uVar4) & 1) != 0)
                {
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
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x3d2c;
                  func_0x0003cf12();
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x3d36;
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
              bVar16 = false;
              *(undefined4 *)((int)register0x0000003c + -4) = 0x186ec;
              candidate_c8e02();
              if (bVar16) {
                *(undefined4 *)((int)register0x0000003c + -4) = 0x186f6;
                candidate_275a();
                if (!bVar16) goto LAB_00018a7c;
              }
            }
          }
          else if (*(short *)(unaff_A5 + 0x430a) != 0) {
LAB_00018a7c:
            if (DAT_0010c2e7 == '\0') {
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18ae0;
              candidate_12244();
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18ae6;
              candidate_12190();
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18af0;
              candidate_120f8();
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18af6;
              candidate_15418();
              unaff_D3w = 0x30;
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18b04;
              candidate_9c0a();
              *(undefined2 *)(unaff_A6 + 0x1f) = 0x20;
              *unaff_A6 = candidate_18b10;
              sVar7 = *(short *)(unaff_A6 + 0x1f);
              *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
              if (sVar7 == 0) goto LAB_00018b18;
            }
            else {
              DAT_0010109f = 0;
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18a96;
              candidate_15418();
              *(undefined2 *)(unaff_A6 + 0x1f) = 0x10;
              *unaff_A6 = candidate_18aa2;
              sVar7 = *(short *)(unaff_A6 + 0x1f) + -1;
              *(short *)(unaff_A6 + 0x1f) = sVar7;
              if (sVar7 == 0) {
                *(undefined4 *)((int)register0x0000003c + -4) = 0x18ab0;
                candidate_15418();
                *(undefined4 *)((int)register0x0000003c + -4) = 0x18ab6;
                candidate_154f8();
                *unaff_A6 = 0x18cf8;
              }
            }
            goto LAB_00018cf8;
          }
          uVar8 = 0;
          *(undefined4 *)((int)register0x0000003c + -4) = 0x18704;
          candidate_c8fdc();
          *(undefined4 *)((int)register0x0000003c + -4) = 0x18708;
          candidate_193fe();
          if ((bool)uVar8) {
            if (-1 < *(char *)(unaff_A5 + 0x4303)) {
              if (*(char *)(unaff_A5 + 0x7d8a) != '\0') {
                *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x9ff;
              }
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18726;
              candidate_18234();
              if (*(byte *)(unaff_A5 + 0x4303) == 0) {
                *(short *)(unaff_A5 + 0x42fa) = *(short *)(unaff_A5 + 0x42fa) + 1;
                psVar13 = (short *)((int)*(short *)(*(int *)((int)unaff_A6 + 0x6e) + 2) +
                                   *(int *)((int)unaff_A6 + 0x6e));
                *(short **)((int)unaff_A6 + 0x6e) = psVar13;
              }
              else {
                *(ushort *)(unaff_A5 + 0x42fa) = (ushort)*(byte *)(unaff_A5 + 0x4303);
LAB_000182a4:
                *(undefined4 *)((int)register0x0000003c + -4) = 0x182ae;
                candidate_1824a();
                *(undefined4 *)((int)unaff_A6 + 0x6e) = unaff_A4;
                psVar13 = *(short **)((int)unaff_A6 + 0x6e);
              }
                    /* WARNING: Could not recover jumptable at 0x000182b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              (*(code *)(*psVar13 + 0x182bc))();
              return;
            }
            if ((DAT_0010c2e7 == '\0') && (bVar16 = false, *(char *)(unaff_A5 + 0x4300) == '\0')) {
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18752;
              FUN_000c8cfc();
              if ((!bVar16) &&
                 ((*(char *)(unaff_A5 + 0x7003) != '\0' &&
                  (uVar6 = *(ushort *)((int)unaff_A6 + 0x7a), uVar6 != 0xffff)))) {
                uVar2 = uVar6 - 2;
                if (uVar6 < 2 || uVar2 == 0) {
                  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x1877c;
                  candidate_c8fdc();
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x18782;
                  candidate_c0a2c();
                  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x18a20;
                  candidate_4256();
                  *(undefined2 *)(unaff_A6 + 0xf) = 0xa0;
                  *(undefined2 *)(unaff_A6 + 0x10) = 0x180;
                  *(undefined2 *)(unaff_A6 + 0x1f) = 0x5a;
                  *unaff_A6 = candidate_18a38;
                  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x18a44;
                  candidate_12022();
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x18a4a;
                  candidate_427a();
                  sVar7 = *(short *)(unaff_A6 + 0x1f);
                  *(short *)(unaff_A6 + 0x1f) = sVar7 + -1;
                  if (sVar7 == 0) {
                    *(undefined4 *)((int)register0x0000003c + -4) = 0x18a58;
                    uVar8 = candidate_3586();
                    *(undefined1 *)((int)unaff_A6 + 0x7a) = uVar8;
                    *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
                    *(undefined4 *)((int)register0x0000003c + -4) = 0x18a72;
                    candidate_4256();
                    *unaff_A6 = 100000;
                  }
                  goto LAB_00018cf8;
                }
                *(ushort *)((int)unaff_A6 + 0x7a) = uVar2;
                if ((uVar2 < 0xa00) && (0xfb < (byte)((char)uVar2 * '\x02'))) {
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x187a4;
                  candidate_120f8();
                }
              }
            }
          }
          if (*(short *)((int)unaff_A6 + 0x7e) != 0) {
            if (*(ushort *)((int)unaff_A6 + 0x7e) < 0x780) {
              *(undefined4 *)((int)register0x0000003c + -4) = 0x187bc;
              candidate_190f0();
              if ((extraout_D1 & 0x20) == 0) goto LAB_000187da;
              uVar6 = (ushort)CONCAT31((uint3)(byte)((ushort)*(undefined2 *)((int)unaff_A6 + 0x7e)
                                                    >> 8),0x80);
              sVar7 = uVar6 - 0x100;
              if (uVar6 < 0x100) {
                sVar7 = 0;
              }
            }
            else {
LAB_000187da:
              sVar7 = *(ushort *)((int)unaff_A6 + 0x7e) - 5;
              if (*(ushort *)((int)unaff_A6 + 0x7e) < 5) {
                sVar7 = 0;
              }
            }
            *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)((int)unaff_A6 + 0x7e);
            *(short *)((int)unaff_A6 + 0x7e) = sVar7;
            if ((((*(char *)(unaff_A5 + 0x7d8a) != '\0') && (*(short *)((int)unaff_A6 + 0x7e) != 0))
                && (*(char *)(unaff_A5 + 0x7db6) != '\x01')) &&
               ((*(char *)(unaff_A5 + 0x7db7) != '\x01' &&
                (bVar16 = false, *(char *)(unaff_A5 + 0x7003) != '\0')))) {
              *(undefined4 *)((int)register0x0000003c + -4) = 0x18822;
              FUN_000c8cfc();
              if (!bVar16) {
                if (*(char *)(unaff_A6 + 0x1f) != *(char *)((int)unaff_A6 + 0x7e)) {
                  *(undefined4 *)((int)register0x0000003c + -4) = 0x1883c;
                  candidate_120f8();
                }
                *(undefined4 *)((int)register0x0000003c + -4) = 0x18842;
                candidate_427a();
              }
            }
          }
        }
      }
    }
  }
LAB_00018cf8:
  do {
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18cfe;
    candidate_ca21e();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18d04;
    candidate_469c();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18d0a;
    candidate_8bfd6();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18d10;
    candidate_19658();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18d16;
    candidate_19e40();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18d1c;
    candidate_401e8();
    if (*(char *)(unaff_A5 + 0x4308) != '\0') {
      *(undefined4 *)((int)register0x0000003c + -4) = 0x1828e;
      candidate_18234();
      *(undefined1 *)(unaff_A5 + 0x4308) = 0;
      *(undefined2 *)(unaff_A5 + 0x42f8) = *(undefined2 *)(unaff_A5 + 0x42fc);
      *(undefined2 *)(unaff_A5 + 0x42fa) = *(undefined2 *)(unaff_A5 + 0x42fe);
      *(undefined2 *)(unaff_A6 + 0x20) = 0xffff;
      goto LAB_000182a4;
    }
    if (*(char *)(unaff_A5 + 0x4309) == '\0') {
      uVar8 = false;
      if (*(char *)(unaff_A5 + 0x4300) == '\0') {
        *(undefined4 *)((int)register0x0000003c + -4) = 0x18d3e;
        func_0x000040ac();
        if ((bool)uVar8) {
          if (*(char *)(extraout_A0 + 0x7b) != '\0') {
            uVar8 = false;
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18d52;
            candidate_ca2f8();
            if (!(bool)uVar8) goto LAB_00018d56;
          }
          goto LAB_00018d8e;
        }
LAB_00018d56:
        *(undefined4 *)((int)register0x0000003c + -4) = 0x18d60;
        func_0x000040ac();
        if ((bool)uVar8) {
          if (*(char *)(extraout_A0_00 + 0x7b) != '\0') {
            bVar16 = false;
            *(undefined4 *)((int)register0x0000003c + -4) = 0x18d74;
            candidate_ca2f8();
            if (!bVar16) goto LAB_00018d78;
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
      *(undefined4 *)((int)register0x0000003c + -4) = 0x18d9a;
      candidate_1903a();
      if (unaff_D7w == 0) {
        uVar8 = false;
      }
      else {
        uVar6 = extraout_D1w;
        if ((short)unaff_D3w < (short)extraout_D1w) {
          uVar6 = unaff_D3w;
          unaff_D3w = extraout_D1w;
        }
        uVar2 = ((short)(unaff_D3w + uVar6) >> 1) - *(short *)(unaff_A5 + 0x69ba);
        if ((int)((uint)uVar2 << 0x10) < 0) {
          uVar6 = unaff_D3w - 0x1df;
          if ((int)((uint)uVar6 << 0x10) < 0) {
            if ((short)uVar6 < (short)uVar2) {
              uVar6 = uVar2;
            }
          }
          else {
            uVar6 = 0;
          }
        }
        else if ((int)((uint)uVar6 << 0x10) < 0) {
          uVar6 = 0;
        }
        else if ((short)uVar2 < (short)uVar6) {
          uVar6 = uVar2;
        }
        if (((DAT_0010f000 & 8) != 0) || ((DAT_0010f000 & 0x10) != 0)) {
          uVar6 = uVar6 & 0xfff8;
        }
        iVar9 = (short)uVar6 * 0x3000;
        iVar10 = *(int *)((int)unaff_A6 + 0x76);
        if ((iVar9 - iVar10 == 0 || iVar9 < iVar10) &&
           (iVar3 = -iVar10, iVar10 = iVar9, iVar9 < iVar3)) {
          iVar10 = iVar3;
        }
        uVar8 = false;
        if (iVar10 != 0) {
          if (iVar10 < 0) {
            uVar11 = -*(int *)(unaff_A5 + 0x6974);
            if (*(int *)(unaff_A5 + 0x6974) < 1) {
              uVar11 = 0;
            }
            uVar8 = CARRY4((int)(iVar10 - uVar11) >> 2,uVar11);
          }
          else {
            uVar11 = -*(int *)(unaff_A5 + 0x6974);
            if (0 < *(int *)(unaff_A5 + 0x6974)) {
              uVar11 = 0;
            }
            uVar8 = CARRY4((int)(iVar10 - uVar11) >> 2,uVar11);
          }
        }
        puVar15 = (undefined4 *)((int)register0x0000003c + -4);
        register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
        *puVar15 = 0x18f10;
      }
      if ((bRam0010e95a & 1) != 0) {
        uVar8 = false;
      }
      if ((bRam0010e95a & 2) != 0) {
        uVar8 = false;
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc81e6;
      candidate_c7ca6();
      if ((bRam0010e956 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8238;
        candidate_c6f9e();
        uVar11 = *(uint *)(extraout_A0_01 + 0x18);
        uVar8 = CARRY4(uRam0010e9cc,uVar11);
        *(uint *)(extraout_A0_01 + 0x18) = uRam0010e9cc + uVar11;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc81fc;
        candidate_1ac38();
      }
      if ((bRam0010e957 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8292;
        candidate_c6f9e();
        uVar11 = *(uint *)(extraout_A0_02 + 0x18);
        uVar8 = CARRY4(uRam0010e9cc,uVar11);
        *(uint *)(extraout_A0_02 + 0x18) = uRam0010e9cc + uVar11;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8256;
        func_0x0001ac3c();
      }
      if ((bRam0010e958 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc82ec;
        candidate_c6f9e();
        uVar11 = *(uint *)(extraout_A0_03 + 0x18);
        uVar8 = CARRY4(uRam0010e9cc,uVar11);
        *(uint *)(extraout_A0_03 + 0x18) = uRam0010e9cc + uVar11;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc82b0;
        func_0x0001ac3c();
      }
      if ((bRam0010e959 & 0x80) == 0) {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8346;
        candidate_c6f9e();
        uVar11 = *(uint *)(extraout_A0_04 + 0x18);
        uVar8 = CARRY4(uRam0010e9cc,uVar11);
        *(uint *)(extraout_A0_04 + 0x18) = uRam0010e9cc + uVar11;
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
          if ((bool)uVar8) goto LAB_000c8386;
        }
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xc8368;
        candidate_c8abc();
        if (!(bool)uVar8) goto LAB_000c836c;
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
      sVar7 = *psRam0010e964;
      goto LAB_000c83ec;
    }
    DAT_0010c309 = 0;
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18c6c;
    candidate_c09d8();
    unaff_D3w = 0xc;
    *(undefined4 *)((int)register0x0000003c + -4) = 0x18c7a;
    candidate_9c0a();
    *(undefined2 *)(unaff_A6 + 0x1f) = 0x14;
    *unaff_A6 = 0x18c86;
    sVar7 = *(short *)(unaff_A6 + 0x1f) + -1;
    *(short *)(unaff_A6 + 0x1f) = sVar7;
  } while (sVar7 != 0);
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18c94;
  candidate_4192();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18ca0;
  candidate_420c();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cac;
  candidate_41e8();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cb8;
  candidate_41e8();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cc6;
  candidate_27c4();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cd4;
  candidate_27c4();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cda;
  candidate_c0cd6();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18ce0;
  candidate_16502();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cec;
  candidate_1aa28();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x18cf2;
  candidate_c8eaa();
code_r0x00004188:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x418a;
  candidate_4126();
  iVar10 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar10 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
LAB_000c83ec:
  switch(sVar7) {
  case 0:
    uVar6 = psRam0010e964[2];
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84b2;
    candidate_c7ebc();
    if (_DAT_0010e99e < (uint)uVar6 * 0x10000) goto LAB_000c840c;
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
    uVar8 = false;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xc84e4;
    func_0x000040ac();
    if ((bool)uVar8) {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84ec;
      candidate_c8524();
      if (!(bool)uVar8) goto LAB_000c840c;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc84fa;
      func_0x000040ac();
      if (!(bool)uVar8) break;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8502;
      candidate_c8524();
    }
    else {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc8514;
      func_0x000040ac();
      if (!(bool)uVar8) goto LAB_000c840c;
      *(undefined4 *)((int)register0x0000003c + -4) = 0xc851c;
      candidate_c8524();
    }
    if (!(bool)uVar8) {
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
    (*(code *)(sVar7 + 0xc844a))();
  }
  psRam0010e964 = (short *)((int)psRam0010e964[1] + (int)psRam0010e964);
  sVar7 = *psRam0010e964;
  goto LAB_000c83ec;
}
