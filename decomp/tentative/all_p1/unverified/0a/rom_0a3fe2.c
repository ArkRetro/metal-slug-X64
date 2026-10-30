/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3fe2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3fe2(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x8a);
  *(char *)(unaff_A6 + 0x8a) = *(char *)(unaff_A6 + 0x8a) + '\x01';
  bVar1 = bVar1 >> 4;
  if (3 < bVar1) {
    bVar1 = 3;
  }
  *(ushort *)(unaff_A6 + 0x26) = ((bVar1 & 3) + 1) * 0x80;
  return;
}
