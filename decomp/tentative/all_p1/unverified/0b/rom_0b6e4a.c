/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b6e4a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_b6e4a(void)

{
  int iVar1;
  undefined2 uVar2;
  ushort uVar3;
  char cVar4;
  byte bVar5;
  uint uVar6;
  short sVar7;
  ushort uVar8;
  short sVar9;
  ushort uVar10;
  ushort uVar11;
  ushort uVar12;
  ushort uVar13;
  byte bVar14;
  short sVar15;
  ushort uVar16;
  short sVar17;
  ushort uVar18;
  ushort uVar19;
  undefined2 *puVar20;
  undefined1 *puVar21;
  undefined1 *puVar22;
  undefined2 *puVar23;
  undefined2 *puVar24;
  int unaff_A5;
  bool bVar25;
  ushort uStack_16;
  short sStack_12;
  short sStack_e;

  uVar3 = uRam000b6bf6;
  uVar6 = uRam000b6bec;
  sVar9 = 0;
  uVar18 = 7;
  uVar12 = 7;
  sVar17 = uRam0010c764 - 0x21;
  sVar15 = sVar17;
  uVar19 = 0x21;
  sVar7 = 0;
  if (-1 < sVar17) {
    uVar18 = 7 - sVar17;
    if (uVar18 == 0 || 7 < sVar17) {
      return;
    }
    sVar15 = 0;
    uVar19 = uRam0010c764;
    sVar7 = sVar17;
  }
  sVar17 = uVar18 + sVar15 + -0x20;
  uVar11 = uVar18;
  if ((sVar17 < 0) || (uVar11 = uVar18 - sVar17, uVar11 != 0 && sVar17 <= (short)uVar18)) {
    sVar17 = sRam0010c766 + -10;
    sVar15 = 10;
    if (-1 < sVar17) {
      sVar9 = -sVar17;
      uVar12 = 7 - sVar17;
      if (uVar12 == 0 || 7 < sVar17) {
        return;
      }
      sVar17 = 0;
      sVar15 = sRam0010c766;
    }
    bVar14 = (byte)sVar15;
    sVar17 = uVar12 + sVar17 + -0x20;
    uVar18 = uVar12;
    if ((sVar17 < 0) || (uVar18 = uVar12 - sVar17, uVar18 != 0 && sVar17 <= (short)uVar12)) {
      uVar8 = (short)(sVar7 << 4) >> 4;
      uVar12 = (short)(sVar9 << 4) >> 4;
      if (((short)(uVar8 - sRam000b6bf4) < 0) && ((short)(uVar12 - uRam000b6bf6) < 0)) {
        if ((short)uVar8 < 0) {
          bVar25 = SCARRY2(uVar8,uVar11);
          uVar11 = uVar8 + uVar11;
          if (uVar11 == 0 || bVar25 != (int)((uint)uVar11 << 0x10) < 0) {
            return;
          }
          uVar19 = uVar19 - uVar8;
          uVar8 = 0;
        }
        if ((short)uVar12 < 0) {
          bVar25 = SCARRY2(uVar12,uVar18);
          uVar18 = uVar12 + uVar18;
          if (uVar18 == 0 || bVar25 != (int)((uint)uVar18 << 0x10) < 0) {
            return;
          }
          bVar14 = (char)uVar12 + bVar14;
          uVar12 = 0;
        }
        sVar17 = (uVar11 + uVar8) - sRam000b6bf4;
        uVar13 = uVar11;
        if (((sVar17 < 0) || (uVar13 = uVar11 - sVar17, uVar13 != 0 && sVar17 <= (short)uVar11)) &&
           ((sVar17 = (uVar18 + uVar12) - uRam000b6bf6, uVar11 = uVar18, sVar17 < 0 ||
            (uVar11 = uVar18 - sVar17, uVar11 != 0 && sVar17 <= (short)uVar18)))) {
          if (uRam000b6bec != 0) {
            if (uRam000b6bec == 0xffffffff) {
              sVar17 = (sRam0010c75a + 1) * 2;
              uVar18 = (uVar19 & 0x1f) * 2;
              uVar19 = uVar13;
              do {
                DAT_00300001 = (undefined1)sVar17;
                uVar10 = (byte)((~bVar14 & 0x1f) * '\x02' +
                               (*(char *)((short)uVar18 + 0x10c770) >> 2)) & 0x3e;
                uVar3 = uVar10 + (uVar18 + sVar17) * 0x20;
                uVar16 = uVar11;
                do {
                  _DAT_003c0000 = (uint)uVar3 * 0x10000 + 0x10000;
                  sVar9 = ((short)((uint)_DAT_003c0000 >> 0x10) + -1) - uVar10;
                  bVar5 = (char)uVar10 + 2;
                  uVar10 = bVar5 & 0xff3e;
                  uVar3 = CONCAT11((char)((ushort)sVar9 >> 8),(bVar5 & 0x3e) + (char)sVar9);
                  uVar16 = uVar16 - 1;
                } while (uVar16 != 0);
                uVar18 = uVar18 + 2 & 0x3e;
                uVar19 = uVar19 - 1;
              } while (uVar19 != 0);
            }
            else {
              uVar18 = (ushort)(uRam000b6bec >> 0x10);
              bVar5 = (byte)((ushort)(uVar18 << 0xc) >> 8) | (byte)(uRam000b6bec >> 0x1c);
              cVar4 = (char)(uVar18 >> 4) + -1;
              *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar5,cVar4);
              do {
              } while (cVar4 != DAT_002fffff);
              uVar18 = (ushort)CONCAT31((uint3)bVar5,2);
              if (-1 < sRam0010c75a) {
                puVar20 = (undefined2 *)
                          (((uint)uVar12 + (uint)uVar8 * (uint)uVar3) * 4 +
                          ((uint)(ushort)(uVar18 << 4 | uVar18 >> 0xc) << 0x10 | uVar6 & 0xffff));
                uVar18 = (uVar19 & 0x1f) * 2;
                uVar19 = uVar13;
                do {
                  uVar10 = (byte)((~bVar14 & 0x1f) * '\x02' +
                                 (cRam0010c7b1 + (*(char *)((short)uVar18 + 0x10c770) >> 3)) *
                                 '\x02') & 0x3e;
                  sVar17 = uVar10 + (uVar18 + (sRam0010c75a + 1) * 2) * 0x20;
                  puVar23 = puVar20;
                  uVar16 = uVar11;
                  do {
                    puVar24 = puVar23 + 1;
                    uVar2 = *puVar23;
                    puVar23 = puVar23 + 2;
                    sVar17 = (short)((uint)(CONCAT22(sVar17,uVar2) + 0x10000) >> 0x10);
                    _DAT_003c0000 = CONCAT22(sVar17,*puVar24);
                    sVar17 = (sVar17 + -1) - uVar10;
                    bVar5 = (char)uVar10 + 2;
                    uVar10 = bVar5 & 0xff3e;
                    sVar17 = CONCAT11((char)((ushort)sVar17 >> 8),(bVar5 & 0x3e) + (char)sVar17);
                    uVar16 = uVar16 - 1;
                  } while (uVar16 != 0);
                  puVar20 = (undefined2 *)((int)(short)(uVar3 << 2) + (int)puVar20);
                  uVar18 = uVar18 + 2 & 0x3e;
                  uVar19 = uVar19 - 1;
                } while (uVar19 != 0);
              }
            }
          }
          *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
          uVar18 = uRam000b6bf6;
          uVar6 = uRam000b6bf0;
          do {
          } while (DAT_002fffff != '\x01');
          if (uRam000b6bf0 != 0) {
            if (uRam000b6bf0 == 0xffffffff) {
              uVar13 = uVar13 << 7;
              do {
                DAT_00300001 = 0;
                uVar13 = uVar13 & 0xf80;
                uVar18 = sStack_e * 2;
                sVar17 = sStack_12;
                do {
                  uVar18 = uVar18 & 0x3e;
                  *(undefined2 *)((short)uVar13 + 0x10c7b6 + (int)(short)uVar18) = 0;
                  *(undefined2 *)((short)uVar13 + 0x10c7f6 + (int)(short)uVar18) = 0;
                  uVar18 = uVar18 + 2;
                  sVar17 = sVar17 + -1;
                } while (sVar17 != 0);
                uVar13 = uVar13 + 0x80;
                uVar12 = uVar12 - 1;
              } while (uVar12 != 0);
            }
            else {
              uVar19 = (ushort)(uRam000b6bf0 >> 0x10);
              bVar14 = (byte)((ushort)(uVar19 << 0xc) >> 8) | (byte)(uRam000b6bf0 >> 0x1c);
              cVar4 = (char)(uVar19 >> 4) + -1;
              *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar14,cVar4);
              do {
              } while (cVar4 != DAT_002fffff);
              uVar19 = (ushort)CONCAT31((uint3)bVar14,2);
              puVar22 = (undefined1 *)
                        (((uint)uStack_16 + (uint)uVar8 * (uint)uVar18) * 4 +
                        ((uint)(ushort)(uVar19 << 4 | uVar19 >> 0xc) << 0x10 | uVar6 & 0xffff));
              uVar13 = uVar13 << 7;
              do {
                iVar1 = (int)(short)(uVar13 & 0xf80);
                uVar19 = sStack_e * 2;
                sVar17 = sStack_12;
                do {
                  uVar19 = uVar19 & 0x3e;
                  *(undefined1 *)(iVar1 + 0x10c7b6 + (int)(short)uVar19) = *puVar22;
                  *(undefined1 *)(iVar1 + 0x10c7f6 + (int)(short)uVar19) = puVar22[1];
                  puVar21 = puVar22 + 3;
                  *(undefined1 *)(iVar1 + 0x10c7b7 + (int)(short)uVar19) = puVar22[2];
                  puVar22 = puVar22 + 4;
                  *(undefined1 *)(iVar1 + 0x10c7f7 + (int)(short)uVar19) = *puVar21;
                  uVar19 = uVar19 + 2;
                  sVar17 = sVar17 + -1;
                } while (sVar17 != 0);
                puVar22 = puVar22 + (short)((uVar18 - sStack_12) * 4);
                uVar13 = (uVar13 & 0xf80) + 0x80;
                uVar12 = uVar12 - 1;
              } while (uVar12 != 0);
            }
          }
          *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
          _DAT_002ffff0 = 1;
          do {
          } while (DAT_002fffff != '\x01');
          return;
        }
      }
      return;
    }
  }
  return;
}
