/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x035af0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_35af0(void)

{
  int unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)(unaff_A6 + 0x9f) = *(undefined1 *)(unaff_A6 + 0x5a);
    *(undefined1 *)(unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)(unaff_A6 + 0x5f) & 0x80) != 0) {
    return;
  }
  return;
}
