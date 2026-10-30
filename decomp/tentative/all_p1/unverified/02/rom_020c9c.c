/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020c9c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20c9c(void)

{
  short extraout_D1w;
  short extraout_D1w_00;
  short unaff_D2w;
  int extraout_A0;
  undefined4 *unaff_A6;

  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    return;
  }
  *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
  *(undefined1 *)((int)unaff_A6 + 0x61) = 3;
  *unaff_A6 = 0x20cbe;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_c8d68();
  if ((extraout_D1w == 1) && (unaff_D2w == *(short *)((int)unaff_A6 + 0x72))) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
  }
  candidate_c8d68();
  if ((extraout_D1w_00 == 1) && (unaff_D2w == *(short *)((int)unaff_A6 + 0x72))) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
  }
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    candidate_120f8();
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 10;
    candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)((int)unaff_A6 + 0x72);
    candidate_9ea8();
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0x1f) = 0;
    *unaff_A6 = candidate_20d66;
  }
  return;
}
