/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f1c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f1c8(void)

{
  int unaff_A6;
  char in_NF;
  char in_VF;

  if (((*(byte *)(unaff_A6 + 0x96) & 0x80) != 0) && ((*(byte *)(unaff_A6 + 0x96) & 0x20) == 0)) {
    candidate_9f1ac();
    if (in_VF == in_NF) {
      if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
        return;
      }
    }
    else if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      return;
    }
  }
  return;
}
