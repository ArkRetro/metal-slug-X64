/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087a64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87a64(void)

{
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x8b) & 0x10) == 0) {
    candidate_87954();
    if ((bool)in_CF) {
      return;
    }
  }
  else if (((*(byte *)(unaff_A6 + 0x8b) & 0x20) == 0) &&
          (*(short *)(*(int *)(unaff_A6 + 0x6e) + 0x40) < *(short *)(unaff_A6 + 0x40))) {
    return;
  }
  return;
}
