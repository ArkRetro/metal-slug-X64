/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a422.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1a422(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  undefined4 uVar6;
  undefined3 uVar7;
  undefined4 uVar8;
  ushort uVar9;
  undefined3 uVar10;
  undefined4 uVar11;
  int extraout_A0;
  int extraout_A0_00;
  undefined4 uVar12;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar13;

  *(undefined2 *)((int)unaff_A6 + 0x72) = *(undefined2 *)(*(int *)((int)unaff_A6 + 0x6e) + 2);
  *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(*(int *)((int)unaff_A6 + 0x6e) + 8);
  *unaff_A6 = 0x1a438;
  bVar13 = false;
  candidate_409a();
  if (!bVar13) goto candidate_4188;
  uVar6 = *(undefined4 *)(extraout_A0 + 0x2c);
  uVar8 = *(undefined4 *)(extraout_A0 + 0x30);
  uVar11 = *(undefined4 *)(extraout_A0 + 0x34);
  uVar12 = *(undefined4 *)(extraout_A0 + 0x38);
  unaff_A6[10] = *(undefined4 *)(extraout_A0 + 0x28);
  unaff_A6[0xb] = uVar6;
  unaff_A6[0xc] = uVar8;
  unaff_A6[0xd] = uVar11;
  unaff_A6[0xe] = uVar12;
  bVar13 = *(ushort *)(*(int *)((int)unaff_A6 + 0x6e) + 6) < *(ushort *)(unaff_A5 + 0x699e);
  uVar7 = (undefined3)((uint)uVar8 >> 8);
  uVar10 = (undefined3)((uint)uVar11 >> 8);
  if (bVar13) {
    uVar9 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
    if (uVar9 < 0xfc11) goto candidate_4188;
    uVar9 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar9 & 0xfff;
    uVar9 = (ushort)CONCAT31(uVar7,-((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                     '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar9 & (ushort)CONCAT31(uVar10,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar9 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar9 ^ uVar5);
  }
  else {
    candidate_1a5ca();
    if (!bVar13) goto candidate_4188;
    if (*(short *)((int)unaff_A6 + 0x72) != 0) {
      *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
      return;
    }
    *(undefined2 *)((int)unaff_A6 + 0x72) = *(undefined2 *)(extraout_A0_00 + 4);
    *(undefined1 *)(unaff_A6 + 0x1d) = 0;
    candidate_1a0a6();
    if ((*(char *)(unaff_A6 + 0x1d) == '\0') ||
       (sVar4 = *(short *)((int)unaff_A6 + 0x76) + -1, *(short *)((int)unaff_A6 + 0x76) = sVar4,
       sVar4 != 0)) {
      return;
    }
    uVar9 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
    if (uVar9 < 0xfc11) goto candidate_4188;
    uVar9 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar9 & 0xfff;
    uVar9 = (ushort)CONCAT31(uVar7,-((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                     '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar9 & (ushort)CONCAT31(uVar10,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar9 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar9 ^ uVar5);
  }
  if ((uVar9 & 1) != 0) {
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
