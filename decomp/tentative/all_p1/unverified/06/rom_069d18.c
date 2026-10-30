/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069d18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69d18(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x15);
  *(byte *)(unaff_A6 + 0x15) = bVar1 & 0xfe;
  candidate_427a();
  if ((bVar1 & 1) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  return;
}
