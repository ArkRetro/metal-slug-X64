/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dca54.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_dca54(void)

{
  code *pcVar1;
  byte bVar2;
  ushort uVar3;
  ushort uVar4;
  byte bVar7;
  ushort uVar5;
  undefined2 uVar6;
  undefined1 uVar8;
  undefined2 unaff_D2w;
  undefined1 uVar9;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int iVar10;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_ZF;
  undefined1 uVar11;
  undefined1 uVar12;
  bool bVar13;

  while( true ) {
    uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
    uVar8 = (undefined1)((ushort)unaff_D2w >> 8);
    candidate_dd850();
    candidate_dd892();
    candidate_dd918();
    candidate_dd5a4();
    candidate_dd608();
    if (!(bool)in_ZF) break;
    uVar12 = false;
    candidate_ddab4();
    if ((bool)uVar12) {
      *unaff_A6 = 0xdca9c;
      uVar12 = 0;
      candidate_dd850();
      candidate_dd61e();
      candidate_dd918();
      candidate_dd5a4();
      candidate_dd608();
      if (!(bool)uVar12) break;
      uVar12 = false;
      candidate_ddab4();
      if ((bool)uVar12) {
        candidate_ddae0();
        if ((bool)uVar12) goto LAB_000dcbde;
        candidate_dd57e();
        if ((bool)uVar12) goto LAB_000dcbb6;
        candidate_1ac38();
        candidate_427a();
        goto LAB_000dda06;
      }
    }
    else {
      candidate_ddae0();
      if (!(bool)uVar12) {
        candidate_dd57e();
        if ((bool)uVar12) {
LAB_000dcbb6:
          *(undefined2 *)(unaff_A6 + 0x23) = 0x2ff;
          *unaff_A6 = 0xdcbc2;
          uVar12 = 0;
          candidate_dd892();
          candidate_dd918();
          candidate_dd5a4();
          candidate_1ac38();
          candidate_427a();
        }
        else {
          candidate_1ac38();
          candidate_427a();
        }
        goto LAB_000dda06;
      }
LAB_000dcbde:
      while( true ) {
        uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
        uVar8 = (undefined1)((ushort)unaff_D2w >> 8);
        *unaff_A6 = 0xdcbe4;
        if (*(byte *)((int)unaff_A6 + 0x9b) != 3) {
          if (*(byte *)((int)unaff_A6 + 0x9b) < 3) {
            bVar7 = 1;
          }
          else {
            bVar7 = 0xff;
          }
          bVar2 = *(byte *)((int)unaff_A6 + 0x9b);
          uVar12 = CARRY1(bVar7,bVar2);
          bVar7 = bVar7 + bVar2;
          *(byte *)((int)unaff_A6 + 0x9b) = bVar7;
          uVar11 = bVar7 == 0;
          goto LAB_000dcc02;
        }
        if (*(char *)((int)unaff_A6 + 0xa1) != '\0') break;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          if (*(char *)((int)unaff_A6 + 0xa3) == '\0') {
LAB_000dcc62:
            candidate_4256();
            *unaff_A6 = 0xdcc78;
            uVar11 = 0;
            uVar12 = 0;
            candidate_dd5a4();
            candidate_dd608();
            if (!(bool)uVar11) goto LAB_000dcadc;
            candidate_1abc8();
            candidate_427a();
            if ((bool)uVar12) {
              *unaff_A6 = 0xdcc9e;
              uVar12 = 0;
            }
            goto LAB_000dda06;
          }
        }
        else if (*(char *)((int)unaff_A6 + 0xa3) != '\0') goto LAB_000dcc62;
        bVar13 = false;
        candidate_40bc();
        iVar10 = extraout_A0;
        if (bVar13) {
          candidate_1b220();
          iVar10 = extraout_A0_00;
        }
        *(undefined2 *)(iVar10 + 0x7c) = *(undefined2 *)(unaff_A6 + 0x1f);
        *(undefined1 *)((int)unaff_A6 + 0xa1) = 1;
        *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
        *(undefined1 *)((int)unaff_A6 + 0xa2) = 0;
        *(undefined1 *)(unaff_A6 + 0x18) = 0;
        candidate_4256();
        *unaff_A6 = 0xdccf0;
        uVar12 = false;
        uVar11 = *(char *)(unaff_A6 + 0x18) == '\0';
        if ((bool)uVar11) goto LAB_000dcc02;
        uVar6 = 0x40;
        if (*(char *)((int)unaff_A6 + 0xa3) != '\0') {
          uVar6 = 0x100;
        }
        *(undefined2 *)((int)unaff_A6 + 0x8a) = uVar6;
        *(undefined2 *)(unaff_A6 + 0x23) = 0x180;
        *unaff_A6 = 0xdcd18;
        candidate_dd892();
        unaff_D2w = *(undefined2 *)((int)unaff_A6 + 0x8a);
        uVar8 = (undefined1)((ushort)unaff_D2w >> 8);
        unaff_D3w = *(undefined2 *)(unaff_A6 + 0x23);
        uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
        uVar5 = candidate_1b29c();
        uVar12 = uVar5 < 10;
        uVar11 = uVar5 == 10;
        if (10 < uVar5) goto LAB_000dcd3a;
        *(undefined2 *)(unaff_A6 + 0x24) = 0;
        *(undefined1 *)((int)unaff_A6 + 0x9d) = 0xff;
        *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
        *(undefined1 *)(unaff_A6 + 0x29) = 0;
        *unaff_A6 = 0xdcd70;
        uVar12 = 0;
        uVar11 = *(char *)(unaff_A6 + 0x29) == '\0';
        if ((bool)uVar11) {
          candidate_ddc0a();
          candidate_dd5a4();
          candidate_dd608();
          if (!(bool)uVar11) goto LAB_000dcadc;
          candidate_1abc8();
          candidate_427a();
          if ((bool)uVar12) {
            *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
            uVar12 = 0;
            candidate_4256();
          }
          goto LAB_000dda06;
        }
        uVar6 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x24) = uVar6;
        *(undefined1 *)((int)unaff_A6 + 0x9d) = 0xff;
      }
      *(undefined1 *)((int)unaff_A6 + 0xa1) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 3;
      *(undefined1 *)(unaff_A6 + 0x18) = 0;
      *unaff_A6 = 0xdcc3a;
      uVar12 = false;
      uVar11 = *(char *)(unaff_A6 + 0x18) == '\0';
      if ((bool)uVar11) {
LAB_000dcc02:
        candidate_dd5a4();
        candidate_dd608();
        if ((bool)uVar11) {
          candidate_1abc8();
          candidate_427a();
          goto LAB_000dda06;
        }
        break;
      }
      candidate_dd850();
      *(undefined2 *)((int)unaff_A6 + 0x8a) = *(undefined2 *)(unaff_A6 + 0xf);
      *unaff_A6 = 0xdcdda;
      candidate_dd892();
      uVar5 = *(ushort *)(unaff_A6 + 0x23);
      uVar3 = *(ushort *)(unaff_A6 + 0x10);
      uVar12 = uVar5 < uVar3;
      uVar11 = uVar5 == uVar3;
      if ((short)uVar3 < (short)uVar5) {
LAB_000dcd3a:
        candidate_dd5a4();
        candidate_dd608();
        if ((bool)uVar11) {
          candidate_1ac38();
          candidate_427a();
          goto LAB_000dda06;
        }
        break;
      }
    }
    *unaff_A6 = candidate_dca54;
    in_ZF = 0;
  }
