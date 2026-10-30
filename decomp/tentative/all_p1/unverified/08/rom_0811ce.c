/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0811ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_811ce(void)

{
  code *pcVar1;
  uint uVar2;
  short sVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  int iVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;
  bool bVar10;

  candidate_81ea4();
  iVar7 = *(int *)((int)unaff_A6 + 0x6e);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    iVar7 = -iVar7;
  }
  uVar5 = iVar7 - unaff_A6[0xc] >> 3;
  uVar2 = unaff_A6[0xc];
  uVar9 = CARRY4(uVar5,uVar2);
  unaff_A6[0xc] = uVar5 + uVar2;
  candidate_1ac38();
  candidate_427a();
  candidate_81e08();
  if ((bool)uVar9) {
    if (*(short *)(unaff_A6 + 0x25) < 1) {
      if (*(short *)(unaff_A6 + 0xf) < 0x20) {
        *unaff_A6 = candidate_81236;
      }
      if (0x120 < *(short *)(unaff_A6 + 0xf)) {
        *unaff_A6 = candidate_81236;
      }
    }
  }
  else {
    *unaff_A6 = candidate_811a2;
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
  bVar10 = false;
  if (0 < *(short *)(unaff_A6 + 0x25)) {
    sVar3 = *(short *)(unaff_A6 + 0x25);
    bVar10 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x25) = sVar3 + -1;
  }
  candidate_1945c();
  if (!bVar10) {
    return;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar4 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7)) !=
                      0));
    uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar8 ^ uVar6) & 1) != 0) {
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
  iVar7 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar7 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
