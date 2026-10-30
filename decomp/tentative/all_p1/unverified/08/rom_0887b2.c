/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0887b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_887b2(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  undefined4 unaff_D2;
  ushort uVar6;
  undefined1 uVar7;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;

  candidate_1b1dc();
  candidate_8917c();
  *(undefined2 *)(unaff_A6 + 0x25) = 0xf;
  *unaff_A6 = 0x887c8;
  candidate_1436a();
  candidate_427a();
  uVar7 = (undefined1)((ushort)unaff_D3w >> 8);
  if ((*(short *)(unaff_A6 + 0x1d) == 0) &&
     (sVar4 = *(short *)(unaff_A6 + 0x25) + -1, *(short *)(unaff_A6 + 0x25) = sVar4, sVar4 < 0)) {
    candidate_120f8();
    candidate_1da40();
    candidate_1dbaa();
    candidate_13f1e();
    candidate_411c();
    candidate_1acf2();
    uVar6 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
    if (uVar6 < 0xfc11) goto candidate_4188;
    uVar6 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar6 & 0xfff;
    uVar6 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                             -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar6 & CONCAT11(uVar7,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar6 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar6 ^ uVar5);
  }
  else {
    iVar2 = unaff_A6[0x28];
    uVar8 = 0;
    if (iVar2 != 0) {
      uVar8 = 0;
      candidate_140c0();
    }
    candidate_1945c();
    if (!(bool)uVar8) {
      return;
    }
    uVar6 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
    if (uVar6 < 0xfc11) goto candidate_4188;
    uVar6 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar6 & 0xfff;
    uVar6 = (ushort)CONCAT31((int3)((uint)iVar2 >> 8),
                             -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar6 & CONCAT11(uVar7,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar6 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar6 ^ uVar5);
  }
  if ((uVar6 & 1) != 0) {
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
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
