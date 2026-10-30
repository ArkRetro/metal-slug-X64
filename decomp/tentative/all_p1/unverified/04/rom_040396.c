/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x040396.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_40396(void)

{
  int *piVar1;
  code *pcVar2;
  int iVar3;
  undefined2 uVar4;
  char cVar5;
  ushort uVar6;
  ushort uVar7;
  int iVar8;
  int iVar9;
  undefined1 uVar10;
  undefined2 unaff_D2w;
  ushort uVar11;
  undefined1 uVar12;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar13;

  *(undefined1 *)((int)unaff_A6 + 0x71) = 0xff;
  candidate_19dfc();
  cVar5 = *(char *)(unaff_A5 + 0x4446) + '\x01';
  *(char *)(unaff_A5 + 0x4446) = cVar5;
  *(char *)(unaff_A6 + 0x1c) = cVar5;
  *unaff_A6 = 0x403c0;
  candidate_1abc8();
  bVar13 = false;
  uVar10 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar12 = (undefined1)((ushort)unaff_D3w >> 8);
  if (*(char *)((int)unaff_A6 + 0x71) == '\0') {
    if ((ushort)(*(short *)(unaff_A6 + 0xf) + 0x30U) < 0x1a0) {
LAB_0004051c:
      iVar8 = unaff_A5 + 0x43f4;
      if ((*(byte *)(unaff_A5 + -0x7ff1) & 1) != 0) {
        iVar8 = unaff_A5 + 0x43a2;
      }
      uVar4 = *(undefined2 *)((int)unaff_A6 + 0x6e);
      uVar10 = *(undefined1 *)(unaff_A6 + 0x1c);
      uVar11 = *(ushort *)(iVar8 + 0x50);
      if (uVar11 < 0x50) {
        piVar1 = (int *)(iVar8 + (short)uVar11);
        *(ushort *)(iVar8 + 0x50) = uVar11 + 10;
        iVar8 = unaff_A6[10];
        iVar9 = unaff_A6[0xb];
        iVar3 = unaff_A6[0xe];
        if (iVar3 != 0) {
          iVar8 = *(int *)(iVar3 + 0x14) + iVar8;
          iVar9 = *(int *)(iVar3 + 0x18) + iVar9;
        }
        iVar3 = *(int *)(unaff_A5 + 0x6980);
        *piVar1 = iVar8 - *(int *)(unaff_A5 + 0x697c);
        piVar1[1] = iVar9 - iVar3;
        *(char *)(piVar1 + 2) = (char)uVar4;
        *(undefined1 *)((int)piVar1 + 9) = uVar10;
      }
      return;
    }
    uVar11 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar11 + 0x3ef;
    if (uVar11 < 0xfc11) goto candidate_4188;
    uVar11 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar7 = uRam002fffe8;
    uVar6 = ~uVar11 & 0xfff;
    uVar11 = CONCAT11(uVar10,-((*(byte *)((short)(uVar11 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar6 & 7)) != 0));
    uRam002fffe0 = uVar11 & CONCAT11(uVar12,-(0x7ff < uVar6));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar11 = (ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar11 ^ uVar7);
  }
  else {
    candidate_409a();
    if (!bVar13) goto candidate_4188;
    if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) goto LAB_0004051c;
    uVar11 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar11 + 0x3ef;
    if (uVar11 < 0xfc11) goto candidate_4188;
    uVar11 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar7 = uRam002fffe8;
    uVar6 = ~uVar11 & 0xfff;
    uVar11 = CONCAT11(uVar10,-((*(byte *)((short)(uVar11 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar6 & 7)) != 0));
    uRam002fffe0 = uVar11 & CONCAT11(uVar12,-(0x7ff < uVar6));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar11 = (ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar11 ^ uVar7);
  }
  if ((uVar11 & 1) != 0) {
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
  iVar8 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar2 = (code *)*unaff_A6;
    *(undefined4 *)(iVar8 + -4) = 0x424a;
    (*pcVar2)();
  } while( true );
}
