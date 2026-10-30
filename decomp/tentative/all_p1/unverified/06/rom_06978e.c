/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06978e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_6978e(void)

{
  code *pcVar1;
  byte bVar2;
  ushort uVar3;
  ushort uVar4;
  char cVar6;
  int iVar5;
  ushort uVar7;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;

  while( true ) {
    bVar2 = *(byte *)((int)unaff_A6 + 0x12);
    *(byte *)((int)unaff_A6 + 0x12) = bVar2 + 8;
    if (0xf7 < bVar2) {
      *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *(int *)((int)unaff_A6 + 0x62) =
           (*(int *)((int)unaff_A6 + 0x6e) - *(int *)((int)unaff_A6 + 0x62) >> 3) +
           *(int *)((int)unaff_A6 + 0x62);
      candidate_1456a();
      goto LAB_000697f2;
    }
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> (*(ushort *)(unaff_A6 + 0x1d) & 0x3f));
    unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
    func_0x000143e4();
    *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
    if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) || (*(short *)((int)unaff_A6 + 0x76) == 0))
    goto LAB_000697f2;
    candidate_1b1dc();
    uVar7 = *(ushort *)((int)unaff_A6 + 0x76);
    *(ushort *)(unaff_A6 + 0x1e) = uVar7;
    *(short *)(unaff_A6 + 0x1f) = -(short)(0x10000 / uVar7);
    *(short *)((int)unaff_A6 + 0x7e) = (short)(0x20000 / ((uint)uVar7 * (uint)uVar7 & 0xffff));
    *unaff_A6 = 0x6989e;
    uVar7 = *(ushort *)(unaff_A6 + 0x1e) - 1;
    *(ushort *)(unaff_A6 + 0x1e) = uVar7;
    if (-1 < (int)((uint)uVar7 << 0x10)) break;
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0xbf00;
    *unaff_A6 = candidate_6978e;
  }
  candidate_1456a();
LAB_000697f2:
  if ((((*(short *)(unaff_A6 + 0xd) < 1) && ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) &&
      (cVar6 = candidate_49e8(), cVar6 == '\0')) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0))
  {
    uVar7 = *(short *)((int)unaff_A6 + 0x72) + 1;
    if (2 < uVar7) {
      uVar7 = 0;
    }
    *(ushort *)((int)unaff_A6 + 0x72) = uVar7;
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + (short)uVar7 + 0x19);
    iVar5 = unaff_A6[0xc];
    if (iVar5 < 0) {
      iVar5 = -iVar5;
    }
    *(short *)((int)unaff_A6 + 0x26) = (short)((uint)iVar5 >> 8);
    uVar8 = 0;
    candidate_698c6();
    candidate_427a();
    candidate_19504();
    if (!(bool)uVar8) {
      return;
    }
  }
  else {
    candidate_120f8();
    *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
    *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
    candidate_9f88();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x1deb8;
    candidate_9ea8();
    candidate_1dea2();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
    unaff_A6[0x14] = 0;
    *unaff_A6 = 0x1c9e6;
    uVar8 = 0;
    candidate_1abc8();
    candidate_427a();
    if (!(bool)uVar8) {
      candidate_19504();
      if (!(bool)uVar8) {
        return;
      }
      uVar7 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
      if (0xfc10 < uVar7) {
        uVar7 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar3 = ~uVar7 & 0xfff;
        uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)
                           ) != 0));
        uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4) & 1) != 0) {
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
  }
  candidate_4126();
  iVar5 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar5 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
