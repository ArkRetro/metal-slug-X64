/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f67c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f67c(void)

{
  int extraout_A0;
  int extraout_A0_00;
  undefined4 extraout_A0_01;
  undefined4 extraout_A0_02;
  undefined4 extraout_A0_03;
  undefined4 extraout_A0_04;
  undefined4 extraout_A0_05;
  int unaff_A6;
  bool bVar1;

  if (*(int *)(unaff_A6 + 0x7a) != 0) {
    bVar1 = false;
    candidate_1aeb0();
    if ((bVar1) && (extraout_A0 == *(int *)(unaff_A6 + 0x7a))) {
      return;
    }
    bVar1 = false;
    candidate_1aeb0();
    if ((bVar1) && (extraout_A0_00 == *(int *)(unaff_A6 + 0x7a))) {
      return;
    }
  }
  candidate_1af9a();
  *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0_01;
  bVar1 = false;
  candidate_1aeb0();
  if ((!bVar1) || (candidate_8f72e(), !bVar1)) {
    bVar1 = false;
    candidate_1aeb0();
    if ((bVar1) && (candidate_8f72e(), bVar1)) {
      *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0_04;
      return;
    }
    candidate_1af9a();
    *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0_05;
    return;
  }
  *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0_02;
  bVar1 = false;
  candidate_1aeb0();
  if (!bVar1) {
    return;
  }
  candidate_8f72e();
  if (!bVar1) {
    return;
  }
  candidate_1af9a();
  *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0_03;
  return;
}
