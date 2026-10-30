/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06139e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6139e(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    *unaff_A6 = candidate_6134c;
  }
  return;
}
