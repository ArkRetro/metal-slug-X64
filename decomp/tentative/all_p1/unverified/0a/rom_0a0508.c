/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0508.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0508(void)

{
  int unaff_A6;
  undefined1 in_CF;

  candidate_a04e2();
  if ((bool)in_CF) {
    *(char *)(unaff_A6 + 0x8a) = *(char *)(unaff_A6 + 0x8a) + '\x01';
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x8a) = 0;
  }
  if (*(char *)(unaff_A6 + 0x8a) < '\x06') {
    return;
  }
  return;
}
