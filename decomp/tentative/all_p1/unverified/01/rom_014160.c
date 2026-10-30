/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x014160.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_14160(void)

{
  int in_D0;
  int iVar1;
  int iVar2;
  ushort uVar3;
  int in_D1;
  int iVar4;
  ushort uVar5;
  int unaff_D2;
  int iVar6;
  ushort uVar7;
  short sVar8;
  int unaff_D3;
  int iVar9;
  ushort uVar10;
  short sVar11;
  byte bVar12;
  byte bVar13;
  int in_A0;
  byte *pbVar14;
  int unaff_A5;

  *(undefined2 *)(unaff_A5 + 0x42a4) = 7;
  *(undefined1 *)(unaff_A5 + 0x42a2) = 0;
  if ((*(char *)(unaff_A5 + 0x4008) == '\0') && (*(char *)(unaff_A5 + 0x4290) == '\0')) {
    return CONCAT44(in_D0,in_D1);
  }
  iVar6 = in_D0 + unaff_D2;
  iVar9 = in_D1 + unaff_D3;
  iVar1 = in_D0;
  iVar4 = in_D1;
  if (in_A0 != 0) {
    iVar1 = *(int *)(in_A0 + 0x1c) + in_D0;
    iVar4 = *(int *)(in_A0 + 0x20) + in_D1;
    iVar6 = *(int *)(in_A0 + 0x14) + iVar6;
    iVar9 = *(int *)(in_A0 + 0x18) + iVar9;
  }
  iVar2 = iVar1;
  if (iVar6 < iVar1) {
    iVar2 = iVar6;
    iVar6 = iVar1;
  }
  iVar1 = iVar4;
  if (iVar9 < iVar4) {
    iVar1 = iVar9;
    iVar9 = iVar4;
  }
  *(int *)(unaff_A5 + 0x4292) = iVar2;
  *(int *)(unaff_A5 + 0x4296) = iVar1;
  *(int *)(unaff_A5 + 0x429a) = iVar6;
  *(int *)(unaff_A5 + 0x429e) = iVar9;
  uVar7 = (short)((uint)iVar6 >> 0x10) + 0x80;
  if (-1 < (short)uVar7) {
    uVar3 = (short)((uint)iVar2 >> 0x10) + 0x80;
    if ((short)uVar3 < 0) {
      uVar3 = 0;
    }
    uVar10 = (short)((uint)iVar9 >> 0x10) - 0x80;
    if (-1 < (short)uVar10) {
      uVar5 = (short)((uint)iVar1 >> 0x10) - 0x80;
      if ((short)uVar5 < 0) {
        uVar5 = 0;
      }
      if ((short)uVar3 < 0x240) {
        if (0x23f < (short)uVar7) {
          uVar7 = 0x23f;
        }
        if ((short)uVar5 < 0x200) {
          if (0x1ff < (short)uVar10) {
            uVar10 = 0x1ff;
          }
          sVar8 = (uVar7 >> 5) - (uVar3 >> 5);
          sVar11 = (uVar10 >> 5) - (uVar5 >> 5);
          bVar13 = 0;
          bVar12 = 0;
          pbVar14 = (byte *)(unaff_A5 + 0x426e + (int)(short)(uVar3 >> 5));
          do {
            bVar13 = *pbVar14 | bVar13;
            sVar8 = sVar8 + -1;
            pbVar14 = pbVar14 + 1;
          } while (sVar8 != -1);
          pbVar14 = (byte *)(unaff_A5 + 0x4280 + (int)(short)(uVar5 >> 5));
          do {
            bVar12 = *pbVar14 | bVar12;
            sVar11 = sVar11 + -1;
            pbVar14 = pbVar14 + 1;
          } while (sVar11 != -1);
          *(byte *)(unaff_A5 + 0x42a2) = bVar12 & bVar13;
        }
      }
    }
  }
  if (*(char *)(unaff_A5 + 0x4008) == '\0') {
    *(byte *)(unaff_A5 + 0x42a2) = *(byte *)(unaff_A5 + 0x4290) & *(byte *)(unaff_A5 + 0x42a2);
  }
  return CONCAT44(in_D0,in_D1);
}
