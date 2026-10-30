/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ae98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ae98(void)

{
  int unaff_A6;
  bool bVar1;

  bVar1 = false;
  candidate_1aeb0();
  if (bVar1) {
    if ((*(byte *)(unaff_A6 + 0xa5) & 1) == 0) {
      return;
    }
  }
  else if ((*(byte *)(unaff_A6 + 0xa5) & 1) != 0) {
    return;
  }
  bVar1 = false;
  candidate_1aeb0();
  if (bVar1) {
    if ((*(byte *)(unaff_A6 + 0xa5) & 2) == 0) {
      return;
    }
  }
  else if ((*(byte *)(unaff_A6 + 0xa5) & 2) != 0) {
    return;
  }
  return;
}
