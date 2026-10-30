/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021026.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21026(void)

{
  short extraout_D1w;
  short extraout_D1w_00;
  short unaff_D2w;
  undefined4 *unaff_A6;

  candidate_c8d68();
  if ((extraout_D1w != -1) && (unaff_D2w == *(short *)((int)unaff_A6 + 0x72))) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
  }
  candidate_c8d68();
  if ((extraout_D1w_00 != -1) && (unaff_D2w == *(short *)((int)unaff_A6 + 0x72))) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
  }
  candidate_20994();
  if ('\x01' < *(char *)(unaff_A6[3] + 0x61)) {
    if (*(char *)(unaff_A6 + 0x18) == '\0') {
      *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
      *unaff_A6 = candidate_210d2;
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x1c) = 0;
      *unaff_A6 = candidate_210a2;
    }
  }
  return;
}
