/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c741a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c741a(void)

{
  uint uVar1;
  undefined2 uVar2;
  char cVar3;
  short sVar4;
  byte bVar5;
  ushort uVar6;
  short sVar7;
  ushort uVar8;
  short extraout_D1w;
  ushort uVar9;
  ushort uVar10;
  ushort uVar11;
  ushort uVar12;
  byte bVar13;
  ushort uVar14;
  ushort uVar15;
  ushort uVar16;
  int in_A0;
  int extraout_A0;
  uint *extraout_A1;
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

  if (*(int *)(in_A0 + 0x2e) == 0) {
    return;
  }
  uVar16 = (ushort)(-0x400000 - *(int *)(in_A0 + 0x14) >> 0x14);
  *(ushort *)(in_A0 + 0x36) = uVar16;
  *(ushort *)(in_A0 + 0x3a) = uVar16;
  sVar7 = (short)(0x800000 - *(int *)(in_A0 + 0x18) >> 0x14);
  *(short *)(in_A0 + 0x38) = sVar7;
  *(short *)(in_A0 + 0x3c) = sVar7;
  bVar13 = (char)sVar7 + 0x1f;
  uVar11 = 0x20;
  uVar12 = 0x20;
  sVar7 = candidate_c6bde();
  DAT_00300001 = (undefined1)sVar7;
  uVar6 = (short)(sVar7 << 4) >> 4;
  uVar10 = (short)(extraout_D1w << 4) >> 4;
  if (((short)(uVar6 - *(short *)(extraout_A1 + 2)) < 0) &&
     ((short)(uVar10 - *(short *)((int)extraout_A1 + 10)) < 0)) {
    if ((short)uVar6 < 0) {
      uVar11 = uVar6 + 0x20;
      if (uVar11 == 0 || SCARRY2(uVar6,0x20) != (int)((uint)uVar11 << 0x10) < 0) {
        return;
      }
      uVar16 = uVar16 - uVar6;
      uVar6 = 0;
    }
    if ((short)uVar10 < 0) {
      uVar12 = uVar10 + 0x20;
      if (uVar12 == 0 || SCARRY2(uVar10,0x20) != (int)((uint)uVar12 << 0x10) < 0) {
        return;
      }
      bVar13 = (char)uVar10 + bVar13;
      uVar10 = 0;
    }
    uVar9 = (uVar11 + uVar6) - *(short *)(extraout_A1 + 2);
    if ((((int)((uint)uVar9 << 0x10) < 0) ||
        (bVar23 = SBORROW2(uVar11,uVar9), uVar11 = uVar11 - uVar9,
        uVar11 != 0 && bVar23 == (int)((uint)uVar11 << 0x10) < 0)) &&
       ((uVar9 = (uVar12 + uVar10) - *(short *)((int)extraout_A1 + 10),
        (int)((uint)uVar9 << 0x10) < 0 ||
        (bVar23 = SBORROW2(uVar12,uVar9), uVar12 = uVar12 - uVar9,
        uVar12 != 0 && bVar23 == (int)((uint)uVar12 << 0x10) < 0)))) {
      uVar9 = *(ushort *)((int)extraout_A1 + 10);
      uVar1 = *extraout_A1;
      if (uVar1 != 0) {
        if (uVar1 == 0xffffffff) {
          sVar7 = (*(short *)(extraout_A0 + 0x2c) + 1) * 2;
          uVar16 = (uVar16 & 0x1f) * 2;
          uVar9 = uVar11;
          do {
            DAT_00300001 = (undefined1)sVar7;
            uVar8 = (byte)((~bVar13 & 0x1f) * '\x02' +
                          (*(char *)(extraout_A0 + 0x42 + (int)(short)uVar16) >> 2)) & 0x3e;
            uVar15 = uVar8 + (uVar16 + sVar7) * 0x20;
            uVar14 = uVar12;
            do {
              _DAT_003c0000 = (uint)uVar15 * 0x10000 + 0x10000;
              sVar4 = ((short)((uint)_DAT_003c0000 >> 0x10) + -1) - uVar8;
              bVar5 = (char)uVar8 + 2;
              uVar8 = bVar5 & 0xff3e;
              uVar15 = CONCAT11((char)((ushort)sVar4 >> 8),(bVar5 & 0x3e) + (char)sVar4);
              uVar14 = uVar14 - 1;
            } while (uVar14 != 0);
            uVar16 = uVar16 + 2 & 0x3e;
            uVar9 = uVar9 - 1;
          } while (uVar9 != 0);
        }
        else {
          uVar15 = (ushort)(uVar1 >> 0x10);
          bVar5 = (byte)((ushort)(uVar15 << 0xc) >> 8) | (byte)(uVar1 >> 0x1c);
          cVar3 = (char)(uVar15 >> 4) + -1;
          *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar5,cVar3);
          do {
          } while (cVar3 != DAT_002fffff);
          uVar15 = (ushort)CONCAT31((uint3)bVar5,2);
          if (-1 < *(short *)(extraout_A0 + 0x2c)) {
            puVar17 = (undefined2 *)
                      (((uint)uVar10 + (uint)uVar6 * (uint)uVar9) * 4 +
                      ((uint)(ushort)(uVar15 << 4 | uVar15 >> 0xc) << 0x10 | uVar1 & 0xffff));
            uVar16 = (uVar16 & 0x1f) * 2;
            uVar15 = uVar11;
            do {
              uVar8 = (byte)((~bVar13 & 0x1f) * '\x02' +
                            (*(char *)(extraout_A0 + 0x83) +
                            (*(char *)(extraout_A0 + 0x42 + (int)(short)uVar16) >> 3)) * '\x02') &
                      0x3e;
              sVar7 = uVar8 + (uVar16 + (*(short *)(extraout_A0 + 0x2c) + 1) * 2) * 0x20;
              puVar21 = puVar17;
              uVar14 = uVar12;
              do {
                puVar22 = puVar21 + 1;
                uVar2 = *puVar21;
                puVar21 = puVar21 + 2;
                sVar7 = (short)((uint)(CONCAT22(sVar7,uVar2) + 0x10000) >> 0x10);
                _DAT_003c0000 = CONCAT22(sVar7,*puVar22);
                sVar7 = (sVar7 + -1) - uVar8;
                bVar5 = (char)uVar8 + 2;
                uVar8 = bVar5 & 0xff3e;
                sVar7 = CONCAT11((char)((ushort)sVar7 >> 8),(bVar5 & 0x3e) + (char)sVar7);
                uVar14 = uVar14 - 1;
              } while (uVar14 != 0);
              puVar17 = (undefined2 *)((int)(short)(uVar9 << 2) + (int)puVar17);
              uVar16 = uVar16 + 2 & 0x3e;
              uVar15 = uVar15 - 1;
            } while (uVar15 != 0);
          }
        }
      }
      *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
      do {
      } while (DAT_002fffff != '\x01');
      uVar1 = extraout_A1[1];
      if (uVar1 != 0) {
        if (uVar1 == 0xffffffff) {
          uVar11 = uVar11 << 7;
          do {
            DAT_00300001 = 0;
            iVar20 = (int)(short)(uVar11 & 0xf80) + extraout_A0 + 0x88;
            uVar12 = sStack_e * 2;
            sVar7 = sStack_12;
            do {
              uVar12 = uVar12 & 0x3e;
              *(undefined2 *)(iVar20 + (short)uVar12) = 0;
              *(undefined2 *)(iVar20 + 0x40 + (int)(short)uVar12) = 0;
              uVar12 = uVar12 + 2;
              sVar7 = sVar7 + -1;
            } while (sVar7 != 0);
            uVar11 = (uVar11 & 0xf80) + 0x80;
            uVar10 = uVar10 - 1;
          } while (uVar10 != 0);
        }
        else {
          uVar12 = *(ushort *)((int)extraout_A1 + 10);
          uVar16 = (ushort)(uVar1 >> 0x10);
          bVar13 = (byte)((ushort)(uVar16 << 0xc) >> 8) | (byte)(uVar1 >> 0x1c);
          cVar3 = (char)(uVar16 >> 4) + -1;
          *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar13,cVar3);
          do {
          } while (cVar3 != DAT_002fffff);
          uVar16 = (ushort)CONCAT31((uint3)bVar13,2);
          puVar19 = (undefined1 *)
                    (((uint)uStack_16 + (uint)uVar6 * (uint)uVar12) * 4 +
                    ((uint)(ushort)(uVar16 << 4 | uVar16 >> 0xc) << 0x10 | uVar1 & 0xffff));
          uVar11 = uVar11 << 7;
          do {
            iVar20 = extraout_A0 + 0x88 + (int)(short)(uVar11 & 0xf80);
            uVar16 = sStack_e * 2;
            sVar7 = sStack_12;
            do {
              uVar16 = uVar16 & 0x3e;
              *(undefined1 *)(iVar20 + (short)uVar16) = *puVar19;
              *(undefined1 *)(iVar20 + 0x40 + (int)(short)uVar16) = puVar19[1];
              puVar18 = puVar19 + 3;
              *(undefined1 *)(iVar20 + 1 + (int)(short)uVar16) = puVar19[2];
              puVar19 = puVar19 + 4;
              *(undefined1 *)(iVar20 + 0x41 + (int)(short)uVar16) = *puVar18;
              uVar16 = uVar16 + 2;
              sVar7 = sVar7 + -1;
            } while (sVar7 != 0);
            puVar19 = puVar19 + (short)((uVar12 - sStack_12) * 4);
            uVar11 = (uVar11 & 0xf80) + 0x80;
            uVar10 = uVar10 - 1;
          } while (uVar10 != 0);
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
