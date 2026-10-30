/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd642.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_dd642(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  undefined2 uVar6;
  undefined1 uVar7;
  ushort uVar8;
  undefined2 unaff_D2w;
  undefined1 uVar9;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;
  undefined1 uVar11;

  candidate_120f8();
  candidate_9ea8();
  candidate_4256();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  sVar5 = candidate_260c8();
  *(int *)((int)unaff_A6 + 0x86) = sVar5 * -0x100;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x18;
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  uVar6 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar6;
  unaff_A6[0x14] = 0xdc92a;
  candidate_4c1c();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *unaff_A6 = 0xdd6be;
  sVar5 = candidate_49e8();
  bVar10 = false;
  uVar7 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
  if (sVar5 == 0) {
    candidate_4c24();
    if (bVar10) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) goto LAB_000dd744;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      candidate_9f88();
      candidate_120f8();
      candidate_9ea8();
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
      candidate_4256();
      *unaff_A6 = 0x9503c;
      uVar11 = 0;
      candidate_1abc8();
      candidate_427a();
      if (!(bool)uVar11) {
        return;
      }
      uVar8 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
      if (0xfc10 < uVar8) {
        uVar8 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar3 = ~uVar8 & 0xfff;
        uVar8 = CONCAT11(uVar7,-((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar3 & 7)) != 0));
        uRam002fffe0 = uVar8 & CONCAT11(uVar9,-(0x7ff < uVar3));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar4) & 1) == 0)
        goto candidate_4188;
        goto LAB_00003c64;
      }
      goto candidate_4188;
    }
    if (*(char *)(unaff_A6[3] + 0x61) == '\x01') goto LAB_000dd744;
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x86) + unaff_A6[0xd];
    *(int *)((int)unaff_A6 + 0x82) = (int)-unaff_A6[0xc] >> 4;
    uVar11 = 0;
    func_0x000143e4();
    candidate_427a();
    candidate_1945c();
    if (!(bool)uVar11) {
      return;
    }
  }
  else {
LAB_000dd744:
    candidate_120f8();
    bVar10 = false;
    candidate_40bc();
    if (bVar10) {
      candidate_1b220();
    }
  }
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11(uVar7,-((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar8 & CONCAT11(uVar9,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar4) & 1) != 0) {
LAB_00003c64:
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
