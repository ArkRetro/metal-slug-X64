/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085e60.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_85e60(void)

{
  undefined4 extraout_A0;
  int unaff_A6;
  undefined1 uVar1;

  uVar1 = false;
  if (*(int *)(unaff_A6 + 0x6e) != 0) {
    candidate_1aeda();
    if ((bool)uVar1) {
      return;
    }
  }
  candidate_1b04a();
  if (!(bool)uVar1) {
    *(undefined4 *)(unaff_A6 + 0x6e) = extraout_A0;
    return;
  }
  *(undefined4 *)(unaff_A6 + 0x6e) = extraout_A0;
  return;
}
