/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a1ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_3a1ae(void)

{
  code *pcVar1;
  short *psVar2;
  ushort uVar3;
  undefined2 extraout_D0u;
  short sVar4;
  ushort uVar5;
  short sVar6;
  ushort uVar7;
  short sVar8;
  undefined4 unaff_D2;
  short sVar9;
  undefined4 unaff_D3;
  uint uVar10;
  uint uVar11;
  uint *puVar12;
  uint *puVar13;
  int *piVar14;
  int iVar16;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar17;
  undefined1 uVar18;
  int *piVar15;

  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)(unaff_A6 + 0x1e) = 0xffff;
  *unaff_A6 = 0x3a1e4;
  uVar18 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar18) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
    if (0xfc10 < uVar5) {
      uVar5 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar7 = ~uVar5 & 0xfff;
      uVar5 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                               -((*(byte *)((short)((ushort)(CONCAT22(extraout_D0u,uVar5) >> 3) &
                                                   0xfff) + 0xdedd2) & '\x01' << (uVar7 & 7)) != 0))
      ;
      uRam002fffe0 = uVar5 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar7));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar7 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar7 - 0x800)) & (uVar5 ^ uVar3) & 1) != 0) {
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
    iVar16 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar16 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  sVar8 = *(short *)(unaff_A6 + 0x1e);
  if (-1 < sVar8) {
    psVar2 = *(short **)
              (*(int *)((short)(*(short *)((int)unaff_A6 + 0x72) * 4) + 0x3a8a0) +
              (int)(short)(sVar8 * 4));
    if ((DAT_0010f000 & 0x20) != 0) {
      candidate_4dca();
    }
    iVar16 = unaff_A5 + -0x1c1c;
    if (-1 < *(char *)(unaff_A5 + -0x1c1e)) {
      iVar16 = unaff_A5 + -0x1a6a;
    }
    *(undefined2 *)(unaff_A5 + -0x18ac) = 0;
    *(undefined4 *)(unaff_A5 + -0x182a) = 0;
    *(undefined2 *)(unaff_A5 + -0x1826) = 0;
    *(undefined4 *)(unaff_A5 + -0x1824) = 0;
    *(undefined2 *)(unaff_A5 + -0x1820) = 0xffff;
    sVar4 = *psVar2;
    sVar6 = psVar2[1];
    sVar8 = psVar2[2];
    sVar9 = psVar2[3];
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      sVar4 = -(sVar8 + sVar4);
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 2) != 0) {
      sVar6 = -(sVar9 + sVar6);
    }
    uVar5 = *(short *)(unaff_A6 + 0xf) + sVar4 + 0x20;
    if ((short)(*(short *)(unaff_A6 + 0xf) + sVar4) < -0x20) {
      bVar17 = SCARRY2(uVar5,sVar8);
      sVar8 = uVar5 + sVar8;
      if (bVar17 != sVar8 < 0) {
        return 0;
      }
      uVar5 = 0;
    }
    if ((short)uVar5 < 0x180) {
      sVar8 = uVar5 + sVar8;
      if (0x17f < sVar8) {
        sVar8 = 0x17f;
      }
      if ((ushort)(sVar8 - uVar5) != 0) {
        uVar7 = (*(short *)(unaff_A6 + 0x10) + sVar6) - 0x108;
        if ((short)(*(short *)(unaff_A6 + 0x10) + sVar6) < 0x108) {
          bVar17 = SCARRY2(uVar7,sVar9);
          sVar9 = uVar7 + sVar9;
          if (bVar17 != sVar9 < 0) {
            return 0;
          }
          uVar7 = 0;
        }
        if ((short)uVar7 < 0xf0) {
          sVar9 = uVar7 + sVar9;
          if (0xef < sVar9) {
            sVar9 = 0xef;
          }
          if ((ushort)(sVar9 - uVar7) != 0) {
            sVar8 = ((ushort)(uVar5 + (sVar8 - uVar5)) >> 5) - (uVar5 >> 5);
            sVar9 = ((ushort)(uVar7 + (sVar9 - uVar7)) >> 5) - (uVar7 >> 5);
            puVar13 = (uint *)(iVar16 + 0x102 + (int)(short)((uVar5 >> 5) << 3));
            uVar11 = 0;
            uVar10 = 0;
            do {
              puVar12 = puVar13 + 1;
              uVar11 = *puVar13 | uVar11;
              puVar13 = puVar13 + 2;
              uVar10 = *puVar12 | uVar10;
              sVar8 = sVar8 + -1;
            } while (sVar8 != -1);
            *(uint *)(unaff_A5 + -0x18b8) = uVar10;
            *(uint *)(unaff_A5 + -0x18b4) = uVar11;
            if (uVar10 != 0 || uVar11 != 0) {
              puVar13 = (uint *)(iVar16 + 0x162 + (int)(short)((uVar7 >> 5) << 3));
              uVar11 = 0;
              do {
                puVar12 = puVar13 + 1;
                uVar11 = *puVar13 | uVar11;
                puVar13 = puVar13 + 2;
                uVar10 = *puVar12 | uVar10;
                sVar9 = sVar9 + -1;
              } while (sVar9 != -1);
              *(uint *)(unaff_A5 + -0x18b8) = uVar10 & *(uint *)(unaff_A5 + -0x18b8);
              *(uint *)(unaff_A5 + -0x18b4) = uVar11 & *(uint *)(unaff_A5 + -0x18b4);
              if (uVar10 != 0 || uVar11 != 0) {
                piVar14 = (int *)(psVar2 + 5);
                while( true ) {
                  piVar15 = piVar14 + 1;
                  if (*piVar14 < 0) break;
                  if ((short)((uint)*piVar14 >> 0x10) == 0) {
                    puVar13 = (uint *)(iVar16 + 0x1a2);
                  }
                  else {
                    puVar13 = (uint *)(iVar16 + 0x1aa);
                  }
                  uVar11 = *puVar13 & *(uint *)(unaff_A5 + -0x18b4);
                  uVar10 = puVar13[1] & *(uint *)(unaff_A5 + -0x18b8);
                  if (uVar11 != 0) {
                    sVar8 = (short)uVar11;
                    while (uVar5 = (ushort)(uVar11 >> 0x10), sVar8 != 0) {
                      if (sVar8 < 0) {
                        candidate_4762();
                      }
                      sVar8 = (short)uVar11 * 2;
                      uVar11 = CONCAT22(uVar5,sVar8);
                    }
                    uVar11 = uVar11 >> 0x10;
                    while (uVar5 != 0) {
                      if ((short)uVar5 < 0) {
                        candidate_4762();
                      }
                      uVar5 = (short)uVar11 * 2;
                      uVar11 = (uint)uVar5;
                    }
                  }
                  piVar14 = piVar15;
                  if (uVar10 != 0) {
                    sVar8 = (short)uVar10;
                    while (uVar5 = (ushort)(uVar10 >> 0x10), sVar8 != 0) {
                      if (sVar8 < 0) {
                        candidate_4762();
                      }
                      sVar8 = (short)uVar10 * 2;
                      uVar10 = CONCAT22(uVar5,sVar8);
                    }
                    uVar10 = uVar10 >> 0x10;
                    while (uVar5 != 0) {
                      if ((short)uVar5 < 0) {
                        candidate_4762();
                      }
                      uVar5 = (short)uVar10 * 2;
                      uVar10 = (uint)uVar5;
                    }
                  }
                }
              }
            }
            return (uint)*(ushort *)(unaff_A5 + -0x18ac);
          }
        }
      }
    }
    return 0;
  }
  return CONCAT22(extraout_D0u,sVar8);
}
