/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d732.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_5d732(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  char cVar11;
  ushort uVar9;
  short sVar10;
  uint uVar7;
  byte bVar12;
  undefined4 uVar8;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  short extraout_D1w_01;
  ushort uVar13;
  int unaff_D2;
  int iVar14;
  undefined4 unaff_D3;
  short sVar15;
  uint unaff_D6;
  int extraout_A0;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar16;
  undefined1 uVar17;
  short sStack_6;
  undefined4 uStack_4;

  unaff_A6[0x14] = 0x5e918;
  bVar16 = false;
  uStack_4 = 0x5d74a;
  candidate_40bc();
  if (bVar16) {
    uStack_4 = 0x5d75a;
    candidate_1b220();
  }
  *unaff_A6 = 0x5d764;
  uStack_4 = 0xb0634;
  candidate_b05dc();
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0x7f;
  uStack_4 = 0xb0644;
  uVar7 = candidate_29c6();
  unaff_A6[0x22] = (uVar7 & 0xff) - 0x80;
  if ((*(byte *)(unaff_A6 + 0x1f) & 0x80) == 0) {
    uStack_4 = 0xb0682;
    candidate_4256();
  }
  else {
    uStack_4 = 0xb066e;
    candidate_4256();
  }
  uStack_4 = 0xb0688;
  uVar7 = candidate_29c6();
  iVar14 = (uVar7 & 0xffff) + 0x40000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar14 = -iVar14;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar14;
  if (iVar14 < 0) {
    iVar14 = -iVar14;
  }
  *(short *)((int)unaff_A6 + 0x26) = (short)((uint)(iVar14 << 6) >> 0x10) + 0x80;
  unaff_A6[0x21] = unaff_A6[0xf];
  *unaff_A6 = 0xb06c4;
  sVar10 = (short)unaff_D3;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
LAB_000b083e:
    sStack_6 = sVar10;
    uVar8 = 0x28000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar8 = 0xfffd8000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar8;
    unaff_A6[0xc] = uVar8;
    uStack_4 = 0xb0868;
    candidate_4256();
    *unaff_A6 = 0xb086e;
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x62) - unaff_A6[0xc] >> 5) + unaff_A6[0xc];
    uVar5 = -0x100000 - unaff_A6[0xd] >> 5;
    uVar7 = unaff_A6[0xd];
    uVar17 = CARRY4(uVar5,uVar7);
    unaff_A6[0xd] = uVar5 + uVar7;
    uStack_4 = 0xb0892;
    func_0x000143e4();
    uStack_4 = 0xb0898;
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_b08ac;
      uVar17 = 0;
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
    iVar14 = *(uint *)((int)unaff_A6 + 0x62) * 8;
    if ((int)(*(uint *)((int)unaff_A6 + 0x62) ^ iVar14 + (unaff_A6[0x21] - unaff_A6[0xf])) < 0) {
      uStack_4 = 0xb06f6;
      bVar12 = candidate_29c6();
      if ((bVar12 & 7) == 0) {
        unaff_A6[0xd] = 0x8a000;
        uVar8 = 0x28000;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          uVar8 = 0xfffd8000;
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = uVar8;
        unaff_A6[0xc] = uVar8;
        uStack_4 = 0xafbf2;
        candidate_4256();
        *unaff_A6 = 0xafbf8;
        unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x62) - unaff_A6[0xc] >> 5) + unaff_A6[0xc];
        uVar5 = -0x100000 - unaff_A6[0xd] >> 5;
        uVar7 = unaff_A6[0xd];
        uVar17 = CARRY4(uVar5,uVar7);
        unaff_A6[0xd] = uVar5 + uVar7;
        uStack_4 = 0xafc1c;
        func_0x000143e4();
        uStack_4 = 0xafc22;
        candidate_427a();
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
          *unaff_A6 = candidate_afd52;
          uVar17 = 0;
        }
        uStack_4 = 0xb0524;
        candidate_4c24();
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
          uStack_4 = 0xb0534;
          candidate_406b6();
        }
        uStack_4 = 0xb053c;
        candidate_ae9c0();
        uStack_4 = 0xb0546;
        candidate_1945c();
        if (!(bool)uVar17) goto LAB_000aeb5e;
        uVar9 = *(ushort *)(unaff_A5 + 0x700c);
        *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
        if (0xfc10 < uVar9) {
          uVar9 = *(ushort *)(unaff_A5 + 0x700a);
          *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
          uVar4 = uRam002fffe8;
          uVar6 = ~uVar9 & 0xfff;
          uVar9 = (ushort)CONCAT31((int3)((uint)iVar14 >> 8),
                                   -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                     '\x01' << (uVar6 & 7)) != 0));
          uRam002fffe0 = uVar9 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar6));
          uRam002fffe2 = 0;
          uRam002fffe4 = 0;
          uRam002fffe6 = 0;
          *(short *)(unaff_A5 + 0x700a) =
               ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU)
               + *(short *)(unaff_A5 + 0x700a);
          if (((ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar9 ^ uVar4) & 1) != 0) {
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
            uStack_4 = 0x3d2c;
            func_0x0003cf12();
            uStack_4 = 0x3d36;
            candidate_3e2a8();
            do {
                    /* WARNING: Do nothing block with infinite loop */
            } while( true );
          }
        }
        goto code_r0x00004188;
      }
      *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    }
    sVar10 = 0x40;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar10 = -0x40;
    }
    sVar15 = *(short *)(unaff_A6 + 0xf);
    uVar17 = 0;
    uStack_4 = CONCAT22(*(undefined2 *)(unaff_A6 + 0x10),uStack_4._2_2_);
    candidate_510c();
    unaff_D2 = (int)(short)(sVar15 + sVar10);
    register0x0000003c = (BADSPACEBASE *)&uStack_4;
    candidate_aeb40();
    candidate_1456a();
    candidate_427a();
    candidate_af02a();
    if ((bool)uVar17) {
      *unaff_A6 = candidate_afbc0;
      uVar17 = 0;
    }
  }
  else {
    uStack_4 = 0xb07ae;
    candidate_4256();
    uVar8 = 0x10000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar8 = 0xffff0000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar8;
    unaff_A6[0x21] = unaff_A6[0xf];
    *unaff_A6 = 0xb07d0;
    unaff_D2 = CONCAT22((short)((uint)unaff_D2 >> 0x10),
                        *(short *)(unaff_A6 + 0xf) + *(short *)((int)unaff_A6 + 0x62) * 4);
    sStack_6 = *(short *)(unaff_A6 + 0x10);
    uStack_4 = 0xb07e6;
    candidate_aeb40();
    uVar17 = false;
    if ((int)((uint)(ushort)(*(ushort *)((int)unaff_A6 + 0x62) ^
                            *(short *)(unaff_A6 + 0x21) - *(short *)(unaff_A6 + 0xf)) << 0x10) < 0)
    {
      iVar14 = -*(int *)((int)unaff_A6 + 0x62);
      *(int *)((int)unaff_A6 + 0x62) = iVar14;
      uVar17 = iVar14 != 0;
    }
    sVar10 = sStack_6;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000b083e;
    uStack_4 = 0xb080c;
    candidate_1456a();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
      if (*(short *)((int)unaff_A6 + 0x62) < 0) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      }
      else {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      }
      *unaff_A6 = 0xb063e;
      uVar17 = 0;
    }
    uStack_4 = 0xb083a;
    candidate_427a();
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0xb08f8;
  candidate_19504();
  if (!(bool)uVar17) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0xb0908;
    uVar8 = candidate_1903a();
    sVar10 = extraout_D1w_01 >> 1;
    if (sVar10 < 0) {
      sVar10 = -sVar10;
    }
    uVar9 = (short)uVar8 - *(short *)(unaff_A6 + 0x21);
    uVar6 = (ushort)((uint)uVar8 >> 0x10);
    if ((int)((uint)uVar9 << 0x10) < 0) {
      sVar10 = -sVar10;
    }
    sVar15 = sStack_6 >> 1;
    if (sVar15 < 0) {
      sVar15 = -sVar15;
    }
    uVar4 = (short)unaff_D2 - *(short *)(unaff_A6 + 0x21);
    uVar13 = (ushort)((uint)unaff_D2 >> 0x10);
    if ((int)((uint)uVar4 << 0x10) < 0) {
      sVar15 = -sVar15;
    }
    iVar14 = ((int)(CONCAT22(uVar6,sVar10 + uVar9 + -0x8000) << 0x10 | (uint)uVar6) >> 8) +
             ((int)(CONCAT22(uVar13,sVar15 + uVar4 + -0x8000) << 0x10 | (uint)uVar13) >> 8);
    if (*(short *)((int)unaff_A6 + 0x92) != 0) {
      iVar14 = ((int)((uint)(ushort)(((*(short *)(unaff_A5 + 0x697c) +
                                      *(short *)((int)unaff_A6 + 0x8e)) - *(short *)(unaff_A6 + 0xf)
                                     ) + 0x8000) << 0x10) >> 6) + iVar14;
    }
    sVar10 = *(short *)(unaff_A6 + 0x23) + 0x77;
    *(short *)(unaff_A6 + 0x23) = sVar10;
    iVar2 = unaff_A6[0x22];
    if (-1 < iVar2) {
      sVar10 = -sVar10;
    }
    iVar14 = unaff_A6[0x22] +
             ((iVar14 >> 6) - CONCAT22((short)(iVar2 >> 0x16),sVar10 * 2 + (short)(iVar2 >> 6)));
    unaff_A6[0x22] = iVar14;
    if (!SCARRY4(unaff_A6[0x21],iVar14)) {
      unaff_A6[0x21] = unaff_A6[0x21] + iVar14;
    }
    *(undefined4 *)((int)register0x0000003c + -4) = 0xb099e;
    candidate_4c24();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *(undefined4 *)((int)register0x0000003c + -4) = 0xb09ae;
      candidate_406b6();
    }
