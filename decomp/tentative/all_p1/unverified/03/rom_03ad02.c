/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03ad02.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3ad02(void)

{
  byte *pbVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  ushort uVar6;
  ushort uVar7;
  ushort uVar8;
  short sVar9;
  ushort uVar10;
  ushort uVar11;
  ushort uVar12;
  byte bVar13;
  byte *pbVar14;
  byte *pbVar15;
  int unaff_A6;

  iVar2 = *(int *)(unaff_A6 + 0x38);
  sVar3 = *(short *)(unaff_A6 + 0x2c);
  if ((iVar2 != 0x10c72e) && (iVar2 != 0x10d7b6)) {
    return;
  }
  sVar4 = *(short *)(iVar2 + 0x14);
  sVar5 = *(short *)(iVar2 + 0x18);
  sVar9 = (short)cRam0003adc9;
  uVar6 = (short)cRam0003adc8 + (*(short *)(unaff_A6 + 0x28) >> 3);
  uVar8 = (ushort)bRam0003adca;
  uVar10 = (ushort)bRam0003adcb;
  pbVar14 = (byte *)0x3add2;
  uVar11 = (ushort)bRam0003adcb;
  do {
    DAT_00300001 = (undefined1)uVar6;
    uVar7 = -(uVar11 + sVar9 + (sVar3 >> 3));
    pbVar15 = pbVar14;
    uVar12 = uVar10;
    if ((ushort)(uVar6 - ((short)(-0x40 - sVar4) >> 3)) < 0x40) {
      do {
        if ((ushort)(uVar7 + ((short)(0x80 - sVar5) >> 3) + 0x3f) < 0x40) {
          pbVar1 = (byte *)(iVar2 + 0x88 + (int)(short)((uVar7 & 0x3f) + (uVar6 & 0x3f) * 0x40));
          if (((*pbVar1 & 0xf) == 0) || ((uRam0003adce & 1 << (*pbVar1 & 0xf)) != 0)) {
            bVar13 = *pbVar15;
            if (0xf < bVar13) {
              __m68k_trap(0xf);
            }
            if (cRam0003adcc != '\0') {
              bVar13 = bVar13 | 0x10;
            }
            *pbVar1 = (*pbVar1 & 0xe0) + bVar13;
          }
        }
        uVar12 = uVar12 - 1;
        uVar7 = uVar7 + 1;
        pbVar15 = pbVar15 + (short)(ushort)bRam0003adca;
      } while (uVar12 != 0);
    }
    pbVar14 = pbVar14 + 1;
    uVar6 = uVar6 + 1;
    uVar8 = uVar8 - 1;
  } while (uVar8 != 0);
  return;
}
