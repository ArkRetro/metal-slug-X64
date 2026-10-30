/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x088978.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_88978(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A6;

  candidate_120f8();
  candidate_411c();
  candidate_1acf2();
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)(extraout_A0 + 0x15) = *(byte *)(extraout_A0 + 0x15) | 1;
  }
  candidate_411c();
  candidate_1acf2();
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)(extraout_A0_00 + 0x15) = *(byte *)(extraout_A0_00 + 0x15) | 1;
  }
  candidate_411c();
  candidate_1acf2();
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    *(byte *)(extraout_A0_01 + 0x15) = *(byte *)(extraout_A0_01 + 0x15) | 1;
  }
  return;
}
