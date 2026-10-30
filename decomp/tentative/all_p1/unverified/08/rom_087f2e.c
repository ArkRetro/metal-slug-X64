/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087f2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_87f2e(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  short sVar6;
  ushort uVar7;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar8;

  candidate_1b1dc();
  candidate_1436a();
  candidate_1b284();
  if ((bool)in_CF) {
    candidate_1b1dc();
    *(undefined1 *)((int)unaff_A6 + 0xa9) = 0xff;
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
    *unaff_A6 = 0x87f9a;
    uVar8 = 0;
    candidate_892fe();
    candidate_1b284();
    if (!(bool)uVar8) {
      *unaff_A6 = candidate_87f2e;
    }
    candidate_892bc();
    iVar2 = unaff_A6[0x28];
    uVar8 = 0;
    if (iVar2 != 0) {
      uVar8 = 0;
      func_0x000140c4();
    }
    candidate_89152();
    candidate_1945c();
    if (!(bool)uVar8) {
      return;
    }
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar4 = ~uVar7 & 0xfff;
      uVar7 = (ushort)CONCAT31((int3)((uint)iVar2 >> 8),
                               -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar4 & 7)) != 0));
      uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar4));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar7 ^ uVar5) & 1) != 0) {
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
    goto candidate_4188;
  }
  uVar8 = 0;
  candidate_4036c();
  candidate_8919c();
  candidate_89132();
  if ((bool)uVar8) {
    *(undefined1 *)((int)unaff_A6 + 0xab) = 0;
  }
  else {
    candidate_120f8();
    *(undefined1 *)(unaff_A6 + 0x2a) = 0;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar8 = 0;
    candidate_146a8();
    candidate_8919c();
    candidate_89132();
    if ((!(bool)uVar8) || (*(short *)(unaff_A6 + 0x1f) != *(short *)((int)unaff_A6 + 0x7a))) {
      candidate_89218();
      if (*(char *)((int)unaff_A6 + 0xa9) == '\0') {
        sVar6 = candidate_260c8();
      }
      else {
        sVar6 = candidate_260c8();
      }
      if ((short)(*(short *)((int)unaff_A6 + 0x92) + *(short *)((int)unaff_A6 + 0x7a)) <
          *(short *)(unaff_A6 + 0xf)) {
        sVar6 = -sVar6;
      }
      *(int *)((int)unaff_A6 + 0x62) = (int)sVar6 << 8;
      unaff_A6[0x27] = (int)sVar6 << 8;
      sVar6 = (short)(char)((ushort)sVar6 >> 8);
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        sVar6 = -sVar6;
      }
      if (sVar6 < 0) {
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
      }
      else {
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
      }
      *unaff_A6 = 0x88120;
      *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0x27];
      *(char *)(unaff_A6 + 0x2a) = *(char *)(unaff_A6 + 0x2a) + '\x01';
      bVar3 = *(byte *)(unaff_A6 + 0x2a) & 7;
      *(byte *)(unaff_A6 + 0x2a) = bVar3;
      if (bVar3 == 0) {
        candidate_89904();
      }
      candidate_892fe();
      sVar6 = *(short *)((int)unaff_A6 + 0x7a) + *(short *)((int)unaff_A6 + 0x92);
      if (*(int *)((int)unaff_A6 + 0x62) < 0) {
        if (*(short *)(unaff_A6 + 0xf) <= sVar6) {
LAB_00088160:
          *unaff_A6 = candidate_881e6;
        }
      }
      else if (sVar6 <= *(short *)(unaff_A6 + 0xf)) goto LAB_00088160;
      if (*(int *)((int)unaff_A6 + 0x62) < 0) {
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          candidate_49e8();
        }
      }
      else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        candidate_49e8();
      }
      candidate_892bc();
      uVar8 = 0;
      if (unaff_A6[0x28] != 0) {
        uVar8 = 0;
        func_0x000140c4();
      }
      candidate_89152();
      candidate_1945c();
      if (!(bool)uVar8) {
        return;
      }
      goto candidate_4188;
    }
    *(undefined1 *)((int)unaff_A6 + 0xab) = 1;
    *(undefined2 *)(unaff_A6 + 0x21) = 0x1e;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0xa9) = 0xff;
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  *unaff_A6 = 0x8833a;
  candidate_892fe();
  if (*(char *)((int)unaff_A6 + 0xab) == '\0') {
    uVar7 = *(ushort *)(unaff_A6 + 0xf);
    if (((short)uVar7 < 0x30) || (uVar8 = uVar7 < 0x120, 0x120 < (short)uVar7)) goto LAB_00088372;
  }
  else {
    if (*(char *)((int)unaff_A6 + 0xab) != '\x01') {
LAB_00088372:
      candidate_892bc();
      if (unaff_A6[0x28] != 0) {
        candidate_140c0();
      }
      candidate_89152();
      return;
    }
    sVar6 = *(short *)(unaff_A6 + 0x21);
    uVar8 = sVar6 == 0;
    sVar6 = sVar6 + -1;
    *(short *)(unaff_A6 + 0x21) = sVar6;
    if (sVar6 != 0) goto LAB_00088372;
  }
  candidate_8919c();
  candidate_893d0();
  if ((bool)uVar8) {
    *unaff_A6 = candidate_88ad2;
  }
  else {
    *unaff_A6 = 0x8806e;
  }
  candidate_892bc();
  uVar8 = 0;
  if (unaff_A6[0x28] != 0) {
    uVar8 = 0;
    func_0x000140c4();
  }
  candidate_89152();
  candidate_1945c();
  if (!(bool)uVar8) {
    return;
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
