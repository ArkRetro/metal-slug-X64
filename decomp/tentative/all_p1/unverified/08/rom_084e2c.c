/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x084e2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_84e2c(void)

{
  int unaff_A6;

  *(undefined4 *)(unaff_A6 + 0x72) = *(undefined4 *)(unaff_A6 + 0x34);
  *(undefined1 *)(unaff_A6 + 0x98) = *(undefined1 *)(unaff_A6 + 0x15);
  candidate_143dc();
  if (((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) && ((*(byte *)(unaff_A6 + 0x98) & 0x40) == 0)) {
    *(int *)(unaff_A6 + 0x34) = -(*(int *)(unaff_A6 + 0x72) >> 2);
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) >> 1;
    candidate_120f8();
  }
  return;
}
