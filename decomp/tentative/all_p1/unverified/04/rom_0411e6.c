/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0411e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_411e6(void)

{
  code *pcVar1;
  short sVar2;
  ushort uVar3;
  uint uVar4;
  ushort uVar5;
  int iVar6;
  uint uVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;

  if (*(short *)((int)unaff_A6 + 0x76) == 2) {
    *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0x10000;
  }
  else if (*(short *)((int)unaff_A6 + 0x76) == 3) {
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0x10000;
  }
  else {
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0x30000;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
  }
  sVar2 = *(short *)((int)unaff_A6 + 0x7a);
  if (sVar2 != 0) {
    iVar6 = sVar2 * 0x100;
    if (iVar6 < 0) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
      iVar6 = sVar2 * -0x100;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      iVar6 = -iVar6;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar6;
  }
  *unaff_A6 = 0x4128c;
  candidate_41144();
  uVar7 = *(int *)(unaff_A5 + 0x6984) +
          ((unaff_A6[0xf] - *(int *)(unaff_A5 + 0x697c)) - unaff_A6[0x11]);
  if ((int)uVar7 < 0) {
    uVar7 = -uVar7;
  }
  uVar4 = CONCAT22(*(undefined2 *)((int)unaff_A6 + 0x6e),*(undefined2 *)((int)unaff_A6 + 0x7e));
  iVar6 = uVar4 - uVar7;
  if (uVar4 < uVar7) {
    *unaff_A6 = candidate_412e0;
  }
  *(short *)((int)unaff_A6 + 0x7e) = (short)iVar6;
  *(short *)((int)unaff_A6 + 0x6e) = (short)((uint)iVar6 >> 0x10);
  uVar9 = 0;
  candidate_427a();
  func_0x000aedb8();
  if ((bool)uVar9) {
    *unaff_A6 = candidate_4137e;
    uVar9 = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    uVar9 = 0;
    if (*(char *)(unaff_A6 + 0x1f) == '\0') {
      candidate_406b6();
    }
    else {
      candidate_407ca();
    }
  }
  candidate_1945c();
  if ((bool)uVar9) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar3 = ~uVar8 & 0xfff;
      uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7))
                        != 0));
      uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar5) & 1) != 0) {
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
    iVar6 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar6 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
