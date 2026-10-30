/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0370fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_370fa(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  undefined2 unaff_D2w;
  ushort uVar4;
  undefined2 unaff_D3w;
  undefined4 unaff_D7;
  ushort uVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    func_0x000143e4();
    unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
    if (*(char *)((int)unaff_A6 + 0x76) != '\0') {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
    }
  }
  else {
    candidate_1456a();
  }
  uVar4 = *(ushort *)(unaff_A6 + 0x1d);
  uVar5 = (ushort)((uint)unaff_D7 >> 0x10);
  *(ushort *)(unaff_A6 + 0x1d) = uVar5;
  uVar6 = uVar5 < uVar4;
  if (uVar5 != uVar4) {
    if (uVar5 == 5) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        uVar6 = 0;
        candidate_4256();
      }
      else {
        uVar6 = 0;
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0x76) = 0xff;
      }
    }
    else {
      uVar6 = 0;
      candidate_4256();
    }
  }
  candidate_427a();
  candidate_1945c();
  if (!(bool)uVar6) {
    return;
  }
  uVar4 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar4 + 0x3ef;
  if (0xfc10 < uVar4) {
    uVar4 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar5 = ~uVar4 & 0xfff;
    uVar4 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar4 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)) !=
                      0));
    uRam002fffe0 = uVar4 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar4 ^ uVar3) & 1) != 0) {
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
