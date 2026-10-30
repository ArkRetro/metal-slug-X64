/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049c82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49c82(void)

{
  byte bVar1;
  int unaff_A6;
  bool bVar2;

  *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -3;
  bVar1 = *(char *)((short)(ushort)(*(byte *)(unaff_A6 + 0xaa) >> 4) + 0xe09f4) - 3;
  bVar2 = bVar1 < *(byte *)(unaff_A6 + 0x96);
  if (*(byte *)(unaff_A6 + 0x96) < bVar1) {
    bVar1 = *(byte *)(unaff_A6 + 0x96);
    bVar2 = 0xfe < bVar1;
    *(byte *)(unaff_A6 + 0x96) = bVar1 + 1;
  }
  if (bVar2) {
    *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -1;
  }
  return;
}
