/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0509a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_509a8(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  short sVar5;
  undefined2 unaff_D2w;
  ushort uVar6;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6[3] + 0x6e);
  candidate_9ea8();
  if (*(char *)((int)unaff_A6 + 0x7f) != '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1002;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1002;
  }
  if (*(char *)((int)unaff_A6 + 0x7f) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  iVar4 = 0xc000;
  if (*(char *)((int)unaff_A6 + 0x7f) != '\0') {
    iVar4 = 0x18000;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    iVar4 = -iVar4;
  }
  unaff_A6[0xc] = iVar4;
  unaff_A6[0xd] = 0x50000;
  sVar5 = candidate_29f2();
  unaff_A6[0xd] = sVar5 * 0x10 + unaff_A6[0xd];
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x3c;
  *unaff_A6 = 0x50a66;
  sVar5 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar5;
  if (sVar5 < 0) {
    uVar6 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
    if (0xfc10 < uVar6) {
      uVar6 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar6 & 0xfff;
      uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7))
                        != 0));
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
  *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xd];
  if (*(ushort *)((int)unaff_A6 + 0x7a) < 0x38) {
    candidate_143dc();
  }
  else {
    unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
    candidate_1ac38();
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    iVar4 = *(int *)((int)unaff_A6 + 0x76);
    if (iVar4 < 0) {
      iVar4 = -iVar4;
    }
    unaff_A6[0xd] = iVar4 * 3 >> 2;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  candidate_427a();
  return;
}
