/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04cf92.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_4cf92(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  undefined2 uVar5;
  short sVar6;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  short sVar8;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;

  *(undefined1 *)(unaff_A6 + 0x29) = *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0xa4);
  bVar9 = false;
  if (*(char *)(unaff_A6 + 0x29) == '\0') {
    candidate_4dd86();
    if (bVar9) {
      uVar5 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar5;
      uVar5 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar5;
      sVar6 = candidate_260c8();
    }
    else {
      uVar5 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar5;
      uVar5 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar5;
      sVar6 = candidate_260c8();
    }
  }
  else {
    candidate_4dd86();
    if (bVar9) {
      uVar5 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar5;
      uVar5 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar5;
      sVar6 = candidate_260c8();
    }
    else {
      uVar5 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar5;
      uVar5 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar5;
      sVar6 = candidate_260c8();
    }
  }
  sVar8 = 0;
  do {
    if (*(short *)((int)unaff_A6 + 0x82) <= *(short *)((int)unaff_A6 + 0x86)) break;
    bVar9 = false;
    candidate_40bc();
    if (bVar9) {
      *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6[3] + 0x98);
      candidate_1acf2();
      *(short *)(extraout_A0 + 0x86) = sVar8;
      *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + 1;
    }
    sVar8 = sVar8 + 1;
    sVar6 = sVar6 + -1;
  } while (-1 < sVar6);
  *unaff_A6 = 0x4d0e0;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0x9e) = 0;
    *unaff_A6 = candidate_4cf56;
  }
  else {
    *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
  }
  if (*(short *)((int)unaff_A6 + 0x86) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x9f) = 0;
  }
  bVar9 = false;
  candidate_4d74a();
  if (!bVar9) {
    return;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                      0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4) & 1) != 0) {
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
