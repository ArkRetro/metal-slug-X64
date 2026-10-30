/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c70fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c70fc(void)

{
  uint uVar1;
  undefined2 uVar2;
  char cVar3;
  short sVar4;
  byte bVar5;
  short in_D0w;
  ushort uVar6;
  short in_D1w;
  ushort uVar7;
  ushort unaff_D2w;
  ushort uVar8;
  ushort uVar9;
  ushort uVar10;
  ushort unaff_D3w;
  ushort unaff_D4w;
  ushort uVar11;
  short unaff_D5w;
  byte bVar12;
  ushort uVar13;
  short sVar14;
  ushort uVar15;
  ushort uVar16;
  int in_A0;
  uint *in_A1;
  undefined2 *puVar17;
  undefined1 *puVar18;
  undefined1 *puVar19;
  int iVar20;
  undefined2 *puVar21;
  undefined2 *puVar22;
  int unaff_A5;
  bool bVar23;
  ushort uStack_16;
  short sStack_12;
  short sStack_e;

  sVar14 = *(short *)(in_A0 + 0x36) - unaff_D4w;
  uVar8 = unaff_D2w;
  if (-1 < sVar14) {
    unaff_D4w = sVar14 + unaff_D4w;
    in_D0w = sVar14 + in_D0w;
    uVar8 = unaff_D2w - sVar14;
    if (uVar8 == 0 || (short)unaff_D2w < sVar14) {
      return;
    }
    sVar14 = 0;
  }
  sVar14 = uVar8 + sVar14 + -0x20;
  uVar9 = uVar8;
  if ((sVar14 < 0) || (uVar9 = uVar8 - sVar14, uVar9 != 0 && sVar14 <= (short)uVar8)) {
    sVar14 = *(short *)(in_A0 + 0x38) - unaff_D5w;
    uVar8 = unaff_D3w;
    if (-1 < sVar14) {
      unaff_D5w = sVar14 + unaff_D5w;
      in_D1w = in_D1w - sVar14;
      uVar8 = unaff_D3w - sVar14;
      if (uVar8 == 0 || (short)unaff_D3w < sVar14) {
        return;
      }
      sVar14 = 0;
    }
    bVar12 = (byte)unaff_D5w;
    sVar14 = uVar8 + sVar14 + -0x20;
    uVar16 = uVar8;
    if ((sVar14 < 0) || (uVar16 = uVar8 - sVar14, uVar16 != 0 && sVar14 <= (short)uVar8)) {
      uVar6 = (short)(in_D0w << 4) >> 4;
      uVar8 = (short)(in_D1w << 4) >> 4;
      if (((short)(uVar6 - *(short *)(in_A1 + 2)) < 0) &&
         ((short)(uVar8 - *(short *)((int)in_A1 + 10)) < 0)) {
        if ((short)uVar6 < 0) {
          bVar23 = SCARRY2(uVar6,uVar9);
          uVar9 = uVar6 + uVar9;
          if (uVar9 == 0 || bVar23 != (int)((uint)uVar9 << 0x10) < 0) {
            return;
          }
          unaff_D4w = unaff_D4w - uVar6;
          uVar6 = 0;
        }
        if ((short)uVar8 < 0) {
          bVar23 = SCARRY2(uVar8,uVar16);
          uVar16 = uVar8 + uVar16;
          if (uVar16 == 0 || bVar23 != (int)((uint)uVar16 << 0x10) < 0) {
            return;
          }
          bVar12 = (char)uVar8 + bVar12;
          uVar8 = 0;
        }
        sVar14 = (uVar9 + uVar6) - *(short *)(in_A1 + 2);
        uVar11 = uVar9;
        if (((sVar14 < 0) || (uVar11 = uVar9 - sVar14, uVar11 != 0 && sVar14 <= (short)uVar9)) &&
           ((sVar14 = (uVar16 + uVar8) - *(short *)((int)in_A1 + 10), uVar9 = uVar16, sVar14 < 0 ||
            (uVar9 = uVar16 - sVar14, uVar9 != 0 && sVar14 <= (short)uVar16)))) {
          uVar16 = *(ushort *)((int)in_A1 + 10);
          uVar1 = *in_A1;
          if (uVar1 != 0) {
            if (uVar1 == 0xffffffff) {
              sVar14 = (*(short *)(in_A0 + 0x2c) + 1) * 2;
              uVar16 = (unaff_D4w & 0x1f) * 2;
              uVar15 = uVar11;
              do {
                DAT_00300001 = (undefined1)sVar14;
                uVar7 = (byte)((~bVar12 & 0x1f) * '\x02' +
                              (*(char *)(in_A0 + 0x42 + (int)(short)uVar16) >> 2)) & 0x3e;
                uVar10 = uVar7 + (uVar16 + sVar14) * 0x20;
                uVar13 = uVar9;
                do {
                  _DAT_003c0000 = (uint)uVar10 * 0x10000 + 0x10000;
                  sVar4 = ((short)((uint)_DAT_003c0000 >> 0x10) + -1) - uVar7;
                  bVar5 = (char)uVar7 + 2;
                  uVar7 = bVar5 & 0xff3e;
                  uVar10 = CONCAT11((char)((ushort)sVar4 >> 8),(bVar5 & 0x3e) + (char)sVar4);
                  uVar13 = uVar13 - 1;
                } while (uVar13 != 0);
                uVar16 = uVar16 + 2 & 0x3e;
                uVar15 = uVar15 - 1;
              } while (uVar15 != 0);
            }
            else {
              uVar15 = (ushort)(uVar1 >> 0x10);
              bVar5 = (byte)((ushort)(uVar15 << 0xc) >> 8) | (byte)(uVar1 >> 0x1c);
              cVar3 = (char)(uVar15 >> 4) + -1;
              *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar5,cVar3);
              do {
              } while (cVar3 != DAT_002fffff);
              uVar15 = (ushort)CONCAT31((uint3)bVar5,2);
              if (-1 < *(short *)(in_A0 + 0x2c)) {
                puVar17 = (undefined2 *)
                          (((uint)uVar8 + (uint)uVar6 * (uint)uVar16) * 4 +
                          ((uint)(ushort)(uVar15 << 4 | uVar15 >> 0xc) << 0x10 | uVar1 & 0xffff));
                uVar15 = (unaff_D4w & 0x1f) * 2;
                uVar10 = uVar11;
                do {
                  uVar7 = (byte)((~bVar12 & 0x1f) * '\x02' +
                                (*(char *)(in_A0 + 0x83) +
                                (*(char *)(in_A0 + 0x42 + (int)(short)uVar15) >> 3)) * '\x02') &
                          0x3e;
                  sVar14 = uVar7 + (uVar15 + (*(short *)(in_A0 + 0x2c) + 1) * 2) * 0x20;
                  puVar21 = puVar17;
                  uVar13 = uVar9;
                  do {
                    puVar22 = puVar21 + 1;
                    uVar2 = *puVar21;
                    puVar21 = puVar21 + 2;
                    sVar14 = (short)((uint)(CONCAT22(sVar14,uVar2) + 0x10000) >> 0x10);
                    _DAT_003c0000 = CONCAT22(sVar14,*puVar22);
                    sVar14 = (sVar14 + -1) - uVar7;
                    bVar5 = (char)uVar7 + 2;
                    uVar7 = bVar5 & 0xff3e;
                    sVar14 = CONCAT11((char)((ushort)sVar14 >> 8),(bVar5 & 0x3e) + (char)sVar14);
                    uVar13 = uVar13 - 1;
                  } while (uVar13 != 0);
                  puVar17 = (undefined2 *)((int)(short)(uVar16 << 2) + (int)puVar17);
                  uVar15 = uVar15 + 2 & 0x3e;
                  uVar10 = uVar10 - 1;
                } while (uVar10 != 0);
              }
            }
          }
          *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
          do {
          } while (DAT_002fffff != '\x01');
          uVar1 = in_A1[1];
          if (uVar1 != 0) {
            if (uVar1 == 0xffffffff) {
              uVar11 = uVar11 << 7;
              do {
                DAT_00300001 = 0;
                iVar20 = (int)(short)(uVar11 & 0xf80) + in_A0 + 0x88;
                uVar9 = sStack_e * 2;
                sVar14 = sStack_12;
                do {
                  uVar9 = uVar9 & 0x3e;
                  *(undefined2 *)(iVar20 + (short)uVar9) = 0;
                  *(undefined2 *)(iVar20 + 0x40 + (int)(short)uVar9) = 0;
                  uVar9 = uVar9 + 2;
                  sVar14 = sVar14 + -1;
                } while (sVar14 != 0);
                uVar11 = (uVar11 & 0xf80) + 0x80;
                uVar8 = uVar8 - 1;
              } while (uVar8 != 0);
            }
            else {
              uVar9 = *(ushort *)((int)in_A1 + 10);
              uVar16 = (ushort)(uVar1 >> 0x10);
              bVar12 = (byte)((ushort)(uVar16 << 0xc) >> 8) | (byte)(uVar1 >> 0x1c);
              cVar3 = (char)(uVar16 >> 4) + -1;
              *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar12,cVar3);
              do {
              } while (cVar3 != DAT_002fffff);
              uVar16 = (ushort)CONCAT31((uint3)bVar12,2);
              puVar19 = (undefined1 *)
                        (((uint)uStack_16 + (uint)uVar6 * (uint)uVar9) * 4 +
                        ((uint)(ushort)(uVar16 << 4 | uVar16 >> 0xc) << 0x10 | uVar1 & 0xffff));
              uVar11 = uVar11 << 7;
              do {
                iVar20 = in_A0 + 0x88 + (int)(short)(uVar11 & 0xf80);
                uVar16 = sStack_e * 2;
                sVar14 = sStack_12;
                do {
                  uVar16 = uVar16 & 0x3e;
                  *(undefined1 *)(iVar20 + (short)uVar16) = *puVar19;
                  *(undefined1 *)(iVar20 + 0x40 + (int)(short)uVar16) = puVar19[1];
                  puVar18 = puVar19 + 3;
                  *(undefined1 *)(iVar20 + 1 + (int)(short)uVar16) = puVar19[2];
                  puVar19 = puVar19 + 4;
                  *(undefined1 *)(iVar20 + 0x41 + (int)(short)uVar16) = *puVar18;
                  uVar16 = uVar16 + 2;
                  sVar14 = sVar14 + -1;
                } while (sVar14 != 0);
                puVar19 = puVar19 + (short)((uVar9 - sStack_12) * 4);
                uVar11 = (uVar11 & 0xf80) + 0x80;
                uVar8 = uVar8 - 1;
              } while (uVar8 != 0);
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
