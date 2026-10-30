/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0374ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_374ba(void)

{
  int unaff_A5;
  int unaff_A6;

  candidate_1ab98();
  *(undefined2 *)(unaff_A6 + 0x66) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  if (*(ushort *)(unaff_A5 + 0x3954) < *(ushort *)(unaff_A5 + 0x395c)) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfb;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 4;
  }
  return;
}
