/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9162.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d9162(void)

{
  int extraout_A0;
  int unaff_A6;

  candidate_1b04a();
  *(undefined1 *)(unaff_A6 + 0x8d) = 4;
  candidate_d8f14();
  if (*(short *)(unaff_A6 + 0x90) <= *(short *)(extraout_A0 + 0x40)) {
    candidate_260c8();
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x8d) = 5;
  candidate_d8f14();
  if (*(short *)(unaff_A6 + 0x90) <= *(short *)(extraout_A0 + 0x40)) {
    candidate_260c8();
    return;
  }
  if (*(short *)(extraout_A0 + 0x3c) <= (short)(*(short *)(unaff_A6 + 0x3c) + -0x20)) {
    candidate_260c8();
    return;
  }
  candidate_260c8();
  return;
}
