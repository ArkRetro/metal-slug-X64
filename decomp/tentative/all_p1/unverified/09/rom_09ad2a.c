/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ad2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ad2a(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  ushort extraout_D0u;
  int iVar6;
  ushort uVar7;
  undefined1 uVar8;
  int extraout_D1;
  int iVar9;
  ushort uVar11;
  undefined2 uVar12;
  short sVar13;
  undefined4 unaff_D2;
  undefined2 uVar14;
  undefined4 unaff_D3;
  ushort uVar15;
  ushort uVar16;
  byte *pbVar17;
  short *psVar18;
  short *psVar19;
  undefined1 *puVar20;
  int iVar21;
  int unaff_A5;
  int unaff_A6;
  bool bVar22;
  short sVar10;

  if (*(short *)(unaff_A6 + 0x88) != 0) {
    sVar13 = *(short *)(unaff_A6 + 0x88) + -1;
    *(short *)(unaff_A6 + 0x88) = sVar13;
    if (sVar13 == 0) {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xdf;
    }
    return;
  }
  iVar9 = *(int *)(unaff_A6 + 0x50);
  bVar22 = false;
  if ((iVar9 != 0) && (candidate_4bac(), !bVar22)) {
    iVar21 = unaff_A5 + -0x1c1c;
    if (*(char *)(unaff_A5 + -0x1c1e) < '\0') {
      iVar21 = unaff_A5 + -0x1a6a;
    }
    uVar3 = *(ushort *)(iVar21 + 0x100);
    if (uVar3 < 0x40) {
      uVar5 = ~uVar3;
      uVar16 = uVar3 >> 3 & 7;
      uVar15 = (ushort)*(byte *)(iVar9 + 1);
      psVar19 = (short *)(iVar9 + 2);
      sVar13 = *psVar19;
      iVar9 = extraout_D1;
      uVar7 = extraout_D0u;
      while (sVar13 != -0x8000) {
        sVar10 = psVar19[1];
        uVar11 = (ushort)((uint)iVar9 >> 0x10);
        psVar18 = psVar19 + 3;
        sVar1 = psVar19[2];
        uVar12 = (undefined2)((uint)unaff_D2 >> 0x10);
        unaff_D2 = CONCAT22(uVar12,sVar1);
        psVar19 = psVar19 + 4;
        sVar2 = *psVar18;
        uVar14 = (undefined2)((uint)unaff_D3 >> 0x10);
        unaff_D3 = CONCAT22(uVar14,sVar2);
        if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
          sVar13 = -(sVar1 + sVar13);
        }
        sVar13 = *(short *)(unaff_A6 + 0x3c) + sVar13;
        if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
          sVar10 = -(sVar2 + sVar10);
        }
        sVar10 = *(short *)(unaff_A6 + 0x40) + sVar10;
        iVar9 = CONCAT22(uVar11,sVar10);
        sVar4 = sVar13 + 0x20;
        iVar6 = CONCAT22(uVar7,sVar4);
        if (sVar13 < -0x20) {
          unaff_D2 = CONCAT22(uVar12,sVar4 + sVar1);
          if (SCARRY2(sVar4,sVar1) == (short)(sVar4 + sVar1) < 0) {
            iVar6 = (uint)uVar7 << 0x10;
            goto LAB_00004cba;
          }
        }
        else {
LAB_00004cba:
          uVar7 = (ushort)iVar6;
          if ((short)uVar7 < 0x180) {
            sVar13 = uVar7 + (short)unaff_D2;
            uVar12 = (undefined2)((uint)unaff_D2 >> 0x10);
            if (0x17f < sVar13) {
              sVar13 = 0x17f;
            }
            sVar13 = sVar13 - uVar7;
            unaff_D2 = CONCAT22(uVar12,sVar13);
            if (sVar13 != 0) {
              sVar1 = sVar10 + -0x108;
              iVar9 = CONCAT22(uVar11,sVar1);
              if (sVar10 < 0x108) {
                unaff_D3 = CONCAT22(uVar14,sVar1 + sVar2);
                if (SCARRY2(sVar1,sVar2) != (short)(sVar1 + sVar2) < 0) goto LAB_00004d5a;
                iVar9 = (uint)uVar11 << 0x10;
              }
              uVar11 = (ushort)iVar9;
              if ((short)uVar11 < 0xf0) {
                sVar10 = uVar11 + (short)unaff_D3;
                uVar14 = (undefined2)((uint)unaff_D3 >> 0x10);
                if (0xef < sVar10) {
                  sVar10 = 0xef;
                }
                sVar10 = sVar10 - uVar11;
                unaff_D3 = CONCAT22(uVar14,sVar10);
                if (sVar10 != 0) {
                  unaff_D2 = CONCAT22(uVar12,((ushort)(uVar7 + sVar13) >> 5) - (uVar7 >> 5));
                  unaff_D3 = CONCAT22(uVar14,((ushort)(uVar11 + sVar10) >> 5) - (uVar11 >> 5));
                  sVar13 = uVar16 + (uVar7 >> 5) * 8;
                  iVar6 = CONCAT22((short)((uint)iVar6 >> 0x10),sVar13);
                  sVar10 = uVar16 + (uVar11 >> 5) * 8;
                  iVar9 = CONCAT22((short)((uint)iVar9 >> 0x10),sVar10);
                  pbVar17 = (byte *)(iVar21 + 0x102 + (int)sVar13);
                  do {
                    *pbVar17 = *pbVar17 | '\x01' << (uVar5 & 7);
                    pbVar17 = pbVar17 + 8;
                    sVar13 = (short)unaff_D2 + -1;
                    unaff_D2 = CONCAT22((short)((uint)unaff_D2 >> 0x10),sVar13);
                  } while (sVar13 != -1);
                  pbVar17 = (byte *)(iVar21 + 0x162 + (int)sVar10);
                  do {
                    *pbVar17 = *pbVar17 | '\x01' << (uVar5 & 7);
                    pbVar17 = pbVar17 + 8;
                    sVar13 = (short)unaff_D3 + -1;
                    unaff_D3 = CONCAT22((short)((uint)unaff_D3 >> 0x10),sVar13);
                  } while (sVar13 != -1);
                  uVar15 = uVar15 | 0x8000;
                  if ((DAT_0010f000 & 0x40) != 0) {
                    candidate_4dd4(iVar6,iVar9,unaff_D2,unaff_D3,0xef);
                  }
                }
              }
            }
          }
        }
LAB_00004d5a:
        uVar7 = (ushort)((uint)iVar6 >> 0x10);
        sVar13 = *psVar19;
      }
      puVar20 = (undefined1 *)(unaff_A6 + 0x58);
      if (*(char *)(unaff_A5 + -0x1c1e) < '\0') {
        puVar20 = (undefined1 *)(unaff_A6 + 0x59);
      }
      uVar8 = 0xff;
      if ((short)uVar15 < 0) {
        if ((uVar15 & 1) != 0) {
          pbVar17 = (byte *)(iVar21 + 0x1a2 + (int)(short)uVar16);
          *pbVar17 = *pbVar17 | '\x01' << (uVar5 & 7);
        }
        if ((uVar15 & 2) != 0) {
          pbVar17 = (byte *)(iVar21 + 0x1aa + (int)(short)uVar16);
          *pbVar17 = *pbVar17 | '\x01' << (uVar5 & 7);
        }
        *(int *)(iVar21 + (short)(uVar3 * 4)) = unaff_A6;
        uVar8 = (undefined1)*(undefined2 *)(iVar21 + 0x100);
        *(short *)(iVar21 + 0x100) = *(short *)(iVar21 + 0x100) + 1;
      }
      *puVar20 = uVar8;
    }
  }
  return;
}
