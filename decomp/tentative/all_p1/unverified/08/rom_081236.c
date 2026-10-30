/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x081236.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_81236(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar6;
  uint uVar4;
  int iVar5;
  ushort uVar7;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;

  *(undefined1 *)((int)unaff_A6 + 0x9d) = 3;
  sVar6 = candidate_29f2();
  *(short *)(unaff_A6 + 0x24) = sVar6 + 0x120;
  *unaff_A6 = 0x81258;
  uVar7 = (ushort)-((int)((uint)(ushort)(*(short *)(unaff_A6 + 0x24) - *(short *)(unaff_A6 + 0x10))
                         << 0x10) < 0);
  uVar4 = (int)((uint)(ushort)((uVar7 ^ *(short *)(unaff_A6 + 0x24) - *(short *)(unaff_A6 + 0x10)) -
                              uVar7) << 0x10) >> 4;
  if (*(int *)((int)unaff_A6 + 0x6e) < (int)uVar4) {
    uVar4 = *(uint *)((int)unaff_A6 + 0x6e);
  }
  *(int *)((int)unaff_A6 + 0x72) =
       ((int)((((int)(short)uVar7 ^ uVar4) - (int)(short)uVar7) - *(int *)((int)unaff_A6 + 0x72)) >>
       4) + *(int *)((int)unaff_A6 + 0x72);
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x72);
  iVar5 = *(int *)((int)unaff_A6 + 0x6e) + (*(int *)((int)unaff_A6 + 0x6e) >> 1);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar5 = -iVar5;
  }
  unaff_A6[0xc] = (iVar5 - unaff_A6[0xc] >> 4) + unaff_A6[0xc];
  candidate_1ac38();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0xf) < 0x50) {
      *unaff_A6 = candidate_812ee;
    }
  }
  else if (0xf0 < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_812ee;
  }
  candidate_4c24();
  if ((*(char *)((int)unaff_A6 + 0x5a) != '\0') && (*(char *)((int)unaff_A6 + 0x9b) != '\x01')) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_813ee;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_4256();
  }
  if (*(char *)((int)unaff_A6 + 0x9b) == '\0') {
    candidate_81ff8();
  }
  candidate_820f8();
  if ('\0' < *(char *)((int)unaff_A6 + 0x9a)) {
    *(char *)((int)unaff_A6 + 0x9a) = *(char *)((int)unaff_A6 + 0x9a) + -1;
  }
  bVar8 = false;
  if (0 < *(short *)(unaff_A6 + 0x25)) {
    sVar6 = *(short *)(unaff_A6 + 0x25);
    bVar8 = sVar6 == 0;
    *(short *)(unaff_A6 + 0x25) = sVar6 + -1;
  }
  candidate_1945c();
  if (bVar8) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7))
                        != 0));
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
    candidate_4126();
    iVar5 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar5 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
