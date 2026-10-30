/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a88fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_a88fc(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  short sVar6;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9c0a();
  candidate_17f20();
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x18;
  *unaff_A6 = 0xa8922;
  sVar6 = *(short *)((int)unaff_A6 + 0x76) + -1;
  *(short *)((int)unaff_A6 + 0x76) = sVar6;
  if (sVar6 != 0) {
    uVar5 = *(short *)((int)unaff_A6 + 0x72) - *(short *)(unaff_A6 + 0x1d);
    if (uVar5 != 0) {
      if (0x5ff < uVar5) {
        if ((short)(uVar5 - 0x600) < 0) {
          uVar5 = uVar5 + 0x600;
        }
        else {
          uVar5 = uVar5 - 0x600;
        }
      }
      if (*(short *)(unaff_A6 + 0x1c) < 0) {
        uVar5 = 0x600 - uVar5;
      }
      uVar4 = (uVar5 >> 4) + 0x20;
      if (uVar5 <= uVar4) {
        uVar4 = uVar5;
      }
      if (*(short *)(unaff_A6 + 0x1c) < 0) {
        uVar4 = -uVar4;
      }
      uVar4 = *(short *)(unaff_A6 + 0x1d) + uVar4;
      sVar6 = 0x600;
      if (0x5ff < uVar4) {
        if ((short)(uVar4 - 0x600) < 0) {
          sVar6 = -0x600;
        }
        uVar4 = uVar4 - sVar6;
      }
      *(ushort *)(unaff_A6 + 0x1d) = uVar4;
    }
    candidate_427a();
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  uVar5 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
  if (0xfc10 < uVar5) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar4 = ~uVar5 & 0xfff;
    uVar5 = (ushort)CONCAT31(0xffffff,-((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) &
                                        '\x01' << (uVar4 & 7)) != 0));
    uRam002fffe0 = uVar5 & (byte)-(0x7ff < uVar4);
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar5 ^ uVar3) & 1) != 0) {
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
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
