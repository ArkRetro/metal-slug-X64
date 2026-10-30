/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048ab6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_48ab6(void)

{
  code *pcVar1;
  int iVar2;
  char *pcVar3;
  byte bVar4;
  ushort uVar5;
  ushort uVar6;
  ushort uVar7;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;

  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
  bVar4 = *(byte *)(unaff_A6[3] + 0x61);
  candidate_8d1d0();
  candidate_1ad52();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  candidate_1da40();
  candidate_40bc();
  candidate_1acf2();
  candidate_40bc();
  candidate_1acf2();
  candidate_40bc();
  candidate_1acf2();
  candidate_40bc();
  candidate_1acf2();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9f88();
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    pcVar3 = (char *)(*(int *)(unaff_A6[3] + 0xc) + 0x96);
    *pcVar3 = *pcVar3 + -1;
  }
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  candidate_120f8();
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x82) = 0x1dec8;
  candidate_9ea8();
  candidate_1dea2();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
  unaff_A6[0x14] = 0;
  *unaff_A6 = 0x1c9e6;
  uVar8 = 0;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar8) {
    candidate_19504();
    if (!(bool)uVar8) {
      return;
    }
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar6 = uRam002fffe8;
      uVar5 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11(0xff,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                              '\x01' << (uVar5 & 7)) != 0));
      uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)(bVar4 + 1) >> 8),-(0x7ff < uVar5));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar7 ^ uVar6) & 1) != 0) {
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
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
