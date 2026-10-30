/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ad18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8ad18(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  byte bVar4;
  ushort uVar5;
  ushort uVar6;
  short sVar7;
  ushort uVar8;
  undefined2 unaff_D2w;
  undefined1 uVar9;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;
  undefined1 uVar11;
  char cVar12;
  bool bVar13;

  unaff_A6[0xc] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xc];
  uVar3 = unaff_A6[0xd];
  uVar11 = CARRY4(*(uint *)((int)unaff_A6 + 0x7a),uVar3);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x7a) + uVar3;
  func_0x000143e4();
  candidate_1945c();
  if ((bool)uVar11) goto candidate_4188;
  cVar12 = '\0';
  if (*(char *)((int)unaff_A6 + 0x82) == '\0') {
    candidate_3c5c6();
    *(char *)((int)unaff_A6 + 0x82) = -cVar12;
  }
  uVar11 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) == 0) {
    sVar7 = candidate_49e8();
    if (sVar7 == 0) {
      candidate_8af5a();
      candidate_427a();
      bVar4 = *(byte *)((int)unaff_A6 + 0x81);
      *(byte *)((int)unaff_A6 + 0x81) = bVar4 - 0x38;
      if (bVar4 < 0x38) {
        bVar13 = false;
        candidate_40bc();
        if (bVar13) {
          candidate_1b220();
        }
      }
      return;
    }
  }
  else if (*(char *)((int)unaff_A6 + 0x82) != '\0') {
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar6 = uRam002fffe8;
      uVar5 = ~uVar8 & 0xfff;
      uVar8 = CONCAT11(uVar11,-((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                '\x01' << (uVar5 & 7)) != 0));
      uRam002fffe0 = uVar8 & CONCAT11(uVar9,-(0x7ff < uVar5));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar6) & 1) == 0)
      goto candidate_4188;
      goto LAB_00003c64;
    }
    goto candidate_4188;
  }
  candidate_120f8();
  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  unaff_A6[0x14] = 0;
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x82) = 0x1deb8;
  candidate_9ea8();
  candidate_1dea2();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
  unaff_A6[0x14] = 0;
  *unaff_A6 = 0x1c9e6;
  uVar10 = 0;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar10) {
    candidate_19504();
    if (!(bool)uVar10) {
      return;
    }
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar6 = uRam002fffe8;
      uVar5 = ~uVar8 & 0xfff;
      uVar8 = CONCAT11(uVar11,-((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                '\x01' << (uVar5 & 7)) != 0));
      uRam002fffe0 = uVar8 & CONCAT11(uVar9,-(0x7ff < uVar5));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar6) & 1) != 0) {
LAB_00003c64:
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
