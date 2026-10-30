/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ac870.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ac870(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 uVar6;
  undefined4 unaff_D2;
  ushort uVar7;
  undefined4 unaff_D3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;
  undefined1 uVar9;

  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  bVar8 = *(ushort *)(unaff_A6 + 0x1f) < 7;
  if ((*(ushort *)(unaff_A6 + 0x1f) != 7) || (candidate_19424(), !bVar8)) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    *(undefined1 *)(unaff_A6 + 0x25) = 0xff;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_ae244();
    candidate_353a4();
    candidate_13f1e();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8810;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8810;
    *(undefined2 *)(unaff_A6 + 0x23) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
    *(undefined2 *)((int)unaff_A6 + 0x8e) = 0;
    *unaff_A6 = 0xac8d4;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      unaff_A6[0xc] = 0;
      uVar3 = unaff_A6[0xd];
      uVar9 = 0x4fff < uVar3;
      unaff_A6[0xd] = uVar3 - 0x5000;
      func_0x000143e4();
    }
    else {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      uVar9 = 0;
      candidate_1456a();
    }
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      candidate_c8bec();
      if (!(bool)uVar9) {
        uVar6 = candidate_aa610();
        if ((bool)uVar9) {
          *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar6;
          bVar8 = false;
          uVar6 = candidate_aa760();
          if (bVar8) {
            *(undefined2 *)(unaff_A6 + 0x1c) = uVar6;
            candidate_ae852();
            *unaff_A6 = candidate_acac0;
            candidate_120f8();
            *(undefined2 *)(unaff_A6 + 0x23) = 0x1e;
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
          }
        }
        return;
      }
      uVar7 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
      if (0xfc10 < uVar7) {
        uVar7 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar5 = uRam002fffe8;
        uVar4 = ~uVar7 & 0xfff;
        uVar7 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                                 -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                                   '\x01' << (uVar4 & 7)) != 0));
        uRam002fffe0 = uVar7 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar4));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar7 ^ uVar5) & 1) != 0) {
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
      goto candidate_4188;
    }
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_120f8();
  candidate_ae220();
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0 + 0x30) = 0xfff30000;
  *(undefined4 *)(extraout_A0 + 0x34) = 0x180000;
  *(undefined2 *)(extraout_A0 + 0x4e) = 10;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_00 + 0x30) = 0xb0000;
  *(undefined4 *)(extraout_A0_00 + 0x34) = 0x150000;
  *(undefined2 *)(extraout_A0_00 + 0x4e) = 7;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_01 + 0x30) = 0x80000;
  *(undefined4 *)(extraout_A0_01 + 0x34) = 0xfffc0000;
  *(undefined2 *)(extraout_A0_01 + 0x4e) = 5;
  candidate_411c();
  candidate_1acf2();
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
