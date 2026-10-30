/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x080b06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_80b06(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  byte bVar4;
  ushort uVar5;
  byte bVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;
  bool bVar10;

  *(undefined2 *)((int)unaff_A6 + 0x7a) = *(undefined2 *)(unaff_A6 + 0x20);
  *(undefined1 *)(unaff_A6 + 0x25) = 0;
  *unaff_A6 = 0x80b16;
  sVar3 = *(short *)((int)unaff_A6 + 0x7a);
  uVar9 = sVar3 == 0;
  sVar3 = sVar3 + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar3;
  if (sVar3 < 0) {
    *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)((int)unaff_A6 + 0x93);
    candidate_4256();
    *(undefined2 *)((int)unaff_A6 + 0x8e) = *(undefined2 *)(unaff_A6 + 0x23);
    *unaff_A6 = 0x80b52;
    uVar9 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar9) {
      bVar4 = *(byte *)(unaff_A6 + 0x24);
      bVar6 = bVar4 - 1;
      *(byte *)(unaff_A6 + 0x24) = bVar6;
      if (bVar6 == 0 || SBORROW1(bVar4,'\x01') != (short)((ushort)bVar6 << 8) < 0) {
        *unaff_A6 = candidate_80ab0;
      }
      else {
        *unaff_A6 = candidate_80b80;
      }
    }
    sVar3 = *(short *)((int)unaff_A6 + 0x8e);
    uVar9 = sVar3 == 0;
    sVar3 = sVar3 + -1;
    *(short *)((int)unaff_A6 + 0x8e) = sVar3;
    *(char *)(unaff_A6 + 0x25) = -(sVar3 < 0);
  }
  else {
    candidate_1abc8();
    candidate_427a();
  }
  candidate_1945c();
  if (!(bool)uVar9) {
    candidate_4c24();
    if ((bool)uVar9) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      return;
    }
    candidate_4256();
    bVar10 = false;
    candidate_40bc();
    if (bVar10) {
      candidate_1b220();
    }
    candidate_810f6();
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    *unaff_A6 = 0x80c2e;
    uVar9 = 0;
    candidate_1abc8();
    candidate_427a();
    candidate_1945c();
    if (!(bool)uVar9) {
      return;
    }
  }
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar7 = uRam002fffe8;
    uVar5 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)) !=
                      0));
    uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar7) & 1) != 0) {
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
