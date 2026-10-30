/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0140c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_140c0(void)

{
  undefined4 *puVar1;
  char *pcVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  ushort uVar8;
  int iVar9;
  uint uVar10;
  ushort uVar11;
  int iVar12;
  undefined4 uVar13;
  int iVar14;
  ushort uVar16;
  short sVar17;
  undefined4 uVar15;
  uint uVar18;
  short sVar19;
  int iVar20;
  undefined4 in_A0;
  int extraout_A0;
  int extraout_A0_00;
  byte *pbVar21;
  undefined4 uVar22;
  int unaff_A5;
  int unaff_A6;

  if (*(byte *)(unaff_A6 + 0x6a) == 0xff) {
    iVar20 = unaff_A5 + 0x422e;
    sVar19 = 7;
    while ((*(int *)(iVar20 + 0x26) != 0 || (*(short *)(iVar20 + 0x2a) != 0))) {
      iVar20 = iVar20 + -0x40;
      sVar19 = sVar19 + -1;
      if (sVar19 == -1) {
        return 0;
      }
    }
    *(char *)(unaff_A6 + 0x6a) = (char)sVar19;
    *(undefined4 *)(iVar20 + 0x26) = in_A0;
    *(short *)(unaff_A5 + 0x42a4) = sVar19;
    candidate_1abb6();
    iVar20 = extraout_A0;
  }
  else {
    uVar11 = (ushort)*(byte *)(unaff_A6 + 0x6a);
    if (7 < uVar11) {
      __m68k_trap(0xf);
    }
    *(ushort *)(unaff_A5 + 0x42a4) = uVar11;
    puVar1 = (undefined4 *)(unaff_A5 + 0x406e + (int)(short)(uVar11 << 6));
    *(undefined4 *)((int)puVar1 + 0x26) = in_A0;
    uVar4 = *(undefined4 *)(unaff_A6 + 0x2c);
    uVar13 = *(undefined4 *)(unaff_A6 + 0x30);
    uVar15 = *(undefined4 *)(unaff_A6 + 0x34);
    uVar22 = *(undefined4 *)(unaff_A6 + 0x38);
    *puVar1 = *(undefined4 *)(unaff_A6 + 0x28);
    puVar1[1] = uVar4;
    puVar1[2] = uVar13;
    puVar1[3] = uVar15;
    puVar1[4] = uVar22;
    candidate_1abc8();
    iVar20 = extraout_A0_00;
  }
  pcVar2 = *(char **)(iVar20 + 0x26);
  iVar9 = *(int *)(iVar20 + 0x1c);
  iVar3 = *(int *)(iVar20 + 0x20);
  iVar12 = *(int *)(iVar20 + 0x14);
  iVar14 = *(int *)(iVar20 + 0x18);
  iVar6 = iVar9;
  if (iVar12 < iVar9) {
    iVar6 = iVar12;
    iVar12 = iVar9;
  }
  iVar9 = iVar3;
  if (iVar14 < iVar3) {
    iVar9 = iVar14;
    iVar14 = iVar3;
  }
  uVar18 = (uint)(ushort)(*pcVar2 * 8) * 0x10000 + iVar6;
  uVar5 = (uint)(ushort)(pcVar2[1] * 8) * 0x10000 + iVar9;
  iVar12 = (uint)(ushort)(*pcVar2 * 8 + pcVar2[2] * 8) * 0x10000 + iVar12;
  iVar14 = (uint)(ushort)(pcVar2[1] * 8 + (pcVar2[3] + 1) * 8) * 0x10000 + iVar14;
  *(uint *)(iVar20 + 0x2c) = uVar18;
  *(uint *)(iVar20 + 0x30) = uVar5;
  *(int *)(iVar20 + 0x34) = iVar12;
  *(int *)(iVar20 + 0x38) = iVar14;
  uVar7 = iVar6 << 0x10 | uVar18 >> 0x10;
  uVar10 = iVar9 << 0x10 | uVar5 >> 0x10;
  uVar11 = (short)((uint)iVar12 >> 0x10) + 0x80;
  if (-1 < (short)uVar11) {
    sVar19 = (short)(uVar18 >> 0x10) + 0x80;
    uVar7 = CONCAT22((short)uVar18,sVar19);
    if (sVar19 < 0) {
      uVar7 = 0;
    }
    uVar16 = (short)((uint)iVar14 >> 0x10) - 0x80;
    if (-1 < (short)uVar16) {
      sVar19 = (short)(uVar5 >> 0x10) + -0x80;
      uVar10 = CONCAT22((short)uVar5,sVar19);
      if (sVar19 < 0) {
        uVar10 = 0;
      }
      if ((short)(ushort)uVar7 < 0x240) {
        if (0x23f < (short)uVar11) {
          uVar11 = 0x23f;
        }
        if ((short)(ushort)uVar10 < 0x200) {
          if (0x1ff < (short)uVar16) {
            uVar16 = 0x1ff;
          }
          uVar8 = (ushort)uVar7 >> 5;
          uVar7 = CONCAT22((short)(uVar7 >> 0x10),uVar8);
          sVar19 = (uVar11 >> 5) - uVar8;
          uVar11 = (ushort)uVar10 >> 5;
          uVar10 = CONCAT22((short)(uVar10 >> 0x10),uVar11);
          sVar17 = (uVar16 >> 5) - uVar11;
          uVar18 = (uint)*(ushort *)(unaff_A5 + 0x42a4);
          if (pcVar2[4] == '\0') {
            *(byte *)(unaff_A5 + 0x4290) = *(byte *)(unaff_A5 + 0x4290) | '\x01' << (uVar18 & 7);
          }
          pbVar21 = (byte *)(unaff_A5 + 0x426e + (int)(short)uVar8);
          do {
            *pbVar21 = *pbVar21 | '\x01' << (uVar18 & 7);
            sVar19 = sVar19 + -1;
            pbVar21 = pbVar21 + 1;
          } while (sVar19 != -1);
          pbVar21 = (byte *)(unaff_A5 + 0x4280 + (int)(short)uVar11);
          do {
            *pbVar21 = *pbVar21 | '\x01' << (uVar18 & 7);
            sVar17 = sVar17 + -1;
            pbVar21 = pbVar21 + 1;
          } while (sVar17 != -1);
        }
      }
    }
  }
  return CONCAT44(uVar7,uVar10);
}
