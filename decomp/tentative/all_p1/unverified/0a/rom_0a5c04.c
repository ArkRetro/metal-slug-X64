/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5c04.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000a5cae: Changing call to branch */
/* WARNING: Possible PIC construction at 0x000a5cc0: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000a5cb2) */
/* WARNING: Removing unreachable block (ram,0x000a5cc4) */
/* WARNING: Removing unreachable block (ram,0x000a5cce) */
/* WARNING: Removing unreachable block (ram,0x000a5cd4) */
/* WARNING: Removing unreachable block (ram,0x000a5cfa) */
/* WARNING: Removing unreachable block (ram,0x000a5cdc) */
/* WARNING: Removing unreachable block (ram,0x000a5ce2) */
/* WARNING: Removing unreachable block (ram,0x000a5cf6) */
/* WARNING: Removing unreachable block (ram,0x000c716c) */
/* WARNING: Removing unreachable block (ram,0x000c7172) */
/* WARNING: Removing unreachable block (ram,0x000c717c) */
/* WARNING: Removing unreachable block (ram,0x000c7182) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_a5c04(void)

{
  uint *puVar1;
  uint uVar2;
  undefined2 uVar3;
  char cVar4;
  char cVar5;
  short sVar6;
  byte bVar7;
  short in_D0w;
  int iVar8;
  short sVar10;
  ushort uVar11;
  ushort uVar12;
  ushort uVar13;
  ushort uVar14;
  short sVar15;
  ushort uVar16;
  ushort uVar17;
  int extraout_A0;
  int extraout_A0_00;
  char *pcVar18;
  undefined2 *puVar19;
  undefined1 *puVar20;
  undefined1 *puVar21;
  undefined2 *puVar22;
  undefined2 *puVar23;
  int unaff_A5;
  int unaff_A6;
  bool bVar24;
  undefined2 uStack_4;
  short sStack_2;
  ushort uVar9;

  uStack_4 = 0x5c0a;
  sStack_2 = in_D0w;
  candidate_a6e88();
  uStack_4 = 0x5c14;
  candidate_120f8();
  if (sStack_2 == 0) {
    bVar24 = false;
    uStack_4 = 0x5c2c;
    candidate_40bc();
    if (bVar24) {
      *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    }
    *(byte *)(extraout_A0 + 0x15) = *(byte *)(extraout_A0 + 0x15) & 0xfe;
  }
  if (sStack_2 == 10) {
    bVar24 = false;
    uStack_4 = 0x5c58;
    candidate_40bc();
    if (bVar24) {
      *(undefined4 *)(extraout_A0_00 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    }
    *(byte *)(extraout_A0_00 + 0x15) = *(byte *)(extraout_A0_00 + 0x15) | 1;
  }
  pcVar18 = (char *)0x2a634f;
  do {
    if (*pcVar18 == -1) {
      pcVar18 = (char *)0x2a6354;
      goto LAB_000a5c8c;
    }
    cVar4 = *pcVar18;
    pcVar18 = pcVar18 + 1;
  } while ((char)sStack_2 != cVar4);
  uVar9 = 8;
  register0x0000003c = (BADSPACEBASE *)&uStack_4;
  goto SUB_000a6d7c;
  while (cVar4 = *pcVar18, pcVar18 = pcVar18 + 1, (char)sStack_2 != cVar4) {
LAB_000a5c8c:
    if (*pcVar18 == -1) {
      uVar9 = sStack_2 + 10;
      goto SUB_000a6d7c;
    }
  }
  uVar9 = 9;
  register0x0000003c = (BADSPACEBASE *)&uStack_4;
SUB_000a6d7c:
  iVar8 = (uint)uVar9 * 10;
  puVar1 = *(uint **)(iVar8 + 0x2a6210);
  uVar11 = (ushort)*(byte *)(iVar8 + 0x2a6216);
  uVar12 = (ushort)*(byte *)(iVar8 + 0x2a6217);
  uVar9 = (short)((ushort)*(byte *)(iVar8 + 0x2a6214) << 4) >> 4;
  uVar14 = (short)((ushort)*(byte *)(iVar8 + 0x2a6215) << 4) >> 4;
  if (((((short)(uVar9 - *(short *)(puVar1 + 2)) < 0) &&
       ((short)(uVar14 - *(short *)((int)puVar1 + 10)) < 0)) &&
      ((sVar15 = (uVar11 + uVar9) - *(short *)(puVar1 + 2), uVar17 = uVar11, sVar15 < 0 ||
       (uVar17 = uVar11 - sVar15, uVar17 != 0 && sVar15 <= (short)uVar11)))) &&
     ((sVar15 = (uVar12 + uVar14) - *(short *)((int)puVar1 + 10), uVar11 = uVar12, sVar15 < 0 ||
      (uVar11 = uVar12 - sVar15, uVar11 != 0 && sVar15 <= (short)uVar12)))) {
    uVar13 = *(byte *)(iVar8 + 0x2a6218) & 0x1f;
    uVar16 = ~(ushort)*(byte *)(iVar8 + 0x2a6219) & 0x1f;
    *(ushort *)((int)register0x0000003c + -4) = uVar16;
    *(ushort *)((int)register0x0000003c + -8) = uVar13;
    *(ushort *)((int)register0x0000003c + -0xc) = uVar11;
    *(ushort *)((int)register0x0000003c + -0x10) = uVar17;
    *(ushort *)((int)register0x0000003c + -0x14) = uVar14;
    *(ushort *)((int)register0x0000003c + -0x18) = uVar9;
    uVar12 = *(ushort *)((int)puVar1 + 10);
    uVar2 = *puVar1;
    if (uVar2 != 0) {
      cVar4 = (char)uVar16;
      if (uVar2 == 0xffffffff) {
        sVar15 = (sRam0010e8f2 + 1) * 2;
        uVar13 = uVar13 * 2;
        do {
          DAT_00300001 = (undefined1)sVar15;
          uVar14 = (byte)(cVar4 * '\x02' + (*(char *)((short)uVar13 + 0x10e908) >> 2)) & 0x3e;
          uVar9 = uVar14 + (uVar13 + sVar15) * 0x20;
          uVar12 = uVar11;
          do {
            _DAT_003c0000 = (uint)uVar9 * 0x10000 + 0x10000;
            sVar10 = ((short)((uint)_DAT_003c0000 >> 0x10) + -1) - uVar14;
            bVar7 = (char)uVar14 + 2;
            uVar14 = bVar7 & 0xff3e;
            uVar9 = CONCAT11((char)((ushort)sVar10 >> 8),(bVar7 & 0x3e) + (char)sVar10);
            uVar12 = uVar12 - 1;
          } while (uVar12 != 0);
          uVar13 = uVar13 + 2 & 0x3e;
          uVar17 = uVar17 - 1;
        } while (uVar17 != 0);
      }
      else {
        uVar16 = (ushort)(uVar2 >> 0x10);
        bVar7 = (byte)((ushort)(uVar16 << 0xc) >> 8) | (byte)(uVar2 >> 0x1c);
        cVar5 = (char)(uVar16 >> 4) + -1;
        *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar7,cVar5);
        do {
        } while (cVar5 != DAT_002fffff);
        uVar16 = (ushort)CONCAT31((uint3)bVar7,2);
        if (-1 < sRam0010e8f2) {
          puVar19 = (undefined2 *)
                    (((uint)uVar14 + (uint)uVar9 * (uint)uVar12) * 4 +
                    ((uint)(ushort)(uVar16 << 4 | uVar16 >> 0xc) << 0x10 | uVar2 & 0xffff));
          uVar13 = uVar13 * 2;
          do {
            uVar9 = (byte)(cVar4 * '\x02' +
                          (cRam0010e949 + (*(char *)((short)uVar13 + 0x10e908) >> 3)) * '\x02') &
                    0x3e;
            sVar15 = uVar9 + (uVar13 + (sRam0010e8f2 + 1) * 2) * 0x20;
            puVar22 = puVar19;
            uVar14 = uVar11;
            do {
              puVar23 = puVar22 + 1;
              uVar3 = *puVar22;
              puVar22 = puVar22 + 2;
              sVar15 = (short)((uint)(CONCAT22(sVar15,uVar3) + 0x10000) >> 0x10);
              _DAT_003c0000 = CONCAT22(sVar15,*puVar23);
              sVar15 = (sVar15 + -1) - uVar9;
              bVar7 = (char)uVar9 + 2;
              uVar9 = bVar7 & 0xff3e;
              sVar15 = CONCAT11((char)((ushort)sVar15 >> 8),(bVar7 & 0x3e) + (char)sVar15);
              uVar14 = uVar14 - 1;
            } while (uVar14 != 0);
            puVar19 = (undefined2 *)((int)(short)(uVar12 << 2) + (int)puVar19);
            uVar13 = uVar13 + 2 & 0x3e;
            uVar17 = uVar17 - 1;
          } while (uVar17 != 0);
        }
      }
    }
    uVar14 = *(ushort *)((int)register0x0000003c + -0x18);
    uVar11 = *(ushort *)((int)register0x0000003c + -0x16);
    uVar9 = *(ushort *)((int)register0x0000003c + -0x14);
    sVar15 = *(short *)((int)register0x0000003c + -0x12);
    sVar10 = *(short *)((int)register0x0000003c + -0x10);
    sVar6 = *(short *)((int)register0x0000003c + -0xe);
    *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
    do {
    } while (DAT_002fffff != '\x01');
    uVar2 = puVar1[1];
    if (uVar2 != 0) {
      if (uVar2 == 0xffffffff) {
        uVar14 = sVar10 << 7;
        do {
          DAT_00300001 = 0;
          uVar14 = uVar14 & 0xf80;
          uVar11 = sVar6 * 2;
          sVar10 = sVar15;
          do {
            uVar11 = uVar11 & 0x3e;
            *(undefined2 *)((short)uVar14 + 0x10e94e + (int)(short)uVar11) = 0;
            *(undefined2 *)((short)uVar14 + 0x10e98e + (int)(short)uVar11) = 0;
            uVar11 = uVar11 + 2;
            sVar10 = sVar10 + -1;
          } while (sVar10 != 0);
          uVar14 = uVar14 + 0x80;
          uVar9 = uVar9 - 1;
        } while (uVar9 != 0);
      }
      else {
        uVar12 = *(ushort *)((int)puVar1 + 10);
        uVar17 = (ushort)(uVar2 >> 0x10);
        bVar7 = (byte)((ushort)(uVar17 << 0xc) >> 8) | (byte)(uVar2 >> 0x1c);
        cVar4 = (char)(uVar17 >> 4) + -1;
        *(ushort *)(unaff_A5 + -0x7fec) = CONCAT11(bVar7,cVar4);
        do {
        } while (cVar4 != DAT_002fffff);
        uVar17 = (ushort)CONCAT31((uint3)bVar7,2);
        puVar21 = (undefined1 *)
                  (((uint)uVar11 + (uint)uVar14 * (uint)uVar12) * 4 +
                  ((uint)(ushort)(uVar17 << 4 | uVar17 >> 0xc) << 0x10 | uVar2 & 0xffff));
        uVar14 = sVar10 << 7;
        do {
          iVar8 = (int)(short)(uVar14 & 0xf80);
          uVar11 = sVar6 * 2;
          sVar10 = sVar15;
          do {
            uVar11 = uVar11 & 0x3e;
            *(undefined1 *)(iVar8 + 0x10e94e + (int)(short)uVar11) = *puVar21;
            *(undefined1 *)(iVar8 + 0x10e98e + (int)(short)uVar11) = puVar21[1];
            puVar20 = puVar21 + 3;
            *(undefined1 *)(iVar8 + 0x10e94f + (int)(short)uVar11) = puVar21[2];
            puVar21 = puVar21 + 4;
            *(undefined1 *)(iVar8 + 0x10e98f + (int)(short)uVar11) = *puVar20;
            uVar11 = uVar11 + 2;
            sVar10 = sVar10 + -1;
          } while (sVar10 != 0);
          puVar21 = puVar21 + (short)((uVar12 - sVar15) * 4);
          uVar14 = (uVar14 & 0xf80) + 0x80;
          uVar9 = uVar9 - 1;
        } while (uVar9 != 0);
      }
    }
    *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
    _DAT_002ffff0 = 1;
    do {
    } while (DAT_002fffff != '\x01');
    return;
  }
  return;
}
