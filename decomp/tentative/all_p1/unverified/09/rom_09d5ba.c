/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09d5ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d5ba(void)

{
  int unaff_A6;

  if (((*(byte *)(unaff_A6 + 0x90) & 0x40) != 0) && ((*(byte *)(unaff_A6 + 0x90) & 0x80) == 0)) {
    candidate_8cdb2();
    *(byte *)(unaff_A6 + 0x90) = *(byte *)(unaff_A6 + 0x90) | 0x80;
  }
  return;
}
