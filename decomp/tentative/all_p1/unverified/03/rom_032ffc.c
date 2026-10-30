/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x032ffc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_32ffc(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  undefined2 uVar4;
  ushort uVar5;
  uint uVar6;
  ushort uVar7;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;
  bool bVar9;

code_r0x00032ffc:
  candidate_4256();
  candidate_333c2();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x3301a;
  uVar8 = false;
  candidate_3334a();
  if ((bool)uVar8) {
LAB_00033242:
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = 0x3325c;
    uVar8 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar8) {
      uVar4 = *(undefined2 *)((short)(*(short *)((int)unaff_A6 + 0x6e) * 2) + 0x32fae);
      candidate_8cd58();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        *(byte *)(extraout_A0 + 0x15) = *(byte *)(extraout_A0 + 0x15) | 1;
      }
      uVar7 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
      if (0xfc10 < uVar7) {
        uVar7 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar5 = uRam002fffe8;
        uVar3 = ~uVar7 & 0xfff;
        uVar7 = CONCAT11((char)((ushort)uVar4 >> 8),
                         -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)
                           ) != 0));
        uRam002fffe0 = uVar7 & CONCAT11(0xff,-(0x7ff < uVar3));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar5) & 1) != 0) {
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
      }
    }
    else {
LAB_00033334:
      candidate_1945c();
      if (!(bool)uVar8) {
        return;
      }
    }
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_3335e();
  if ((bool)uVar8) {
LAB_00033118:
    candidate_4256();
    candidate_333c2();
    if (*(short *)(unaff_A6 + 0xf) <
        (short)(*(short *)((int)unaff_A6 + 0x7a) + *(short *)(unaff_A6[0x1d] + 0x3c))) {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0x10000;
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    else {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0xffff0000;
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    *unaff_A6 = 0x33166;
    bVar9 = false;
    candidate_3334a();
    if (bVar9) goto LAB_00033242;
    candidate_33382();
    if (!bVar9) {
      uVar7 = *(short *)((int)unaff_A6 + 0x7a) + *(short *)(unaff_A6[0x1d] + 0x3c);
      if (*(short *)((int)unaff_A6 + 0x62) < 0) {
        uVar3 = *(ushort *)(unaff_A6 + 0xf);
        if ((short)uVar7 < (short)uVar3) {
LAB_0003319e:
          uVar8 = uVar7 < uVar3;
          candidate_8ccd4();
          candidate_427a();
          goto LAB_00033334;
        }
      }
      else {
        uVar3 = *(ushort *)(unaff_A6 + 0xf);
        if ((short)uVar3 < (short)uVar7) goto LAB_0003319e;
      }
      candidate_4256();
      candidate_333c2();
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      if (*(short *)(unaff_A6 + 0xf) <= *(short *)(unaff_A6[0x1d] + 0x3c)) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      }
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *unaff_A6 = 0x331e8;
      bVar9 = false;
      candidate_3334a();
      if (bVar9) goto LAB_00033242;
      candidate_33382();
      if (!bVar9) {
        *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + 1;
        uVar8 = *(ushort *)(unaff_A6 + 0x1f) < 6;
        if (5 < (short)*(ushort *)(unaff_A6 + 0x1f)) goto code_r0x00033206;
        goto LAB_00033228;
      }
    }
    goto code_r0x00032ffc;
  }
  candidate_8ccd4();
  candidate_427a();
  if ((bool)uVar8) {
    uVar6 = candidate_29c6();
    if ((uVar6 & 1) == 0) {
      *unaff_A6 = candidate_3305c;
      uVar8 = 0;
    }
    else {
      *unaff_A6 = candidate_32ffc;
      uVar8 = 0;
    }
  }
  goto LAB_00033334;
code_r0x00033206:
  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  uVar7 = (*(short *)((int)unaff_A6 + 0x7a) + *(short *)(unaff_A6[0x1d] + 0x3c)) -
          *(short *)(unaff_A6 + 0xf);
  if ((short)uVar7 < 0) {
    uVar7 = -uVar7;
  }
  uVar8 = uVar7 < 0x10;
  if ((short)uVar7 < 0x11) {
LAB_00033228:
    candidate_8ccd4();
    candidate_427a();
    if ((bool)uVar8) {
      *unaff_A6 = 0x331ae;
      uVar8 = 0;
    }
    goto LAB_00033334;
  }
  goto LAB_00033118;
}
