/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bc38e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bc38e(void)

{
  ushort uVar1;
  char cVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  ushort uVar7;
  byte bVar8;
  char *in_A1;
  byte *pbVar9;
  byte *pbVar10;
  int unaff_A6;

  uVar1 = *(ushort *)(unaff_A6 + 0x2c);
  cVar2 = in_A1[1];
  uVar4 = (short)*in_A1 + (*(ushort *)(unaff_A6 + 0x28) >> 3);
  uVar6 = (ushort)(byte)in_A1[2];
  bVar3 = in_A1[3];
  pbVar9 = (byte *)(in_A1 + 10);
  do {
    DAT_00300001 = (undefined1)uVar4;
    uVar5 = -((ushort)bVar3 + (short)cVar2 + (uVar1 >> 3));
    pbVar10 = pbVar9;
    uVar7 = (ushort)bVar3;
    do {
      bVar8 = *pbVar10;
      if (0xf < bVar8) {
        __m68k_trap(0xf);
      }
      if (in_A1[4] != '\0') {
        bVar8 = bVar8 | 0x10;
      }
      *(byte *)((short)((uVar5 & 0x3f) + (uVar4 & 0x3f) * 0x40 + 0x88) + 0x10c72e) =
           in_A1[5] << 5 | bVar8;
      pbVar10 = pbVar10 + (short)(ushort)(byte)in_A1[2];
      uVar5 = uVar5 + 1;
      uVar7 = uVar7 - 1;
    } while (uVar7 != 0);
    pbVar9 = pbVar9 + 1;
    uVar4 = uVar4 + 1;
    uVar6 = uVar6 - 1;
  } while (uVar6 != 0);
  return;
}
