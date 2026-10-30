/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8a3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c8a3c(void)

{
  char cVar1;
  byte bVar2;
  short in_D0w;
  ushort uVar3;
  short in_D1w;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  byte bVar7;
  int in_A0;
  char *in_A1;
  byte *pbVar8;
  byte *pbVar9;

  cVar1 = in_A1[1];
  uVar3 = *in_A1 + in_D0w;
  uVar5 = (ushort)(byte)in_A1[2];
  bVar2 = in_A1[3];
  pbVar8 = (byte *)(in_A1 + 10);
  do {
    DAT_00300001 = (undefined1)uVar3;
    uVar4 = -((ushort)bVar2 + cVar1 + in_D1w);
    pbVar9 = pbVar8;
    uVar6 = (ushort)bVar2;
    do {
      bVar7 = *pbVar9;
      if (0xf < bVar7) {
        __m68k_trap(0xf);
      }
      if (in_A1[4] != '\0') {
        bVar7 = bVar7 | 0x10;
      }
      *(byte *)(in_A0 + (short)((uVar4 & 0x3f) + (uVar3 & 0x3f) * 0x40 + 0x88)) =
           in_A1[5] << 5 | bVar7;
      pbVar9 = pbVar9 + (short)(ushort)(byte)in_A1[2];
      uVar4 = uVar4 + 1;
      uVar6 = uVar6 - 1;
    } while (uVar6 != 0);
    pbVar8 = pbVar8 + 1;
    uVar3 = uVar3 + 1;
    uVar5 = uVar5 - 1;
  } while (uVar5 != 0);
  return;
}