LAB_000aeb5e:
    *(undefined4 *)((int)register0x0000003c + -4) = 0xaeb64;
    cVar11 = candidate_1ae8e();
    if (cVar11 == '\0') {
      *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
    }
    else {
      if (cVar11 == '\x01') {
        *(undefined1 *)(unaff_A6 + 0x20) = 0;
        iVar14 = unaff_A5 + -0x6b3c;
      }
      else if (cVar11 == '\x02') {
        *(undefined1 *)(unaff_A6 + 0x20) = 1;
        iVar14 = unaff_A5 + -0x6a8c;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xaebe4;
        uVar9 = candidate_29f2();
        iVar14 = extraout_A0;
        if (extraout_D1w < uVar9) {
          iVar14 = extraout_A1;
        }
        *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar9;
      }
      if ((int)((uint)(byte)(-(-1 < (short)(*(short *)(iVar14 + 0x3c) - *(short *)(unaff_A6 + 0xf)))
                            ^ -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 0x18) < 0) {
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
    *(undefined4 *)((int)register0x0000003c + -4) = 0xaec3c;
    candidate_260c8();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xaec44;
    uVar9 = candidate_29c6();
    if (((uVar9 & 0xff) < extraout_D1w_00) && (uVar9 >> 8 < extraout_D1w_00)) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 2;
      sVar10 = 0x30;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        sVar10 = -0x30;
      }
      uVar3 = *(undefined2 *)(unaff_A6 + 0x10);
      *(short *)((int)register0x0000003c + -2) = *(short *)(unaff_A6 + 0xf) + sVar10;
      *(undefined2 *)((int)register0x0000003c + -4) = uVar3;
      *(undefined4 *)((int)register0x0000003c + -8) = 0xaec80;
      candidate_510c();
      *(undefined4 *)((int)register0x0000003c + -4) = 0xaec88;
      candidate_aeb40();
    }
    sVar10 = *(short *)((int)unaff_A6 + 0x6e);
    if (sVar10 != 0) {
      bVar16 = (short)(sVar10 + -1) < 0;
      if (sVar10 == 1) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0xaecae;
        candidate_29c6();
        if (bVar16) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
        }
        else {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
        }
      }
    }
    sVar10 = *(short *)(unaff_A6 + 0x1d);
    if (sVar10 != 0) {
      sVar10 = sVar10 + -1;
      *(short *)(unaff_A6 + 0x1d) = sVar10;
    }
    bVar16 = sVar10 < 0;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xaece2;
    candidate_29c6();
    if (bVar16) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) ^ 0x80;
    }
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
    if (0xff < (ushort)(*(short *)(unaff_A6 + 0xf) - 0x20U)) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xf7;
    }
    if (*(short *)((int)unaff_A6 + 0x92) != 0) {
      sVar10 = *(short *)((int)unaff_A6 + 0x92) >> 6;
      if (sVar10 == 0) {
        sVar10 = 1;
      }
      *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) - sVar10;
    }
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xdf;
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xef;
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xbf;
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfb;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xaed54;
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
    *(undefined4 *)((int)register0x0000003c + -4) = 0xaed98;
    candidate_14850();
    if ((unaff_D6 & 4) != 0) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 4;
    }
    return;
  }
code_r0x00004188:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x418a;
  candidate_4126();
  iVar14 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar14 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
