/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c88b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c88b0(void)

{
  byte *pbVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  char cVar6;
  byte bVar7;
  ushort uVar8;
  ushort uVar9;
  ushort uVar10;
  ushort uVar11;
  byte bVar12;
  char *in_A1;
  byte *pbVar13;
  byte *pbVar14;
  int unaff_A6;

  iVar2 = *(int *)(unaff_A6 + 0x38);
  sVar3 = *(short *)(unaff_A6 + 0x2c);
  if ((iVar2 != 0x10c72e) && (iVar2 != 0x10d7b6)) {
    return;
  }
  sVar4 = *(short *)(iVar2 + 0x14);
  sVar5 = *(short *)(iVar2 + 0x18);
  cVar6 = in_A1[1];
  uVar8 = (short)*in_A1 + (*(short *)(unaff_A6 + 0x28) >> 3);
  uVar10 = (ushort)(byte)in_A1[2];
  bVar7 = in_A1[3];
  pbVar13 = (byte *)(in_A1 + 10);
  do {
    DAT_00300001 = (undefined1)uVar8;
    uVar9 = -((ushort)bVar7 + (short)cVar6 + (sVar3 >> 3));
    pbVar14 = pbVar13;
    uVar11 = (ushort)bVar7;
    if ((ushort)(uVar8 - ((short)(-0x40 - sVar4) >> 3)) < 0x40) {
      do {
        if ((ushort)(uVar9 + ((short)(0x80 - sVar5) >> 3) + 0x3f) < 0x40) {
          pbVar1 = (byte *)(iVar2 + 0x88 + (int)(short)((uVar9 & 0x3f) + (uVar8 & 0x3f) * 0x40));
          if (((*pbVar1 & 0xf) == 0) || ((*(uint *)(in_A1 + 6) & 1 << (*pbVar1 & 0xf)) != 0)) {
            bVar12 = *pbVar14;
            if (0xf < bVar12) {
              __m68k_trap(0xf);
            }
            if (in_A1[4] != '\0') {
              bVar12 = bVar12 | 0x10;
            }
            *pbVar1 = (*pbVar1 & 0xe0) + bVar12;
          }
        }
        uVar11 = uVar11 - 1;
        uVar9 = uVar9 + 1;
        pbVar14 = pbVar14 + (short)(ushort)(byte)in_A1[2];
      } while (uVar11 != 0);
    }
    pbVar13 = pbVar13 + 1;
    uVar8 = uVar8 + 1;
    uVar10 = uVar10 - 1;
  } while (uVar10 != 0);
  return;
}