LAB_000dcadc:
  *(undefined2 *)(unaff_A6 + 0x23) = 0x1ff;
  candidate_4256();
  *unaff_A6 = 0xdcaf8;
  uVar12 = 0;
  candidate_dd892();
  candidate_dd5a4();
  candidate_dd4ca();
  candidate_1ac38();
  candidate_dd4fc();
  if ((bool)uVar12) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
    candidate_19dfc();
    uVar5 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
    if (uVar5 < 0xfc11) goto candidate_4188;
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar5 & 0xfff;
    uVar5 = CONCAT11(uVar8,-((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar5 & CONCAT11(uVar9,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar5 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar5 ^ uVar4);
  }
  else {
    candidate_427a();
LAB_000dda06:
    candidate_1945c();
    if (!(bool)uVar12) {
      return;
    }
    *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
    candidate_12192();
    uVar5 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
    if (uVar5 < 0xfc11) goto candidate_4188;
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar5 & 0xfff;
    uVar5 = CONCAT11(uVar8,-((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar5 & CONCAT11(uVar9,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar5 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar5 ^ uVar4);
  }
  if ((uVar5 & 1) != 0) {
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
    func_0x0003cf12();
    candidate_3e2a8();
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
candidate_4188:
  candidate_4126();
  iVar10 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar10 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
