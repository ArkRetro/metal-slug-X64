/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b08ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_b08ac(void)

{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  char cVar10;
  ushort uVar8;
  short sVar9;
  undefined4 uVar7;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  short extraout_D1w_01;
  ushort uVar11;
  undefined4 unaff_D2;
  int iVar12;
  short unaff_D3w;
  short sVar13;
  uint unaff_D6;
  int extraout_A0;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar14;
  undefined1 uVar15;

  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0xb08ca;
  uVar15 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    unaff_A6[0xd] = 0;
    *unaff_A6 = 0xafd10;
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x62) - unaff_A6[0xc] >> 5) + unaff_A6[0xc];
    uVar5 = -0x100000 - unaff_A6[0xd] >> 5;
    uVar2 = unaff_A6[0xd];
    uVar15 = CARRY4(uVar5,uVar2);
    unaff_A6[0xd] = uVar5 + uVar2;
    func_0x000143e4();
    candidate_427a();
    if (((bool)uVar15) && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) {
      *unaff_A6 = candidate_afd52;
      uVar15 = 0;
    }
    candidate_4c24();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_406b6();
    }
    candidate_ae9c0();
    candidate_1945c();
    if (!(bool)uVar15) goto LAB_000aeb5e;
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar4 = uRam002fffe8;
      uVar6 = ~uVar8 & 0xfff;
      uVar8 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                               -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar6 & 7)) != 0));
      uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar6));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar8 ^ uVar4) & 1) != 0) {
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
  }
  else {
    candidate_1456a();
    candidate_427a();
    if ((bool)uVar15) {
      *unaff_A6 = 0xb063e;
      uVar15 = 0;
    }
    candidate_19504();
    if (!(bool)uVar15) {
      uVar7 = candidate_1903a();
      sVar9 = extraout_D1w_01 >> 1;
      if (sVar9 < 0) {
        sVar9 = -sVar9;
      }
      uVar8 = (short)uVar7 - *(short *)(unaff_A6 + 0x21);
      uVar6 = (ushort)((uint)uVar7 >> 0x10);
      if ((int)((uint)uVar8 << 0x10) < 0) {
        sVar9 = -sVar9;
      }
      sVar13 = unaff_D3w >> 1;
      if (sVar13 < 0) {
        sVar13 = -sVar13;
      }
      uVar4 = (short)unaff_D2 - *(short *)(unaff_A6 + 0x21);
      uVar11 = (ushort)((uint)unaff_D2 >> 0x10);
      if ((int)((uint)uVar4 << 0x10) < 0) {
        sVar13 = -sVar13;
      }
      iVar12 = ((int)(CONCAT22(uVar6,sVar9 + uVar8 + -0x8000) << 0x10 | (uint)uVar6) >> 8) +
               ((int)(CONCAT22(uVar11,sVar13 + uVar4 + -0x8000) << 0x10 | (uint)uVar11) >> 8);
      if (*(short *)((int)unaff_A6 + 0x92) != 0) {
        iVar12 = ((int)((uint)(ushort)(((*(short *)(unaff_A5 + 0x697c) +
                                        *(short *)((int)unaff_A6 + 0x8e)) -
                                       *(short *)(unaff_A6 + 0xf)) + 0x8000) << 0x10) >> 6) + iVar12
        ;
      }
      sVar9 = *(short *)(unaff_A6 + 0x23) + 0x77;
      *(short *)(unaff_A6 + 0x23) = sVar9;
      iVar3 = unaff_A6[0x22];
      if (-1 < iVar3) {
        sVar9 = -sVar9;
      }
      iVar12 = unaff_A6[0x22] +
               ((iVar12 >> 6) - CONCAT22((short)(iVar3 >> 0x16),sVar9 * 2 + (short)(iVar3 >> 6)));
      unaff_A6[0x22] = iVar12;
      if (!SCARRY4(unaff_A6[0x21],iVar12)) {
        unaff_A6[0x21] = unaff_A6[0x21] + iVar12;
      }
      candidate_4c24();
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        candidate_406b6();
      }
LAB_000aeb5e:
      cVar10 = candidate_1ae8e();
      if (cVar10 == '\0') {
        *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
      }
      else {
        if (cVar10 == '\x01') {
          *(undefined1 *)(unaff_A6 + 0x20) = 0;
          iVar12 = unaff_A5 + -0x6b3c;
        }
        else if (cVar10 == '\x02') {
          *(undefined1 *)(unaff_A6 + 0x20) = 1;
          iVar12 = unaff_A5 + -0x6a8c;
        }
        else {
          uVar8 = candidate_29f2();
          iVar12 = extraout_A0;
          if (extraout_D1w < uVar8) {
            iVar12 = extraout_A1;
          }
          *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar8;
        }
        if ((short)((ushort)(byte)(-(-1 < (short)(*(short *)(iVar12 + 0x3c) -
                                                 *(short *)(unaff_A6 + 0xf))) ^
                                  -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
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
      uVar8 = candidate_29c6();
      if (((uVar8 & 0xff) < extraout_D1w_00) && (uVar8 >> 8 < extraout_D1w_00)) {
        *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 2;
        sVar9 = 0x30;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          sVar9 = -0x30;
        }
        candidate_510c(CONCAT22(*(undefined2 *)(unaff_A6 + 0x10),*(short *)(unaff_A6 + 0xf) + sVar9)
                      );
        candidate_aeb40();
      }
      sVar9 = *(short *)((int)unaff_A6 + 0x6e);
      if (sVar9 != 0) {
        bVar14 = (short)(sVar9 + -1) < 0;
        if (sVar9 == 1) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
        }
        else {
          candidate_29c6();
          if (bVar14) {
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
          }
          else {
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
          }
        }
      }
      sVar9 = *(short *)(unaff_A6 + 0x1d);
      if (sVar9 != 0) {
        sVar9 = sVar9 + -1;
        *(short *)(unaff_A6 + 0x1d) = sVar9;
      }
      bVar14 = sVar9 < 0;
      candidate_29c6();
      if (bVar14) {
        *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) ^ 0x80;
      }
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
      if (0xff < (ushort)(*(short *)(unaff_A6 + 0xf) - 0x20U)) {
        *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xf7;
      }
      if (*(short *)((int)unaff_A6 + 0x92) != 0) {
        sVar9 = *(short *)((int)unaff_A6 + 0x92) >> 6;
        if (sVar9 == 0) {
          sVar9 = 1;
        }
        *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) - sVar9;
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
  }
  candidate_4126();
  iVar12 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar12 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
