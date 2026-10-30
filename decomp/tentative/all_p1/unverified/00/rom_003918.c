/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x003918.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3918(void)

{
  char cVar1;
  ushort uVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  undefined1 in_D0b;
  short sVar6;
  char cVar8;
  undefined2 uVar7;
  undefined4 unaff_D2;
  undefined4 unaff_D3;
  int unaff_A5;
  int unaff_A6;

  _DAT_003c0004 = 0;
  _DAT_003c0000 = 0x82000000;
  if ((DAT_0010f000 & 4) != 0) {
    DAT_003a0011 = in_D0b;
  }
  candidate_698c();
  candidate_c7aa8();
  DAT_003a0001 = candidate_11cfe();
  if (((DAT_0010f000 & 1) != 0) && (DAT_0010008d != '\0')) {
    sVar6 = (_DAT_003c0006 >> 7) - 0x100;
    if (_DAT_003c0006 >> 7 < 0x100) {
      sVar6 = 0;
    }
    *(short *)(unaff_A5 + -0x7f70) = sVar6;
    *(undefined1 *)(unaff_A5 + -0x7f70) = *(undefined1 *)(unaff_A5 + 0x7009);
    candidate_3d3b2();
  }
  *(undefined2 *)(unaff_A5 + -0x1c26) = 0;
  _DAT_003c0000 = 0x82000000;
  do {
    uVar2 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar2 + 0x3ef;
    if (0xfc10 < uVar2) {
      uVar2 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar4 = ~uVar2 & 0xfff;
      unaff_D2 = CONCAT31((int3)((uint)unaff_D2 >> 8),
                          -((*(byte *)((short)(uVar2 >> 3 & 0xfff) + 0xdedd2) &
                            '\x01' << (uVar4 & 7)) != 0));
      uRam002fffe0 = (ushort)unaff_D2 &
                     (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar4));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ff) +
           *(short *)(unaff_A5 + 0x700a);
      unaff_D3 = 0;
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & ((ushort)unaff_D2 ^ uVar5) & 1) != 0)
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
        func_0x0003cf12();
        candidate_3e2a8();
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
    }
    bVar3 = *(byte *)(unaff_A5 + -0x7f75);
    *(byte *)(unaff_A5 + -0x7f75) = bVar3 & 0x7f;
  } while ((bVar3 & 0x80) == 0);
  while( true ) {
    while( true ) {
      *(undefined1 *)(unaff_A5 + -0x7f76) = *(undefined1 *)(unaff_A5 + 0x7009);
      *(undefined1 *)(unaff_A5 + 0x7009) = 0;
      _DAT_003c0000 = 0x82000000;
      candidate_26fa();
      candidate_3cb3a();
      if (*(code **)(unaff_A5 + -0x7f6e) == (code *)0x0) break;
      (**(code **)(unaff_A5 + -0x7f6e))();
      candidate_698c();
      do {
      } while (*(char *)(unaff_A5 + 0x7009) == '\0');
    }
    cVar8 = candidate_15520();
    cVar1 = *(char *)(unaff_A5 + -0x7f73);
    *(char *)(unaff_A5 + -0x7f73) = cVar8;
    if (cVar8 != '\0') break;
    if (cVar1 != '\0') {
      uVar7 = candidate_2836();
      *(undefined2 *)(unaff_A6 + 0x72) = uVar7;
      candidate_2802();
    }
    if (cVar1 == '\x01') {
      candidate_9bbc();
      *(undefined1 *)(unaff_A6 + 0x70) = 0xff;
      candidate_9c0a();
    }
    candidate_698c();
    do {
    } while (*(byte *)(unaff_A5 + 0x7009) < 2);
  }
  if (cVar1 == '\0') {
    candidate_2802();
    if (*(char *)(unaff_A6 + 0x70) != '\0') {
      candidate_9bd4();
      *(undefined1 *)(unaff_A6 + 0x70) = 0;
    }
    *(undefined1 *)(unaff_A5 + -0x7f72) = 0xff;
  }
  else {
    *(undefined1 *)(unaff_A5 + -0x7f72) = 0;
  }
  if (*(short *)(unaff_A6 + 0x6e) != *(short *)(unaff_A5 + -0x7ff2)) {
    __m68k_trap(0xf);
  }
  sVar6 = *(short *)(unaff_A6 + 0x6e) + 1;
  *(short *)(unaff_A5 + -0x7ff2) = sVar6;
  *(short *)(unaff_A6 + 0x6e) = sVar6;
  candidate_1ab50();
  candidate_1ae4a();
  return;
}
