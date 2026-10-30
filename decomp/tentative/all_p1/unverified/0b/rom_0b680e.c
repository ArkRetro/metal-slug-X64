/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b680e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b680e(void)

{
  undefined4 *unaff_A6;

  candidate_146e8();
  candidate_1436a();
  candidate_c88b0();
  candidate_c88b0();
  *unaff_A6 = 0xb6834;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 1;
  candidate_1abc8();
  candidate_140c0();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -1;
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    candidate_1abc8();
    candidate_c88b0();
    candidate_c88b0();
    candidate_13f1e();
    *unaff_A6 = candidate_b6202;
  }
  return;
}
