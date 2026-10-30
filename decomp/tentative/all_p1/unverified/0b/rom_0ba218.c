/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ba218.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ba218(void)

{
  int iVar1;
  undefined2 uVar2;
  ushort uVar3;
  char cVar4;
  byte bVar5;
  uint uVar6;
  short sVar8;
  uint uVar7;
  short sVar9;
  ushort uVar10;
  ushort uVar11;
  ushort uVar12;
  ushort uVar13;
  ushort uVar14;
  byte bVar15;
  short sVar16;
  ushort uVar17;
  short sVar18;
  ushort uVar19;
  undefined2 *puVar20;
  undefined1 *puVar21;
  undefined1 *puVar22;
  undefined2 *puVar23;
  undefined2 *puVar24;
  int unaff_A5;
  int unaff_A6;
  bool bVar25;
  ushort uStack_16;
  short sStack_12;
  short sStack_e;

  uVar3 = uRam000e6db2;
  uVar6 = uRam000e6da8;
  sVar9 = 0x78;
  sVar8 = 0x1e;
  uVar19 = 0x10;
  uVar12 = 5;
  uVar13 = *(ushort *)(unaff_A6 + 0x74);
  sVar18 = *(short *)(unaff_A6 + 0x76);
  sVar16 = sRam0010c764 - uVar13;
  if (-1 < sVar16) {
    uVar13 = sVar16 + uVar13;
    sVar9 = sVar16 + 0x78;
    uVar19 = 0x10 - sVar16;
    if (uVar19 == 0 || 0x10 < sVar16) {
      return;
    }
    sVar16 = 0;
  }
  sVar16 = uVar19 + sVar16 + -0x20;
  uVar11 = uVar19;
  if ((sVar16 < 0) || (uVar11 = uVar19 - sVar16, uVar11 != 0 && sVar16 <= (short)uVar19)) {
    sVar16 = sRam0010c766 - sVar18;
    if (-1 < sVar16) {
      sVar18 = sVar16 + sVar18;
      sVar8 = 0x1e - sVar16;
      uVar12 = 5 - sVar16;
      if (uVar12 == 0 || 5 < sVar16) {
        return;
      }
      sVar16 = 0;
    }
    bVar15 = (byte)sVar18;
    sVar18 = uVar12 + sVar16 + -0x20;
    uVar19 = uVar12;
    if ((sVar18 < 0) || (uVar19 = uVar12 - sVar18, uVar19 != 0 && sVar18 <= (short)uVar12)) {
      uVar12 = (short)(sVar9 << 4) >> 4;
      uVar14 = (short)(sVar8 << 4) >> 4;
      uVar7 = (uint)uVar14;
      if (((short)(uVar12 - sRam000e6db0) < 0) && ((short)(uVar14 - uRam000e6db2) < 0)) {
        if ((short)uVar12 < 0) {
          bVar25 = SCARRY2(uVar12,uVar11);
          uVar11 = uVar12 + uVar11;
          if (uVar11 == 0 || bVar25 != (int)((uint)uVar11 << 0x10) < 0) {
            return;
          }
          uVar13 = uVar13 - uVar12;
          uVar12 = 0;
        }
        if ((short)uVar14 < 0) {
          bVar25 = SCARRY2(uVar14,uVar19);
          uVar19 = uVar14 + uVar19;
          if (uVar19 == 0 || bVar25 != (int)((uint)uVar19 << 0x10) < 0) {
            return;
          }
          bVar15 = (char)uVar14 + bVar15;
          uVar7 = 0;
        }
        sVar18 = (uVar11 + uVar12) - sRam000e6db0;
        uVar14 = uVar11;
        if ((sVar18 < 0) || (uVar14 = uVar11 - sVar18, uVar14 != 0 && sVar18 <= (short)uVar11)) {
          sVar9 = (short)uVar7;
          sVar18 = (uVar19 + sVar9) - uRam000e6db2;
          uVar11 = uVar19;
          if ((sVar18 < 0) || (uVar11 = uVar19 - sVar18, uVar11 != 0 && sVar18 <= (short)uVar19)) {
            if (uRam000e6da8 != 0) {
              if (uRam000e6da8 == 0xffffffff) {
                sVar18 = (sRam0010c75a + 1) * 2;
                uVar13 = (uVar13 & 0x1f) * 2;
                uVar19 = uVar14;
                do {
                  DAT_00300001 = (undefined1)sVar18;
                  uVar10 = (byte)((~bVar15 & 0x1f) * '\x02' +
                                 (*(char *)((short)uVar13 + 0x10c770) >> 2)) & 0x3e;
                  uVar3 = uVar10 + (uVar13 + sVar18) * 0x20;
                  uVar17 = uVar11;
                  do {
                    _DAT_003c0000 = (uint)uVar3 * 0x10000 + 0x10000;
                    sVar16 = ((short)((uint)_DAT_003c0000 >> 0x10) + -1) - uVar10;
                    bVar5 = (char)uVar10 + 2;
                    uVar10 = bVar5 & 0xff3e;
                    uVar3 = CONCAT11((char)((ushort)sVar16 >> 8),(bVar5 & 0x3e) + (char)sVar16);
                    uVar17 = uVar17 - 1;
                  } while (uVar17 != 0);
                  uVar13 = uVar13 + 2 & 0x3e;
                  uVar19 = uVar19 - 1;
                } while (uVar19 != 0);
              }
              else {
                uVar19 = (ushort)(uRam000e6da8 >> 0x10);
                bVar5 = (byte)((ushort)(uVar19 << 0xc) >> 8) | (byte)(uRam000e6da8 >> 0x1c);
                cVar4 = (char)(uVar19 >> 4) + -1;
                *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar5,cVar4);
                do {
                } while (cVar4 != DAT_002fffff);
                uVar19 = (ushort)CONCAT31((uint3)bVar5,2);
                if (-1 < sRam0010c75a) {
                  puVar20 = (undefined2 *)
                            ((uVar7 + (uint)uVar12 * (uint)uVar3) * 4 +
                            ((uint)(ushort)(uVar19 << 4 | uVar19 >> 0xc) << 0x10 | uVar6 & 0xffff));
                  uVar13 = (uVar13 & 0x1f) * 2;
                  uVar19 = uVar14;
                  do {
                    uVar10 = (byte)((~bVar15 & 0x1f) * '\x02' +
                                   (cRam0010c7b1 + (*(char *)((short)uVar13 + 0x10c770) >> 3)) *
                                   '\x02') & 0x3e;
                    sVar18 = uVar10 + (uVar13 + (sRam0010c75a + 1) * 2) * 0x20;
                    puVar23 = puVar20;
                    uVar17 = uVar11;
                    do {
                      puVar24 = puVar23 + 1;
                      uVar2 = *puVar23;
                      puVar23 = puVar23 + 2;
                      sVar18 = (short)((uint)(CONCAT22(sVar18,uVar2) + 0x10000) >> 0x10);
                      _DAT_003c0000 = CONCAT22(sVar18,*puVar24);
                      sVar18 = (sVar18 + -1) - uVar10;
                      bVar5 = (char)uVar10 + 2;
                      uVar10 = bVar5 & 0xff3e;
                      sVar18 = CONCAT11((char)((ushort)sVar18 >> 8),(bVar5 & 0x3e) + (char)sVar18);
                      uVar17 = uVar17 - 1;
                    } while (uVar17 != 0);
                    puVar20 = (undefined2 *)((int)(short)(uVar3 << 2) + (int)puVar20);
                    uVar13 = uVar13 + 2 & 0x3e;
                    uVar19 = uVar19 - 1;
                  } while (uVar19 != 0);
                }
              }
            }
            *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
            uVar13 = uRam000e6db2;
            uVar6 = uRam000e6dac;
            do {
            } while (DAT_002fffff != '\x01');
            if (uRam000e6dac != 0) {
              if (uRam000e6dac == 0xffffffff) {
                uVar14 = uVar14 << 7;
                do {
                  DAT_00300001 = 0;
                  uVar14 = uVar14 & 0xf80;
                  uVar13 = sStack_e * 2;
                  sVar18 = sStack_12;
                  do {
                    uVar13 = uVar13 & 0x3e;
                    *(undefined2 *)((short)uVar14 + 0x10c7b6 + (int)(short)uVar13) = 0;
                    *(undefined2 *)((short)uVar14 + 0x10c7f6 + (int)(short)uVar13) = 0;
                    uVar13 = uVar13 + 2;
                    sVar18 = sVar18 + -1;
                  } while (sVar18 != 0);
                  uVar14 = uVar14 + 0x80;
                  sVar9 = sVar9 + -1;
                } while (sVar9 != 0);
              }
              else {
                uVar19 = (ushort)(uRam000e6dac >> 0x10);
                bVar15 = (byte)((ushort)(uVar19 << 0xc) >> 8) | (byte)(uRam000e6dac >> 0x1c);
                cVar4 = (char)(uVar19 >> 4) + -1;
                *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar15,cVar4);
                do {
                } while (cVar4 != DAT_002fffff);
                uVar19 = (ushort)CONCAT31((uint3)bVar15,2);
                puVar22 = (undefined1 *)
                          (((uint)uStack_16 + (uint)uVar12 * (uint)uVar13) * 4 +
                          ((uint)(ushort)(uVar19 << 4 | uVar19 >> 0xc) << 0x10 | uVar6 & 0xffff));
                uVar14 = uVar14 << 7;
                do {
                  iVar1 = (int)(short)(uVar14 & 0xf80);
                  uVar19 = sStack_e * 2;
                  sVar18 = sStack_12;
                  do {
                    uVar19 = uVar19 & 0x3e;
                    *(undefined1 *)(iVar1 + 0x10c7b6 + (int)(short)uVar19) = *puVar22;
                    *(undefined1 *)(iVar1 + 0x10c7f6 + (int)(short)uVar19) = puVar22[1];
                    puVar21 = puVar22 + 3;
                    *(undefined1 *)(iVar1 + 0x10c7b7 + (int)(short)uVar19) = puVar22[2];
                    puVar22 = puVar22 + 4;
                    *(undefined1 *)(iVar1 + 0x10c7f7 + (int)(short)uVar19) = *puVar21;
                    uVar19 = uVar19 + 2;
                    sVar18 = sVar18 + -1;
                  } while (sVar18 != 0);
                  puVar22 = puVar22 + (short)((uVar13 - sStack_12) * 4);
                  uVar14 = (uVar14 & 0xf80) + 0x80;
                  sVar9 = sVar9 + -1;
                } while (sVar9 != 0);
              }
            }
            *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
            _DAT_002ffff0 = 1;
            do {
            } while (DAT_002fffff != '\x01');
            return;
          }
        }
      }
      return;
    }
  }
  return;
}
