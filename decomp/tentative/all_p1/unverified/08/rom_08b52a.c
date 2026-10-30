/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b52a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_8b52a(void)

{
  int *piVar1;
  char *pcVar2;
  undefined4 uVar3;
  char cVar4;
  char cVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  ushort uVar9;
  undefined4 in_D0;
  uint uVar10;
  ushort uVar11;
  undefined4 in_D1;
  int iVar12;
  int iVar13;
  ushort uVar15;
  short sVar16;
  int iVar14;
  uint uVar17;
  short sVar18;
  int iVar19;
  int extraout_A0;
  int extraout_A0_00;
  byte *pbVar20;
  int iVar21;
  int unaff_A5;
  int unaff_A6;

  uVar3 = *(undefined4 *)
           ((short)((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x7e) & 0x3f) << 2) + 0xe53e6);
  cVar4 = *(char *)(unaff_A6 + 0x82);
  cVar5 = *(char *)(unaff_A6 + 0x83);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    cVar4 = -cVar4;
  }
  if (*(byte *)(unaff_A6 + 0x6a) == 0xff) {
    iVar19 = unaff_A5 + 0x422e;
    sVar18 = 7;
    while ((*(int *)(iVar19 + 0x26) != 0 || (*(short *)(iVar19 + 0x2a) != 0))) {
      iVar19 = iVar19 + -0x40;
      sVar18 = sVar18 + -1;
      if (sVar18 == -1) {
        return CONCAT44(CONCAT22((short)((uint)in_D0 >> 0x10),(short)cVar4),
                        CONCAT22((short)((uint)in_D1 >> 0x10),(short)cVar5));
      }
    }
    *(char *)(unaff_A6 + 0x6a) = (char)sVar18;
    *(undefined4 *)(iVar19 + 0x26) = uVar3;
    *(short *)(unaff_A5 + 0x42a4) = sVar18;
    candidate_1abb6();
    iVar19 = extraout_A0;
  }
  else {
    uVar11 = (ushort)*(byte *)(unaff_A6 + 0x6a);
    if (7 < uVar11) {
      __m68k_trap(0xf);
    }
    *(ushort *)(unaff_A5 + 0x42a4) = uVar11;
    piVar1 = (int *)(unaff_A5 + 0x406e + (int)(short)(uVar11 << 6));
    *(undefined4 *)((int)piVar1 + 0x26) = uVar3;
    iVar19 = *(int *)(unaff_A6 + 0x2c);
    iVar12 = *(int *)(unaff_A6 + 0x30);
    iVar14 = *(int *)(unaff_A6 + 0x34);
    iVar21 = *(int *)(unaff_A6 + 0x38);
    *piVar1 = *(int *)(unaff_A6 + 0x28) + (uint)(ushort)(short)cVar4 * 0x10000;
    piVar1[1] = iVar19 + (uint)(ushort)(short)cVar5 * 0x10000;
    piVar1[2] = iVar12;
    piVar1[3] = iVar14;
    piVar1[4] = iVar21;
    candidate_1abc8();
    iVar19 = extraout_A0_00;
  }
  pcVar2 = *(char **)(iVar19 + 0x26);
  iVar12 = *(int *)(iVar19 + 0x1c);
  iVar14 = *(int *)(iVar19 + 0x20);
  iVar21 = *(int *)(iVar19 + 0x14);
  iVar13 = *(int *)(iVar19 + 0x18);
  iVar7 = iVar12;
  if (iVar21 < iVar12) {
    iVar7 = iVar21;
    iVar21 = iVar12;
  }
  iVar12 = iVar14;
  if (iVar13 < iVar14) {
    iVar12 = iVar13;
    iVar13 = iVar14;
  }
  uVar17 = (uint)(ushort)(*pcVar2 * 8) * 0x10000 + iVar7;
  uVar6 = (uint)(ushort)(pcVar2[1] * 8) * 0x10000 + iVar12;
  iVar21 = (uint)(ushort)(*pcVar2 * 8 + pcVar2[2] * 8) * 0x10000 + iVar21;
  iVar13 = (uint)(ushort)(pcVar2[1] * 8 + (pcVar2[3] + 1) * 8) * 0x10000 + iVar13;
  *(uint *)(iVar19 + 0x2c) = uVar17;
  *(uint *)(iVar19 + 0x30) = uVar6;
  *(int *)(iVar19 + 0x34) = iVar21;
  *(int *)(iVar19 + 0x38) = iVar13;
  uVar8 = iVar7 << 0x10 | uVar17 >> 0x10;
  uVar10 = iVar12 << 0x10 | uVar6 >> 0x10;
  uVar11 = (short)((uint)iVar21 >> 0x10) + 0x80;
  if (-1 < (short)uVar11) {
    sVar18 = (short)(uVar17 >> 0x10) + 0x80;
    uVar8 = CONCAT22((short)uVar17,sVar18);
    if (sVar18 < 0) {
      uVar8 = 0;
    }
    uVar15 = (short)((uint)iVar13 >> 0x10) - 0x80;
    if (-1 < (short)uVar15) {
      sVar18 = (short)(uVar6 >> 0x10) + -0x80;
      uVar10 = CONCAT22((short)uVar6,sVar18);
      if (sVar18 < 0) {
        uVar10 = 0;
      }
      if ((short)(ushort)uVar8 < 0x240) {
        if (0x23f < (short)uVar11) {
          uVar11 = 0x23f;
        }
        if ((short)(ushort)uVar10 < 0x200) {
          if (0x1ff < (short)uVar15) {
            uVar15 = 0x1ff;
          }
          uVar9 = (ushort)uVar8 >> 5;
          uVar8 = CONCAT22((short)(uVar8 >> 0x10),uVar9);
          sVar18 = (uVar11 >> 5) - uVar9;
          uVar11 = (ushort)uVar10 >> 5;
          uVar10 = CONCAT22((short)(uVar10 >> 0x10),uVar11);
          sVar16 = (uVar15 >> 5) - uVar11;
          uVar17 = (uint)*(ushort *)(unaff_A5 + 0x42a4);
          if (pcVar2[4] == '\0') {
            *(byte *)(unaff_A5 + 0x4290) = *(byte *)(unaff_A5 + 0x4290) | '\x01' << (uVar17 & 7);
          }
          pbVar20 = (byte *)(unaff_A5 + 0x426e + (int)(short)uVar9);
          do {
            *pbVar20 = *pbVar20 | '\x01' << (uVar17 & 7);
            sVar18 = sVar18 + -1;
            pbVar20 = pbVar20 + 1;
          } while (sVar18 != -1);
          pbVar20 = (byte *)(unaff_A5 + 0x4280 + (int)(short)uVar11);
          do {
            *pbVar20 = *pbVar20 | '\x01' << (uVar17 & 7);
            sVar16 = sVar16 + -1;
            pbVar20 = pbVar20 + 1;
          } while (sVar16 != -1);
        }
      }
    }
  }
  return CONCAT44(uVar8,uVar10);
}
