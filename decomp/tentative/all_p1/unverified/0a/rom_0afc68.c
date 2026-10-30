/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0afc68.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_afc68(void)

{
  code *pcVar1;
  uint uVar2;
  ushort uVar3;
  uint uVar4;
  ushort uVar5;
  char cVar9;
  ushort uVar7;
  short sVar8;
  undefined4 uVar6;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  uint unaff_D6;
  int extraout_A0;
  int iVar10;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar11;
  undefined1 uVar12;

  candidate_4256();
  uVar6 = 0x30000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar6 = 0xfffd0000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar6;
  unaff_A6[0xc] = uVar6;
  unaff_A6[0xd] = 0x20000;
  *unaff_A6 = 0xafd10;
  unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x62) - unaff_A6[0xc] >> 5) + unaff_A6[0xc];
  uVar4 = -0x100000 - unaff_A6[0xd] >> 5;
  uVar2 = unaff_A6[0xd];
  uVar12 = CARRY4(uVar4,uVar2);
  unaff_A6[0xd] = uVar4 + uVar2;
  func_0x000143e4();
  candidate_427a();
  if (((bool)uVar12) && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) {
    *unaff_A6 = candidate_afd52;
    uVar12 = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_406b6();
  }
  candidate_ae9c0();
  candidate_1945c();
  if ((bool)uVar12) {
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
    iVar10 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar10 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  cVar9 = candidate_1ae8e();
  if (cVar9 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  else {
    if (cVar9 == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x20) = 0;
      iVar10 = unaff_A5 + -0x6b3c;
    }
    else if (cVar9 == '\x02') {
      *(undefined1 *)(unaff_A6 + 0x20) = 1;
      iVar10 = unaff_A5 + -0x6a8c;
    }
    else {
      uVar7 = candidate_29f2();
      iVar10 = extraout_A0;
      if (extraout_D1w < uVar7) {
        iVar10 = extraout_A1;
      }
      *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar7;
    }
    if ((short)((ushort)(byte)(-(-1 < (short)(*(short *)(iVar10 + 0x3c) - *(short *)(unaff_A6 + 0xf)
                                             )) ^ -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) <<
               8) < 0) {
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
    sVar8 = 0x30;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar8 = -0x30;
    }
    candidate_510c(CONCAT22(*(undefined2 *)(unaff_A6 + 0x10),*(short *)(unaff_A6 + 0xf) + sVar8));
    candidate_aeb40();
  }
  sVar8 = *(short *)((int)unaff_A6 + 0x6e);
  if (sVar8 != 0) {
    bVar11 = (short)(sVar8 + -1) < 0;
    if (sVar8 == 1) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
    }
    else {
      candidate_29c6();
      if (bVar11) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      }
      else {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
      }
    }
  }
  sVar8 = *(short *)(unaff_A6 + 0x1d);
  if (sVar8 != 0) {
    sVar8 = sVar8 + -1;
    *(short *)(unaff_A6 + 0x1d) = sVar8;
  }
  bVar11 = sVar8 < 0;
  candidate_29c6();
  if (bVar11) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) ^ 0x80;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
  if (0xff < (ushort)(*(short *)(unaff_A6 + 0xf) - 0x20U)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xf7;
  }
  if (*(short *)((int)unaff_A6 + 0x92) != 0) {
    sVar8 = *(short *)((int)unaff_A6 + 0x92) >> 6;
    if (sVar8 == 0) {
      sVar8 = 1;
    }
    *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) - sVar8;
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
