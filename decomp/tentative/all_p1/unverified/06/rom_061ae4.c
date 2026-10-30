/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x061ae4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_61ae4(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  short sVar4;
  ushort uVar5;
  ushort uVar6;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;
  bool bVar9;

  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
  candidate_4256();
  candidate_1b1dc();
  *unaff_A6 = 0x61b06;
  uVar8 = 0;
  candidate_623fe();
  candidate_427a();
  if ((bool)uVar8) {
    *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)(unaff_A6 + 0x22);
    *unaff_A6 = 0x6173c;
    uVar8 = 0;
  }
  candidate_625cc();
  if ((bool)uVar8) {
    *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)(unaff_A6 + 0x22);
    *unaff_A6 = 0x6173c;
  }
  bVar9 = false;
  if (0 < *(short *)(unaff_A6 + 0x21)) {
    sVar4 = *(short *)(unaff_A6 + 0x21);
    bVar9 = sVar4 == 0;
    *(short *)(unaff_A6 + 0x21) = sVar4 + -1;
  }
  candidate_1945c();
  if (!bVar9) {
    bVar9 = false;
    if ('\0' < *(char *)((int)unaff_A6 + 0x9d)) {
      cVar3 = *(char *)((int)unaff_A6 + 0x9d);
      bVar9 = cVar3 == '\0';
      *(char *)((int)unaff_A6 + 0x9d) = cVar3 + -1;
    }
    candidate_4c24();
    if (bVar9) {
      candidate_625ec();
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      cVar3 = *(char *)((int)unaff_A6 + 0x5a);
      *unaff_A6 = candidate_61c6e;
      if (cVar3 == '\n') {
        *unaff_A6 = candidate_61e16;
      }
      if (cVar3 == '\v') {
        *unaff_A6 = candidate_61e2e;
      }
      if (cVar3 == '\a') {
        *unaff_A6 = candidate_61d46;
      }
      candidate_4e76();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    return;
  }
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar5 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)) !=
                      0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar7 ^ uVar6) & 1) != 0) {
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
