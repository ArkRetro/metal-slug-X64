/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0de702.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_de702(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  byte bVar6;
  uint uVar5;
  char cVar7;
  uint uVar8;
  undefined1 uVar9;
  undefined2 unaff_D2w;
  ushort uVar10;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar11;

  uVar9 = (undefined1)((ushort)unaff_D2w >> 8);
  unaff_A6[0xd] = -(*(int *)((int)unaff_A6 + 0x76) - (*(int *)((int)unaff_A6 + 0x76) >> 3));
  if (*(char *)((int)unaff_A6 + 0x7a) != '@') {
    bVar6 = candidate_2eb4();
    uVar9 = (undefined1)((ushort)*(undefined2 *)((short)((ushort)bVar6 * 2) + 0x2a7c) >> 8);
    candidate_ded6a();
    uVar5 = candidate_deda6();
    uVar8 = (uint)(short)-((int)uVar5 < 0);
    uVar5 = (uVar8 ^ uVar5) - uVar8;
    if (0x30000 < uVar5) {
      uVar5 = 0x30000;
    }
    unaff_A6[0xc] = (uVar8 ^ uVar5) - uVar8;
  }
  candidate_1abc8();
  candidate_427a();
  *unaff_A6 = candidate_de5b4;
  cVar7 = candidate_49e8();
  if (cVar7 != '\0') {
    *unaff_A6 = candidate_de7f2;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_de7f2;
  }
  bVar11 = false;
  candidate_1945c();
  if (!bVar11) {
    candidate_505a();
    return;
  }
  candidate_c0dfc();
  uVar10 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar10 + 0x3ef;
  if (0xfc10 < uVar10) {
    uVar10 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar10 & 0xfff;
    uVar10 = CONCAT11(uVar9,-((*(byte *)((short)(uVar10 >> 3 & 0xfff) + 0xdedd2) &
                              '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar10 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar10 ^ uVar4) & 1) != 0) {
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
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
