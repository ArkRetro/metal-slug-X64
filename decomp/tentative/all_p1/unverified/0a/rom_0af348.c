/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0af348.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_af348(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar6;
  ushort uVar4;
  short sVar5;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  uint unaff_D6;
  int extraout_A0;
  int iVar7;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;
  undefined1 uVar9;

  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0xaf366;
  uVar9 = 0;
  candidate_1456a();
  candidate_427a();
  if ((bool)uVar9) {
    *unaff_A6 = candidate_af380;
    uVar9 = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_406b6();
  }
  candidate_ae9c0();
  candidate_1945c();
  if ((bool)uVar9) {
    uVar4 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar4 + 0x3ef;
    if (0xfc10 < uVar4) {
      uVar4 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar4 & 0xfff;
      uVar4 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar4 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7))
                        != 0));
      uRam002fffe0 = uVar4 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar4 ^ uVar3) & 1) != 0) {
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
  cVar6 = candidate_1ae8e();
  if (cVar6 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  else {
    if (cVar6 == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x20) = 0;
      iVar7 = unaff_A5 + -0x6b3c;
    }
    else if (cVar6 == '\x02') {
      *(undefined1 *)(unaff_A6 + 0x20) = 1;
      iVar7 = unaff_A5 + -0x6a8c;
    }
    else {
      uVar4 = candidate_29f2();
      iVar7 = extraout_A0;
      if (extraout_D1w < uVar4) {
        iVar7 = extraout_A1;
      }
      *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar4;
    }
    if ((short)((ushort)(byte)(-(-1 < (short)(*(short *)(iVar7 + 0x3c) - *(short *)(unaff_A6 + 0xf))
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
  uVar4 = candidate_29c6();
  if (((uVar4 & 0xff) < extraout_D1w_00) && (uVar4 >> 8 < extraout_D1w_00)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 2;
    sVar5 = 0x30;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar5 = -0x30;
    }
    candidate_510c(CONCAT22(*(undefined2 *)(unaff_A6 + 0x10),*(short *)(unaff_A6 + 0xf) + sVar5));
    candidate_aeb40();
  }
  sVar5 = *(short *)((int)unaff_A6 + 0x6e);
  if (sVar5 != 0) {
    bVar8 = (short)(sVar5 + -1) < 0;
    if (sVar5 == 1) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
    }
    else {
      candidate_29c6();
      if (bVar8) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      }
      else {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
      }
    }
  }
  sVar5 = *(short *)(unaff_A6 + 0x1d);
  if (sVar5 != 0) {
    sVar5 = sVar5 + -1;
    *(short *)(unaff_A6 + 0x1d) = sVar5;
  }
  bVar8 = sVar5 < 0;
  candidate_29c6();
  if (bVar8) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) ^ 0x80;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
  if (0xff < (ushort)(*(short *)(unaff_A6 + 0xf) - 0x20U)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xf7;
  }
  if (*(short *)((int)unaff_A6 + 0x92) != 0) {
    sVar5 = *(short *)((int)unaff_A6 + 0x92) >> 6;
    if (sVar5 == 0) {
      sVar5 = 1;
    }
    *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) - sVar5;
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
