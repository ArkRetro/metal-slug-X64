/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01264e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1264e(void)

{
  uint in_D0;
  uint uVar1;
  short sVar2;
  short in_A1w;
  ushort *puVar3;
  short *psVar4;

  puVar3 = (ushort *)0x10bff4;
  uVar1 = 0xc;
  do {
    *puVar3 = (ushort)(in_D0 >> (uVar1 & 0x3f)) & 0xf;
    puVar3 = puVar3 + 1;
    uVar1 = uVar1 - 4;
  } while (uVar1 != 0);
  *puVar3 = (ushort)in_D0 & 0xf;
  sVar2 = 3;
  psVar4 = (short *)0x10bff4;
  do {
    _DAT_003c0000 = in_A1w;
    uRam003c0002 = (ushort)*(byte *)(*psVar4 + 0x126a8);
    in_A1w = in_A1w + 0x20;
    sVar2 = sVar2 + -1;
    psVar4 = psVar4 + 1;
  } while (sVar2 != -1);
  return;
}
