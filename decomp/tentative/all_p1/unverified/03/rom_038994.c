/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038994.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38994(void)

{
  int unaff_A6;

  candidate_120f8();
  *(undefined4 *)(unaff_A6 + 0x72) = 0x38a32;
  if (*(short *)(unaff_A6 + 0x70) == 0) {
    candidate_13f1e();
  }
  candidate_1da40();
  candidate_1da40();
  candidate_1da40();
                    /* WARNING: Could not recover jumptable at 0x000389e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)((short)(*(short *)(unaff_A6 + 0x6e) * 4) + 0x38a5a))();
  return;
}
