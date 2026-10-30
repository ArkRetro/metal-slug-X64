/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x081886.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_81886(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar4;
  short extraout_D1w;
  uint uVar5;
  int iVar6;
  uint unaff_D2;
  ushort uVar7;
  undefined2 unaff_D3w;
  undefined4 extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;

  *(undefined4 *)((int)unaff_A6 + 0x7e) = 0;
  bVar8 = false;
  candidate_8240c();
  if (bVar8) {
    *unaff_A6 = 0x81924;
    unaff_A6[0xd] = unaff_A6[0xd] - ((int)unaff_A6[0xd] >> 3);
    uVar5 = (uint)(short)-((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0);
    unaff_A6[0xc] = ((uVar5 ^ 0x5000) - uVar5) + unaff_A6[0xc];
    if (*(int *)((int)unaff_A6 + 0x6e) < (int)((uVar5 ^ unaff_A6[0xc]) - uVar5)) {
      unaff_A6[0xc] = (uVar5 ^ *(uint *)((int)unaff_A6 + 0x6e)) - uVar5;
    }
    func_0x000143e4();
    candidate_820ba();
    candidate_427a();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x7a) = extraout_A0;
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0x3c;
    *unaff_A6 = 0x818a2;
    candidate_1b162();
    uVar7 = (ushort)-((int)((uint)(ushort)(extraout_D1w - *(short *)(unaff_A6 + 0x10)) << 0x10) < 0)
    ;
    unaff_D2 = (uint)(short)uVar7;
    unaff_A6[0xd] = ((int)(((unaff_D2 ^ 0x28000) - unaff_D2) - unaff_A6[0xd]) >> 3) + unaff_A6[0xd];
    if ((ushort)((uVar7 ^ extraout_D1w - *(short *)(unaff_A6 + 0x10)) - uVar7) < 5) {
      *unaff_A6 = 0x8191e;
    }
    uVar7 = *(ushort *)((int)unaff_A6 + 0x86) - 1;
    *(ushort *)((int)unaff_A6 + 0x86) = uVar7;
    if ((int)((uint)uVar7 << 0x10) < 0) {
      *unaff_A6 = 0x8191e;
    }
    iVar6 = unaff_A6[0xd];
    if (iVar6 < 0) {
      iVar6 = -iVar6;
    }
    *(int *)((int)unaff_A6 + 0x7e) = iVar6 + *(int *)((int)unaff_A6 + 0x7e);
    if (*(short *)(unaff_A6 + 0x23) < *(short *)((int)unaff_A6 + 0x7e)) {
      *unaff_A6 = 0x8191e;
    }
    func_0x000143e4();
    candidate_820ba();
    candidate_427a();
  }
  bVar8 = false;
  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0) {
    candidate_411c();
    candidate_29c6();
    candidate_1b220();
    iVar6 = unaff_A6[0xd];
    bVar8 = (-iVar6 & 1U) != 0;
    *(int *)(extraout_A0_00 + 0x30) = (int)-unaff_A6[0xc] >> 1;
    *(int *)(extraout_A0_00 + 0x34) = -iVar6 >> 1;
  }
  candidate_4c24();
  if (bVar8) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  cVar4 = candidate_49e8();
  bVar8 = false;
  if (((cVar4 == '\0') && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) &&
     ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
    candidate_1945c();
    if (!bVar8) {
      return;
    }
  }
  else {
    candidate_ad9e6();
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar7 & 0xfff;
      uVar7 = (ushort)CONCAT31((int3)(unaff_D2 >> 8),
                               -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar2 & 7)) != 0));
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
