/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae9c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ae9c0(void)

{
  ushort uVar1;
  short sVar5;
  ushort uVar6;
  uint uVar2;
  int iVar3;
  int iVar4;
  short extraout_D1w;
  uint uVar7;
  ushort uVar8;
  short sVar10;
  ushort uVar11;
  uint uVar9;
  short unaff_D7w;
  uint uVar12;
  int unaff_A5;
  int unaff_A6;

  uVar7 = 0;
  if ((ushort)(*(short *)(unaff_A6 + 0x3c) - 0x20U) < 0x100) {
    sVar5 = candidate_1903a();
    if (unaff_D7w != 0) {
      uVar6 = extraout_D1w - *(short *)(unaff_A6 + 0x40);
      if ((int)((uint)uVar6 << 0x10) < 0) {
        uVar6 = -uVar6;
      }
      sVar10 = 0x60 - ((short)uVar6 >> 1);
      if (0x60 < (ushort)((short)uVar6 >> 1)) {
        sVar10 = 0;
      }
      uVar6 = sVar5 - *(short *)(unaff_A6 + 0x84);
      if (-1 < (int)((uint)uVar6 << 0x10)) {
        sVar10 = -sVar10;
      }
      uVar6 = sVar10 + uVar6;
      if (*(char *)(unaff_A6 + 0x80) != '\0') {
        uVar6 = uVar6 - ((short)uVar6 >> 2);
      }
      uVar8 = -*(ushort *)(unaff_A6 + 0x40);
      if ((int)((uint)uVar8 << 0x10) < 0) {
        uVar8 = *(ushort *)(unaff_A6 + 0x40);
      }
      sVar5 = 0x60 - ((short)uVar8 >> 1);
      if (0x60 < (ushort)((short)uVar8 >> 1)) {
        sVar5 = 0;
      }
      if (-1 < (int)((uint)(ushort)-*(short *)(unaff_A6 + 0x84) << 0x10)) {
        sVar5 = -sVar5;
      }
      uVar8 = sVar5 + -*(short *)(unaff_A6 + 0x84);
      if (*(char *)(unaff_A6 + 0x80) != '\x01') {
        uVar8 = uVar8 - ((short)uVar8 >> 2);
      }
      uVar1 = uVar6;
      if ((short)uVar6 < 0) {
        uVar1 = -uVar6;
      }
      uVar11 = uVar8;
      if ((short)uVar8 < 0) {
        uVar11 = -uVar8;
      }
      if (uVar11 <= uVar1) {
        uVar8 = uVar6;
      }
      if ((ushort)(uVar8 + 0x14) < 0x28) {
        uVar8 = -uVar8;
      }
      uVar7 = (uint)uVar8 << 0x10;
    }
    uVar6 = *(ushort *)(unaff_A6 + 0x92);
    if (uVar6 != 0) {
      uVar8 = (*(short *)(unaff_A6 + 0x40) - *(short *)(unaff_A6 + 0x90)) -
              *(short *)(unaff_A5 + 0x6980);
      if ((int)((uint)uVar8 << 0x10) < 0) {
        uVar8 = -uVar8;
      }
      sVar5 = uVar6 - uVar8;
      if (uVar6 < uVar8) {
        sVar5 = 0;
      }
      uVar6 = (*(short *)(unaff_A5 + 0x697c) + *(short *)(unaff_A6 + 0x8e)) -
              *(short *)(unaff_A6 + 0x3c);
      if ((int)((uint)uVar6 << 0x10) < 0) {
        uVar6 = sVar5 + uVar6;
        if ((int)((uint)uVar6 << 0x10) < 0) {
          uVar6 = 0;
        }
      }
      else {
        uVar6 = uVar6 - sVar5;
        if (-1 < (int)((uint)uVar6 << 0x10)) {
          uVar6 = 0;
        }
      }
      uVar2 = (uint)uVar6 * 0x10000;
      uVar12 = uVar2;
      if ((int)uVar2 < 0) {
        uVar12 = (uint)uVar6 * -0x10000;
      }
      uVar9 = uVar7;
      if ((int)uVar7 < 0) {
        uVar9 = -uVar7;
      }
      if (uVar9 <= uVar12) {
        uVar7 = uVar2;
      }
    }
  }
  iVar3 = -*(int *)(unaff_A6 + 0x84);
  if (iVar3 + 0xa00000 < 0) {
    iVar3 = iVar3 + 0x1000000;
    if (-1 < iVar3) {
      iVar3 = 0;
    }
  }
  else {
    iVar3 = iVar3 + 0x400000;
    if (iVar3 < 0) {
      iVar3 = 0;
    }
  }
  uVar6 = *(short *)(unaff_A6 + 0x8c) + 0x13f;
  *(ushort *)(unaff_A6 + 0x8c) = uVar6;
  iVar4 = (uint)uVar6 * 8;
  if (-1 < *(int *)(unaff_A6 + 0x88)) {
    iVar4 = (uint)uVar6 * -8;
  }
  iVar3 = *(int *)(unaff_A6 + 0x88) +
          (((int)((iVar3 + uVar7) - *(int *)(unaff_A6 + 0x88)) >> 7) -
          (iVar4 + *(int *)(unaff_A6 + 0x88) >> 5));
  *(int *)(unaff_A6 + 0x88) = iVar3;
  *(int *)(unaff_A6 + 0x84) = iVar3 + *(int *)(unaff_A6 + 0x84);
  return;
}
