/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a040.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a040(void)

{
  int unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if ((bool)in_CF) {
    if ((*(byte *)(unaff_A6 + 0x5f) & 0x80) != 0) {
      *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x8a) = 4;
    }
    *(char *)(*(int *)(unaff_A6 + 0xc) + 0x17) = *(char *)(unaff_A6 + 0x17) + '\x01';
    *(undefined1 *)(unaff_A6 + 0x5a) = 0;
  }
  return;
}
