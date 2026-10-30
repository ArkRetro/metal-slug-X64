/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4db6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_a4db6(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  undefined4 uVar6;
  byte bVar8;
  ushort uVar7;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;

  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) | 0x10;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x78;
  *unaff_A6 = 0xa4dda;
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) == 0) {
    uVar6 = candidate_1abc8();
    if ((*(byte *)((int)unaff_A6 + 0x8b) & 1) == 0) {
      bVar9 = false;
      uVar6 = candidate_40bc();
      if (bVar9) {
        *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
        bVar9 = true;
      }
      if (bVar9) {
        *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) | 1;
        *(undefined1 *)(extraout_A0 + 0x8d) = 0;
      }
    }
    if ((*(byte *)((int)unaff_A6 + 0x8b) & 2) == 0) {
      bVar9 = false;
      uVar6 = candidate_40bc();
      if (bVar9) {
        *(undefined4 *)(extraout_A0_00 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
        bVar9 = true;
      }
      if (bVar9) {
        *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) | 2;
        *(undefined1 *)(extraout_A0_00 + 0x8d) = 1;
      }
    }
    if (*(short *)((int)unaff_A6 + 0x86) == 0) {
      bVar3 = *(byte *)((int)unaff_A6 + 0x8b) & 3;
      bVar8 = *(byte *)((int)unaff_A6 + 0x8b) & 3;
      if (bVar3 != 0) {
        if (bVar3 == 3) {
          *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) ^ 0x30;
          bVar8 = (byte)((CONCAT31((int3)((uint)uVar6 >> 8),*(undefined1 *)((int)unaff_A6 + 0x8b)) &
                         0xffffff30) >> 4);
        }
        *(byte *)((int)unaff_A6 + 0x8b) = bVar8 << 2 | *(byte *)((int)unaff_A6 + 0x8b);
        uVar7 = candidate_29c6();
        *(ushort *)((int)unaff_A6 + 0x86) = (uVar7 & 0x7f) + 0x96;
      }
    }
    else {
      *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + -1;
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
    uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7)) !=
                      0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar4));
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
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
