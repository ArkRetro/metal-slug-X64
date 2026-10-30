/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x089654.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_89654(void)

{
  int extraout_A0;
  int in_A1;
  int unaff_A6;

  candidate_411c();
  candidate_1acf2();
  *(undefined4 *)(extraout_A0 + 0x30) = *(undefined4 *)(in_A1 + 4);
  *(undefined4 *)(extraout_A0 + 0x34) = *(undefined4 *)(in_A1 + 8);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(int *)(extraout_A0 + 0x30) = -*(int *)(extraout_A0 + 0x30);
  }
  return;
}
