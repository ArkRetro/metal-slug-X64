/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05b950.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_5b950(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  undefined2 unaff_D2w;
  ushort uVar5;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int iVar6;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;

  if ((*(byte *)(unaff_A6 + 0x21) & 4) == 0) {
    candidate_120f8();
    *(ushort *)((int)unaff_A6 + 0xaa) = *(ushort *)((int)unaff_A6 + 0xaa) >> 8;
    *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
    unaff_A6[0xd] = 0;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffffe000;
    sVar4 = *(short *)(unaff_A6 + 0x2a);
    if (0x600 < sVar4) {
      sVar4 = 0x600;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar4 = -sVar4;
    }
    unaff_A6[0xc] = (int)sVar4 << 8;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
    *unaff_A6 = 0x5b9ae;
    bVar7 = false;
    candidate_5c050();
    if (bVar7) {
      *unaff_A6 = 0x5ba32;
    }
    candidate_5c132();
    candidate_427a();
    candidate_49e8();
    bVar7 = false;
    if ((*(char *)((int)unaff_A6 + 0x83) != '\0') &&
       (bVar7 = *(ushort *)(unaff_A6 + 0x10) < 0x120, (short)*(ushort *)(unaff_A6 + 0x10) < 0x121))
    {
      *unaff_A6 = candidate_5b9fe;
      bVar7 = false;
    }
    candidate_1945c();
    if (bVar7) {
      *unaff_A6 = 0x5bb1a;
    }
    return;
  }
  if (((*(byte *)(unaff_A6 + 0x21) & 4) == 0) && (*(char *)((int)unaff_A6 + 0x83) == '\0')) {
    candidate_c8022();
  }
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_120f8();
  bVar7 = false;
  candidate_40bc();
  if (bVar7) {
    candidate_1b220();
  }
  candidate_1da40();
  bVar7 = false;
  candidate_40bc();
  if (bVar7) {
    candidate_1b220();
  }
  bVar7 = false;
  candidate_40bc();
  if (bVar7) {
    candidate_1b220();
  }
  bVar7 = false;
  candidate_40bc();
  if (bVar7) {
    candidate_1b220();
  }
  bVar7 = false;
  candidate_40bc();
  iVar6 = extraout_A0;
  if (bVar7) {
    candidate_1b220();
    iVar6 = extraout_A0_00;
  }
  *(byte *)(iVar6 + 0x15) = *(byte *)(iVar6 + 0x15) ^ 1;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  uVar5 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
  if (0xfc10 < uVar5) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar2 = ~uVar5 & 0xfff;
    uVar5 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0));
    uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar5 ^ uVar3) & 1) != 0) {
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
  iVar6 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar6 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
