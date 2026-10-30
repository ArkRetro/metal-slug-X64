/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0210d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_210d2(void)

{
  short extraout_D1w;
  short extraout_D1w_00;
  short unaff_D2w;
  undefined4 *unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_c8d68();
  if (((unaff_D2w == *(short *)((int)unaff_A6 + 0x72)) && (extraout_D1w != -1)) &&
     (*(undefined1 *)(unaff_A6 + 0x18) = 1, extraout_D1w == 1)) {
    *(undefined1 *)(unaff_A6 + 0x18) = 2;
  }
  candidate_c8d68();
  if (((unaff_D2w == *(short *)((int)unaff_A6 + 0x72)) && (extraout_D1w_00 != -1)) &&
     (*(undefined1 *)(unaff_A6 + 0x18) = 1, extraout_D1w_00 == 1)) {
    *(undefined1 *)(unaff_A6 + 0x18) = 2;
  }
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    candidate_2122e();
  }
  else {
    candidate_211e0();
    if (*(char *)(unaff_A6 + 0x18) == '\x02') {
      *unaff_A6 = candidate_21162;
    }
  }
  candidate_20994();
  return;
}
