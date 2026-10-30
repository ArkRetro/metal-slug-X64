/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c2d6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c2d6a(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  short sVar4;
  ushort uVar5;
  ushort uVar6;
  char cVar7;
  int extraout_D1;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  short unaff_D7w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;
  bool bVar10;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ec;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ec;
  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0x7f;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_120f8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  bVar10 = *(ushort *)(unaff_A6 + 0xf) < 0xa0;
  *(char *)((int)unaff_A6 + 0x5b) = -(-1 < (short)(*(ushort *)(unaff_A6 + 0xf) - 0xa0));
  candidate_3c55c();
  if ((bVar10) && ((cVar7 = candidate_191fc(), cVar7 == '\x02' || (cVar7 == '\x03')))) {
    candidate_9f88();
    candidate_9ea8();
    candidate_c0f32();
    candidate_c0f16();
    unaff_A6[0xd] = 0;
    *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
    *unaff_A6 = candidate_c2d06;
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
    if (*(short *)(unaff_A6 + 0x20) == 0) {
LAB_000c2d40:
      unaff_A6[0xd] = (-0x100000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
    }
    else {
      *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
      bVar10 = false;
      candidate_3c55c();
      if (!bVar10) goto LAB_000c2d40;
      unaff_A6[0xd] = (*(int *)(extraout_A0 + 0x18) + extraout_D1 + -0x80000) - unaff_A6[0x10] >> 3;
    }
    candidate_427a();
    func_0x000143e4();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) goto LAB_000c2f90;
  }
  else if (*(char *)((int)unaff_A6 + 0x93) == '\x02') {
    unaff_A6[0xd] = 0xa9000;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      unaff_A6[0xc] = 0xfffe0000;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      unaff_A6[0xc] = 0x20000;
    }
    candidate_c0fb6();
    candidate_c0f16();
    *unaff_A6 = 0xc2a48;
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
    uVar3 = unaff_A6[0xd];
    uVar9 = uVar3 < (uint)((int)unaff_A6[0xd] >> 5);
    unaff_A6[0xd] = uVar3 - ((int)unaff_A6[0xd] >> 5);
    candidate_143ee();
    candidate_427a();
    if ((bool)uVar9) goto LAB_000c2f90;
  }
  else {
    if (*(char *)((int)unaff_A6 + 0x93) == '\x03') {
      candidate_9f88();
      candidate_9ea8();
      candidate_9ea8();
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
      candidate_c0f32();
      candidate_c0f16();
      *(undefined2 *)(unaff_A6 + 0x20) = 0x80;
    }
    else {
      candidate_9f88();
      if ((&DAT_0010c30c)[*(short *)(unaff_A6 + 0x1c)] == '\0') {
        candidate_9ea8();
      }
      else {
        candidate_9ea8();
      }
      candidate_c0f32();
      candidate_c0f16();
      *(undefined2 *)(unaff_A6 + 0x20) = 0x20;
    }
    unaff_A6[0xd] = 0xa9000;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      unaff_A6[0xc] = 0xfffe0000;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      unaff_A6[0xc] = 0x20000;
    }
    candidate_c4c28();
    *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = 0xc2e8a;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
      unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
      candidate_143ee();
    }
    else {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      candidate_14594();
    }
    uVar5 = (short)(*(short *)(unaff_A6 + 0x20) - *(short *)((int)unaff_A6 + 0x26)) >> 4;
    uVar8 = *(ushort *)((int)unaff_A6 + 0x26);
    bVar10 = CARRY2(uVar5,uVar8);
    *(ushort *)((int)unaff_A6 + 0x26) = uVar5 + uVar8;
    candidate_427a();
    if (bVar10) {
      *unaff_A6 = candidate_c2f06;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      unaff_A6[0xc] = 0;
      *(undefined2 *)(unaff_A6 + 0x20) = 0x100;
      *(undefined1 *)(unaff_A6 + 0x1e) = 0;
    }
  }
  *(undefined1 *)(unaff_A5 + 0x430f) = 0xff;
  candidate_c8d4e();
  if (unaff_D7w == 0) {
    *(undefined1 *)((int)*(short *)(unaff_A6 + 0x1c) + unaff_A5 + 0x430a) = 0xff;
  }
  candidate_aa4c8();
  if ((*(ushort *)(unaff_A6 + 0xf) < 0x140) &&
     ((ushort)(*(short *)(unaff_A6 + 0x10) - 0xc0U) < 0x200)) {
    return;
  }
LAB_000c2f90:
  *unaff_A6 = 0xc2f96;
  cVar7 = candidate_19028();
  if (cVar7 == '\0') {
    return;
  }
  sVar4 = *(short *)(unaff_A6 + 0x1c);
  *(undefined1 *)(sVar4 + 0x10c718) = 0;
  *(undefined1 *)(sVar4 + 0x10c71e) = 0;
  *(undefined1 *)(sVar4 + 0x10c720) = 0;
  *(undefined2 *)((short)(sVar4 * 2) + 0x10c724) = 0;
  if (DAT_0010c2e7 == '\x01') {
    *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
    *unaff_A6 = 0xc2fe0;
    sVar4 = *(short *)(unaff_A6 + 0x20) + -1;
    *(short *)(unaff_A6 + 0x20) = sVar4;
    if (sVar4 != 0) {
      return;
    }
  }
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar5 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)) !=
                      0));
    uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar6) & 1) != 0) {
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
