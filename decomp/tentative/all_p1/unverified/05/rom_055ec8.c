/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055ec8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_55ec8(void)

{
  int unaff_A6;
  bool bVar1;

  bVar1 = (*(byte *)(unaff_A6 + 0x15) & 1) == 0;
  candidate_149ca();
  if (!bVar1) {
    return 0;
  }
  bVar1 = (*(byte *)(unaff_A6 + 0x15) & 1) == 0;
  candidate_149ca();
  if (!bVar1) {
    return 1;
  }
  return 2;
}
