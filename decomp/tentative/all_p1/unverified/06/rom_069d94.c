/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069d94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69d94(void)

{
  short extraout_D0u;
  int unaff_A6;

  candidate_1ac4c();
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0x72) <= extraout_D0u) {
      return;
    }
  }
  else if (extraout_D0u <= *(short *)(unaff_A6 + 0x74)) {
    return;
  }
  return;
}
