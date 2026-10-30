/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07aa7e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7aa7e(void)

{
  int extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;

  candidate_1da40();
  *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
  *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
  candidate_1da40();
  *(undefined2 *)(extraout_A0_00 + 0x66) = 0x1001;
  *(undefined2 *)(extraout_A0_00 + 0x1e) = 0x1001;
  *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
  *unaff_A6 = 0x7aaba;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x61) == -1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
