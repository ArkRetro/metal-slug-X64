/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06268a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_6268a(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  ushort uVar7;
  short sVar8;
  int extraout_D1;
  undefined4 unaff_D2;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar9;
  bool bVar10;
  undefined8 uVar11;
  undefined2 in_stack_fffffffa;
  undefined4 uVar12;
  undefined2 uVar13;

  candidate_1af9a();
  if ((bool)in_CF) {
    candidate_1abc8();
    return;
  }
  unaff_A6[10] = (*(int *)(extraout_A0 + 0x3c) - unaff_A6[0xf]) +
                 (uint)*(ushort *)((int)unaff_A6 + 0x86) * -0x10000 + unaff_A6[10];
  candidate_9ea8();
  uVar9 = 0;
  candidate_4520();
  candidate_1b1dc();
  candidate_1abc8();
  candidate_1af9a();
  if ((bool)uVar9) {
    if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    else {
      *(int *)((int)unaff_A6 + 0x6e) = -*(int *)((int)unaff_A6 + 0x6e);
    }
  }
  else if ((int)unaff_A6[0xf] < *(int *)(extraout_A0_00 + 0x3c)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  else {
    *(int *)((int)unaff_A6 + 0x6e) = -*(int *)((int)unaff_A6 + 0x6e);
  }
  uVar7 = candidate_29c6();
  *(short *)((int)unaff_A6 + 0x76) = ((uVar7 & 7) - 3) + *(short *)((int)unaff_A6 + 0x76);
  if (*(char *)((int)unaff_A6 + 0x92) == '\x02') {
    candidate_4256();
    unaff_D2 = 0xffffb000;
    unaff_D3w = *(undefined2 *)((int)unaff_A6 + 0x82);
    uVar11 = candidate_3b332();
    *(undefined8 *)(unaff_A6 + 0xc) = uVar11;
    *(int *)((int)unaff_A6 + 0x76) = -*(int *)((int)unaff_A6 + 0x76);
    *(undefined4 *)((int)unaff_A6 + 0x7e) = 0xfffd8000;
    *(undefined1 *)(unaff_A6 + 0x24) = 0xc0;
    bVar10 = false;
    candidate_40bc();
    if (bVar10) {
      candidate_1b220();
    }
    *unaff_A6 = candidate_62886;
    if (*(int *)((int)unaff_A6 + 0x7e) + unaff_A6[0xd] < 0) {
      candidate_4256();
      *unaff_A6 = 0x628a8;
    }
    if ((int)unaff_A6[0xd] < 0) {
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
    }
    iVar2 = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
    if ((iVar2 < 0) && (-1 < *(int *)((int)unaff_A6 + 0x76))) {
      unaff_A6[0xd] = unaff_A6[0xd] - iVar2;
      *unaff_A6 = candidate_62902;
    }
    *(int *)((int)unaff_A6 + 0x76) = iVar2;
    candidate_1ac38();
    candidate_427a();
    candidate_49e8();
    uVar3 = unaff_A6[0xd];
    bVar10 = 0x4fff < uVar3;
    unaff_A6[0xd] = uVar3 - 0x5000;
  }
  else {
    sVar8 = candidate_260c8();
    *(int *)((int)unaff_A6 + 0x7a) = (int)sVar8 * (int)sVar8;
    candidate_3c5d8();
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0x24) = 0x40;
    *unaff_A6 = 0x62778;
    *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + '\n';
    candidate_2e7c();
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + extraout_D1;
    candidate_1ac38();
    candidate_427a();
    uVar5 = *(int *)((int)unaff_A6 + 0x76) - unaff_A6[0xd];
    *(uint *)((int)unaff_A6 + 0x76) = uVar5;
    uVar3 = *(uint *)((int)unaff_A6 + 0x7a);
    if ((int)uVar3 >= (int)uVar5) {
      *unaff_A6 = candidate_627c2;
    }
    bVar10 = (int)uVar3 < (int)uVar5 && uVar5 < uVar3;
  }
  candidate_1945c();
  if (!bVar10) {
    uVar13 = 0x2a02;
    candidate_4c24();
    if (bVar10) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar12 = CONCAT22((short)unaff_D2,in_stack_fffffffa);
      candidate_40bc(uVar12,CONCAT22(unaff_D3w,uVar13));
      *(short *)(extraout_A0_01 + 0x28) = (short)((uint)uVar12 >> 0x10);
      *(short *)(extraout_A0_01 + 0x2a) = (short)uVar12;
      candidate_1ac94();
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_62a40;
    }
    return;
  }
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar4 = ~uVar7 & 0xfff;
    uVar7 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                             -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar4 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar7 ^ uVar6) & 1) != 0) {
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
