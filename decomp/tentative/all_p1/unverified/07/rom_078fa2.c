/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078fa2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78fa2(void)

{
  int iVar1;
  short *psVar2;
  byte *pbVar3;
  int unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if ((bool)in_CF) {
    iVar1 = *(int *)(unaff_A6 + 0xc);
    pbVar3 = (byte *)(iVar1 + 0x9c);
    *pbVar3 = *pbVar3 & ~('\x01' << (*(ushort *)(unaff_A6 + 0x8a) & 7));
    psVar2 = (short *)(iVar1 + 0x88);
    *psVar2 = *psVar2 + 1;
    candidate_406b6();
  }
  return;
}
