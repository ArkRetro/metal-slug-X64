/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0afe4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_afe4e(void)

{
  code *pcVar1;
  uint uVar2;
  ushort uVar3;
  uint uVar4;
  ushort uVar5;
  short sVar6;
  ushort uVar7;
  char cVar8;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  uint unaff_D6;
  int extraout_A0;
  int iVar9;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;
  undefined1 in_CF;
  undefined1 uVar11;

  candidate_af0a8();
  if ((bool)in_CF) {
    uVar7 = candidate_29c6();
    *(short *)((int)unaff_A6 + 0x82) = (uVar7 & 0x1f) + *(short *)((int)unaff_A6 + 0x82);
  }
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0xafe82;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_4256();
    unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x62);
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    unaff_A6[0xd] = 0;
    *unaff_A6 = 0xb0082;
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x62) - unaff_A6[0xc] >> 5) + unaff_A6[0xc];
    uVar4 = -0x100000 - unaff_A6[0xd] >> 5;
    uVar2 = unaff_A6[0xd];
    uVar11 = CARRY4(uVar4,uVar2);
    unaff_A6[0xd] = uVar4 + uVar2;
    func_0x000143e4();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_afdfa;
      uVar11 = 0;
    }
    candidate_4c24();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_406b6();
    }
  }
  else {
    candidate_1456a();
    candidate_427a();
    uVar7 = *(short *)(unaff_A6 + 0x21) - *(short *)(unaff_A6 + 0xf);
    if ((int)((uint)uVar7 << 0x10) < 0) {
      uVar7 = -uVar7;
    }
    if (0xf < uVar7) {
      *unaff_A6 = candidate_aff16;
    }
    cVar8 = candidate_aee0c();
    if (cVar8 != '\0') {
      if (cVar8 == '\x01') {
        *unaff_A6 = candidate_affcc;
      }
      else {
        *unaff_A6 = candidate_b0014;
      }
    }
    uVar7 = *(ushort *)((int)unaff_A6 + 0x82) - 1;
    *(ushort *)((int)unaff_A6 + 0x82) = uVar7;
    if ((int)((uint)uVar7 << 0x10) < 0) {
      *unaff_A6 = candidate_b0014;
    }
    if ((*(byte *)(unaff_A6 + 0x1f) & 1) == 0) {
      *unaff_A6 = candidate_aff82;
    }
    if (*(short *)(unaff_A6 + 0x1d) == 0) {
      *unaff_A6 = 0xb0764;
    }
    if (*(char *)((int)unaff_A6 + 0x6d) != '@') {
      *unaff_A6 = candidate_b0014;
    }
    uVar11 = 0;
    candidate_4c24();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_407ca();
    }
  }
  candidate_ae9c0();
  candidate_1945c();
  if ((bool)uVar11) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar3 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7))
                        != 0));
      uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar5) & 1) != 0) {
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
    iVar9 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar9 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  cVar8 = candidate_1ae8e();
  if (cVar8 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  else {
    if (cVar8 == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x20) = 0;
      iVar9 = unaff_A5 + -0x6b3c;
    }
    else if (cVar8 == '\x02') {
      *(undefined1 *)(unaff_A6 + 0x20) = 1;
      iVar9 = unaff_A5 + -0x6a8c;
    }
    else {
      uVar7 = candidate_29f2();
      iVar9 = extraout_A0;
      if (extraout_D1w < uVar7) {
        iVar9 = extraout_A1;
      }
      *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar7;
    }
    if ((short)((ushort)(byte)(-(-1 < (short)(*(short *)(iVar9 + 0x3c) - *(short *)(unaff_A6 + 0xf))
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
  uVar7 = candidate_29c6();
  if (((uVar7 & 0xff) < extraout_D1w_00) && (uVar7 >> 8 < extraout_D1w_00)) {
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
    bVar10 = (short)(sVar6 + -1) < 0;
    if (sVar6 == 1) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
    }
    else {
      candidate_29c6();
      if (bVar10) {
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
  bVar10 = sVar6 < 0;
  candidate_29c6();
  if (bVar10) {
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
