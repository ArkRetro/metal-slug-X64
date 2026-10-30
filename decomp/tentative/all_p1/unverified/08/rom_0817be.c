/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0817be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_817be(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 uVar5;
  short sVar6;
  undefined4 uVar4;
  char cVar7;
  int iVar8;
  undefined2 unaff_D2w;
  ushort uVar9;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;

  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  unaff_A6[0x14] = 0xe49e0;
  candidate_1b1dc();
  uVar5 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar5;
  uVar5 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x23) = uVar5;
  sVar6 = candidate_260c8();
  *(int *)((int)unaff_A6 + 0x6e) = (int)sVar6 << 8;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar4 = 0xfffd0000;
  }
  else {
    uVar4 = 0x30000;
  }
  unaff_A6[0xc] = uVar4;
  *unaff_A6 = 0x8184e;
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
  iVar8 = unaff_A6[0xc];
  if (iVar8 < 0) {
    iVar8 = -iVar8;
  }
  if (iVar8 < 0x10001) {
    *unaff_A6 = candidate_81886;
  }
  func_0x000143e4();
  candidate_820ba();
  candidate_427a();
  bVar10 = false;
  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0) {
    candidate_411c();
    candidate_29c6();
    candidate_1b220();
    iVar8 = unaff_A6[0xd];
    bVar10 = (-iVar8 & 1U) != 0;
    *(int *)(extraout_A0 + 0x30) = (int)-unaff_A6[0xc] >> 1;
    *(int *)(extraout_A0 + 0x34) = -iVar8 >> 1;
  }
  candidate_4c24();
  if (bVar10) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  cVar7 = candidate_49e8();
  bVar10 = false;
  if (((cVar7 == '\0') && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) &&
     ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
    candidate_1945c();
    if (!bVar10) {
      return;
    }
  }
  else {
    candidate_ad9e6();
    uVar9 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
    if (0xfc10 < uVar9) {
      uVar9 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar9 & 0xfff;
      uVar9 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7))
                        != 0));
      uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar9 ^ uVar3) & 1) != 0) {
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
  }
  candidate_4126();
  iVar8 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar8 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
