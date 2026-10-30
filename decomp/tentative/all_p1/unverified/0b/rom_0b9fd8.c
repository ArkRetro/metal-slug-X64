/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b9fd8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_b9fd8(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  undefined2 unaff_D2w;
  ushort uVar5;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  while( true ) {
    candidate_1abc8();
    uVar6 = *(byte *)(*(short *)((int)unaff_A6 + 0x6e) + 0x10c6c0) < 2;
    if (((bool)uVar6) && (candidate_4c24(), (bool)uVar6)) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar6 = 0;
      if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) &&
         (uVar6 = 0, *(short *)((int)unaff_A6 + 0x4e) < 1)) {
        candidate_49e8();
        candidate_120f8();
        (**(code **)(unaff_A6[0x1c] + 0xe))();
        candidate_1da40();
        *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
        *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
        if (*(code **)(unaff_A6[0x1c] + 0x12) != (code *)0x0) {
          (**(code **)(unaff_A6[0x1c] + 0x12))();
        }
        if (-1 < *(char *)(*(short *)((int)unaff_A6 + 0x6e) + 0x2d5502)) {
          candidate_29c6();
          candidate_bb034();
        }
        uVar6 = 0;
        if (*(short *)(unaff_A6[0x1c] + 0x16) == 0) goto candidate_4188;
      }
    }
    candidate_1945c();
    if (!(bool)uVar6) {
      return;
    }
    *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + 0x20;
    if (*(code **)(unaff_A6[0x1c] + 0x1c) == (code *)0x0) break;
    (**(code **)(unaff_A6[0x1c] + 0x1c))();
    if (*(short *)((int)unaff_A6 + 0x6e) == 0x1d) {
      *unaff_A6 = 0xba1b8;
      uVar6 = 0;
      candidate_1abc8();
      candidate_1945c();
      if (!(bool)uVar6) {
        return;
      }
      candidate_ba218();
      *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x200;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)(*(short *)((int)unaff_A6 + 0x6e) + 0x10c6c0) = 0;
      *unaff_A6 = 0xba1f4;
      uVar6 = 0;
      candidate_1abc8();
      candidate_1945c();
      if ((bool)uVar6) {
        return;
      }
    }
    else {
      *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x200;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)(*(short *)((int)unaff_A6 + 0x6e) + 0x10c6c0) = 0;
      *unaff_A6 = 0xba192;
      uVar6 = 0;
      candidate_1abc8();
      candidate_1945c();
      if ((bool)uVar6) {
        return;
      }
    }
    *(undefined2 *)((int)unaff_A6 + 0x4e) = *(undefined2 *)unaff_A6[0x1c];
    if (((undefined2 *)unaff_A6[0x1c])[0x11] != 0) {
      candidate_4c1c();
    }
    *unaff_A6 = candidate_b9fd8;
  }
  uVar5 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
  if (0xfc10 < uVar5) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar5 & 0xfff;
    uVar5 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                      0));
    uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar5 ^ uVar4) & 1) != 0) {
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
