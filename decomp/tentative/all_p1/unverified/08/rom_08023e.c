/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08023e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8023e(void)

{
  code *pcVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  short sVar5;
  undefined2 uVar6;
  ushort uVar7;
  undefined1 uVar8;
  int iVar9;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int extraout_A0_05;
  int extraout_A0_06;
  int extraout_A0_07;
  int extraout_A0_08;
  int extraout_A0_09;
  int iVar10;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar11;

  candidate_4256();
  sVar5 = candidate_260c8();
  *(int *)((int)unaff_A6 + 0x6e) = (int)sVar5 << 8;
  uVar6 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar6;
  uVar6 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x22) = uVar6;
  candidate_80a08();
  uVar8 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar8 = 0x80;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = uVar8;
  uVar7 = candidate_29f2();
  *(ushort *)(unaff_A6 + 0x21) = uVar7 & 7;
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  uVar8 = candidate_366ac();
  *(undefined1 *)(unaff_A6 + 0x23) = uVar8;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  candidate_120f8();
  *unaff_A6 = 0x802ce;
  candidate_80942();
  candidate_1ac38();
  candidate_8086a();
  candidate_427a();
  if ((0 < *(short *)(unaff_A6 + 0x22)) &&
     (sVar5 = *(short *)(unaff_A6 + 0x22), sVar3 = sVar5 + -1, *(short *)(unaff_A6 + 0x22) = sVar3,
     sVar3 == 0 || sVar5 < 1)) {
    candidate_80a3c();
    candidate_80a08();
  }
  sVar5 = *(short *)((int)unaff_A6 + 0x86);
  uVar8 = sVar5 == 0;
  sVar5 = sVar5 + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar5;
  if (sVar5 == 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  else {
    uVar8 = *(ushort *)((int)unaff_A6 + 0x86) < 0xffe7;
    if ((short)*(ushort *)((int)unaff_A6 + 0x86) < -0x18) goto LAB_00080546;
  }
  candidate_1945c();
  if (!(bool)uVar8) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
      sVar5 = candidate_49e8();
      uVar8 = 0;
      if (sVar5 != 0) {
        candidate_9ea8();
        candidate_4256();
        candidate_120f8();
        *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
        *unaff_A6 = 0x80390;
        uVar8 = 0;
        candidate_1abc8();
        candidate_427a();
        if ((bool)uVar8) {
          *unaff_A6 = 0x80546;
        }
        return;
      }
    }
    candidate_4c24();
    if ((bool)uVar8) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      return;
    }
    bVar11 = false;
    candidate_40bc();
    iVar10 = extraout_A0;
    if (bVar11) {
      candidate_1b220();
      bVar11 = true;
      iVar10 = extraout_A0_00;
    }
    if (bVar11) {
      iVar9 = unaff_A6[0xd];
      *(int *)(iVar10 + 0x30) = (int)unaff_A6[0xc] >> 3;
      *(int *)(iVar10 + 0x34) = iVar9 >> 3;
    }
    uVar7 = candidate_29c6();
    *(short *)(extraout_A0_01 + 0x28) =
         (((short)uVar7 >> 8 & 0xfU) - 7) + *(short *)(extraout_A0_01 + 0x28);
    bVar11 = false;
    candidate_40bc();
    iVar10 = extraout_A0_02;
    if (bVar11) {
      candidate_1b220();
      bVar11 = true;
      iVar10 = extraout_A0_03;
    }
    if (bVar11) {
      iVar9 = unaff_A6[0xd];
      *(int *)(iVar10 + 0x30) = (int)unaff_A6[0xc] >> 3;
      *(int *)(iVar10 + 0x34) = iVar9 >> 3;
    }
    *(short *)(iVar10 + 0x28) = ((uVar7 & 0xf) - 7) + *(short *)(iVar10 + 0x28);
    *(byte *)(iVar10 + 0x15) = *(byte *)(iVar10 + 0x15) ^ 1;
    bVar11 = false;
    candidate_40bc();
    iVar10 = extraout_A0_04;
    if (bVar11) {
      candidate_1b220();
      bVar11 = true;
      iVar10 = extraout_A0_05;
    }
    if (bVar11) {
      iVar9 = unaff_A6[0xd];
      *(int *)(iVar10 + 0x30) = (int)unaff_A6[0xc] >> 3;
      *(int *)(iVar10 + 0x34) = iVar9 >> 3;
    }
    bVar11 = false;
    candidate_40bc();
    iVar10 = extraout_A0_06;
    if (bVar11) {
      candidate_1b220();
      bVar11 = true;
      iVar10 = extraout_A0_07;
    }
    if (bVar11) {
      iVar9 = unaff_A6[0xd];
      *(int *)(iVar10 + 0x30) = (int)unaff_A6[0xc] >> 3;
      *(int *)(iVar10 + 0x34) = iVar9 >> 3;
    }
    *(byte *)(iVar10 + 0x15) = *(byte *)(iVar10 + 0x15) ^ 1;
    bVar11 = false;
    candidate_40bc();
    iVar10 = extraout_A0_08;
    if (bVar11) {
      candidate_1b220();
      bVar11 = true;
      iVar10 = extraout_A0_09;
    }
    if (bVar11) {
      iVar9 = unaff_A6[0xd];
      *(int *)(iVar10 + 0x30) = (int)unaff_A6[0xc] >> 3;
      *(int *)(iVar10 + 0x34) = iVar9 >> 3;
    }
    candidate_8083c();
    candidate_9ea8();
    candidate_4256();
    candidate_120f8();
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    *unaff_A6 = 0x8052e;
    uVar8 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar8) {
      *unaff_A6 = 0x80546;
    }
    return;
  }
LAB_00080546:
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar2 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar7 ^ uVar4) & 1) != 0) {
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
  iVar10 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar10 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
