/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0706e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_706e6(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 extraout_D0u;
  int iVar4;
  char cVar7;
  uint uVar5;
  byte bVar8;
  short extraout_D1w;
  undefined4 uVar9;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;
  undefined8 uVar11;
  ushort uVar6;

  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  uVar11 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar11;
  candidate_1abc8();
  candidate_411c();
  candidate_1acf2();
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x87) = 4;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 8;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0;
  *unaff_A6 = 0x70778;
  uVar6 = *(ushort *)((int)unaff_A6 + 0x7e) - 1;
  *(ushort *)((int)unaff_A6 + 0x7e) = uVar6;
  if ((int)((uint)uVar6 << 0x10) < 0) {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 8;
    *unaff_A6 = 0x707a2;
    uVar6 = *(ushort *)((int)unaff_A6 + 0x7e) - 1;
    *(ushort *)((int)unaff_A6 + 0x7e) = uVar6;
    if ((int)((uint)uVar6 << 0x10) < 0) {
      *unaff_A6 = 0x707d0;
      candidate_49e8();
      iVar4 = 0;
      if (extraout_A0 != 0) {
        candidate_1b162();
        iVar4 = CONCAT22(extraout_D0u,extraout_D1w - *(short *)(unaff_A6 + 0x10));
      }
      if ((short)iVar4 != 0) {
        if ((short)iVar4 < 0) {
          iVar4 = -0x20000;
        }
        else {
          iVar4 = 0x20000;
        }
      }
      *(int *)((int)unaff_A6 + 0x76) = iVar4 - unaff_A6[0xd] >> 4;
      *(int *)((int)unaff_A6 + 0x72) = 0x60000 - unaff_A6[0xc] >> 4;
    }
    else {
      *(int *)((int)unaff_A6 + 0x72) = (int)-unaff_A6[0xc] >> 3;
      *(int *)((int)unaff_A6 + 0x76) = -0x20000 - unaff_A6[0xd] >> 3;
    }
  }
  else {
    *(int *)((int)unaff_A6 + 0x72) = (int)-unaff_A6[0xc] >> 3;
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  }
  unaff_A6[0xc] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xc];
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  func_0x000143e4();
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) && (cVar7 = candidate_49e8(), cVar7 == '\0')) {
    cVar7 = candidate_2eb4();
    *(short *)(unaff_A6 + 0x21) =
         ((short)((ushort)(byte)((cVar7 + 0x40U & 0x7f) - 0x40) * 0x100 -
                 *(short *)(unaff_A6 + 0x21)) >> 3) + *(short *)(unaff_A6 + 0x21);
    cVar7 = *(char *)(unaff_A6 + 0x21);
    uVar6 = (ushort)cVar7;
    if ((short)uVar6 < 0) {
      uVar6 = CONCAT11(cVar7 >> 7,-cVar7);
    }
    uVar5 = (uint)uVar6;
    if (0x20 < (byte)uVar6) {
      uVar5 = CONCAT31((uint3)(byte)(uVar6 >> 8),0x20);
    }
    bVar8 = (byte)uVar5 >> 3;
    uVar5 = CONCAT31((int3)(uVar5 >> 8),bVar8);
    if ((short)uVar5 < 0) {
      uVar5 = (uint)(byte)-bVar8;
    }
    *(char *)((int)unaff_A6 + 0x87) = (char)uVar5 + '\x04';
    uVar10 = 0;
    candidate_427a();
    candidate_19504();
    if (!(bool)uVar10) {
      *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
      uVar6 = *(ushort *)((int)unaff_A6 + 0x82) & 3;
      *(ushort *)((int)unaff_A6 + 0x82) = uVar6;
      if (uVar6 == 0) {
        candidate_411c();
        uVar9 = unaff_A6[0xd];
        *(undefined4 *)(extraout_A0_00 + 0x30) = unaff_A6[0xc];
        *(undefined4 *)(extraout_A0_00 + 0x34) = uVar9;
        uVar9 = *(undefined4 *)((int)unaff_A6 + 0x76);
        *(undefined4 *)(extraout_A0_00 + 0x72) = *(undefined4 *)((int)unaff_A6 + 0x72);
        *(undefined4 *)(extraout_A0_00 + 0x76) = uVar9;
        candidate_29c6();
        candidate_1acf2();
      }
      return;
    }
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_01 + 0x30) = 0x200000;
      *(undefined4 *)(extraout_A0_01 + 0x34) = 0x180000;
      *(undefined2 *)(extraout_A0_01 + 0x4e) = 7;
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_02 + 0x30) = 0x1a0000;
      *(undefined4 *)(extraout_A0_02 + 0x34) = 0x30000;
      *(undefined2 *)(extraout_A0_02 + 0x4e) = 6;
    }
    else {
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_03 + 0x30) = 0;
      *(undefined4 *)(extraout_A0_03 + 0x34) = 0;
      *(undefined2 *)(extraout_A0_03 + 0x4e) = 0;
    }
    candidate_411c();
    candidate_1acf2();
  }
  uVar6 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
  if (0xfc10 < uVar6) {
    uVar6 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar2 = ~uVar6 & 0xfff;
    uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0));
    uRam002fffe0 = uVar6 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar6 ^ uVar3) & 1) != 0) {
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
  candidate_4126();
  iVar4 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar4 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
