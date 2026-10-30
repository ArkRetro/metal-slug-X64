/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c4c64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c4c64(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  short sVar5;
  int iVar6;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_4256();
  candidate_9ea8();
  iVar6 = 0x10000;
  iVar4 = *(int *)(unaff_A6[3] + 0x30);
  if (iVar4 < 0) {
    iVar6 = -0x10000;
  }
  unaff_A6[0xc] = iVar6 + iVar4;
  unaff_A6[0xd] = *(undefined4 *)(unaff_A6[3] + 0x34);
  *unaff_A6 = 0xc4cba;
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
  iVar4 = unaff_A6[0xc] * 2;
  if (iVar4 < 0) {
    iVar4 = unaff_A6[0xc] * -2;
  }
  unaff_A6[0xd] = ((iVar4 + -0x60000) - unaff_A6[0xd] >> 4) + unaff_A6[0xd];
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    sVar5 = 0x80;
  }
  else {
    unaff_A6[0xd] = 0;
    sVar5 = 0x400;
  }
  uVar2 = (short)(sVar5 - *(short *)((int)unaff_A6 + 0x26)) >> 3;
  uVar7 = *(ushort *)((int)unaff_A6 + 0x26);
  uVar8 = CARRY2(uVar2,uVar7);
  *(ushort *)((int)unaff_A6 + 0x26) = uVar2 + uVar7;
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar8) {
    uVar7 = *(short *)(unaff_A6 + 0x20) + 1;
    *(ushort *)(unaff_A6 + 0x20) = uVar7;
    if (0xe < uVar7) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    }
    if (0x1d < uVar7) goto candidate_4188;
  }
  if (-1 < (short)(*(short *)(unaff_A6 + 0x10) + -0x100)) {
    return;
  }
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar2 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar7 ^ uVar3) & 1) != 0) {
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
  iVar4 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar4 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
