/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0210a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_210a2(void)

{
  undefined4 *unaff_A6;

  if (*(short *)(unaff_A6 + 0x1c) < 0x20) {
    candidate_211e0();
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + 1;
  }
  else {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
    *unaff_A6 = candidate_210d2;
  }
  candidate_20994();
  return;
}
