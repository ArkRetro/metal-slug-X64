/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0afa4c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_afa4c(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar7;
  ushort uVar5;
  short sVar6;
  int iVar4;
  byte bVar8;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  short extraout_D1w_01;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  uint unaff_D6;
  int extraout_A0;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;
  undefined1 uVar10;

  candidate_4256();
  unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x62);
  unaff_A6[0xd] = 0x8a000;
  candidate_29c6();
  sVar6 = candidate_29c6();
  iVar4 = (short)(extraout_D1w_01 + sVar6) + 0x28000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar4 = -iVar4;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar4;
  unaff_A6[0xc] = iVar4;
  *unaff_A6 = 0xafa9c;
  candidate_aedd6();
  candidate_427a();
  uVar10 = false;
  if (*(short *)(unaff_A6 + 0xd) < 0) {
    bVar8 = candidate_49e8();
    uVar10 = bVar8 < 5;
    if (bVar8 == 5) {
      *unaff_A6 = candidate_afada;
      uVar10 = false;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_afb88;
    uVar10 = false;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_406b6();
  }
  candidate_ae9c0();
  candidate_1945c();
  if ((bool)uVar10) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
    if (0xfc10 < uVar5) {
      uVar5 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar5 & 0xfff;
      uVar5 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7))
                        != 0));
      uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar5 ^ uVar3) & 1) != 0) {
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
    iVar4 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar4 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  cVar7 = candidate_1ae8e();
  if (cVar7 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  else {
    if (cVar7 == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x20) = 0;
      iVar4 = unaff_A5 + -0x6b3c;
    }
    else if (cVar7 == '\x02') {
      *(undefined1 *)(unaff_A6 + 0x20) = 1;
      iVar4 = unaff_A5 + -0x6a8c;
    }
    else {
      uVar5 = candidate_29f2();
      iVar4 = extraout_A0;
      if (extraout_D1w < uVar5) {
        iVar4 = extraout_A1;
      }
      *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar5;
    }
    if ((short)((ushort)(byte)(-(-1 < (short)(*(short *)(iVar4 + 0x3c) - *(short *)(unaff_A6 + 0xf))
                                ) ^ -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfe;
    }
    else {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 1;
    }
  }
  if (*(short *)(unaff_A6 + 0x1c) != 0) {
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + -1;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfd;
  candidate_260c8();
  uVar5 = candidate_29c6();
  if (((uVar5 & 0xff) < extraout_D1w_00) && (uVar5 >> 8 < extraout_D1w_00)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 2;
    sVar6 = 0x30;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar6 = -0x30;
    }
    candidate_510c(CONCAT22(*(undefined2 *)(unaff_A6 + 0x10),*(short *)(unaff_A6 + 0xf) + sVar6));
    candidate_aeb40();
  }
  sVar6 = *(short *)((int)unaff_A6 + 0x6e);
  if (sVar6 != 0) {
    bVar9 = (short)(sVar6 + -1) < 0;
    if (sVar6 == 1) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
    }
    else {
      candidate_29c6();
      if (bVar9) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      }
      else {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
      }
    }
  }
  sVar6 = *(short *)(unaff_A6 + 0x1d);
  if (sVar6 != 0) {
    sVar6 = sVar6 + -1;
    *(short *)(unaff_A6 + 0x1d) = sVar6;
  }
  bVar9 = sVar6 < 0;
  candidate_29c6();
  if (bVar9) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) ^ 0x80;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
  if (0xff < (ushort)(*(short *)(unaff_A6 + 0xf) - 0x20U)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xf7;
  }
  if (*(short *)((int)unaff_A6 + 0x92) != 0) {
    sVar6 = *(short *)((int)unaff_A6 + 0x92) >> 6;
    if (sVar6 == 0) {
      sVar6 = 1;
    }
    *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) - sVar6;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xdf;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xef;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xbf;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfb;
  candidate_148c2();
  if ((unaff_D6 & 0x10) == 0) {
    if ((unaff_D6 & 0x100) != 0) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x10;
    }
    if ((unaff_D6 & 4) != 0) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x20;
    }
  }
  else {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x40;
  }
  candidate_14850();
  if ((unaff_D6 & 4) != 0) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 4;
  }
  return;
}
