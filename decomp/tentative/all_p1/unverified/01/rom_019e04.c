/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019e04.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_19e04(void)

{
  short in_D0w;
  undefined4 *in_A0;
  undefined4 extraout_A0;
  undefined4 uVar1;
  int extraout_A0_00;
  int unaff_A5;

  if (in_A0 == (undefined4 *)0x0) {
    return;
  }
  uVar1 = *in_A0;
  *(short *)(unaff_A5 + 0x4324) = in_D0w;
  if (in_D0w != 0) {
    candidate_19f3c();
    uVar1 = extraout_A0;
  }
  candidate_41b0();
  *(undefined4 *)(extraout_A0_00 + 0x38) = 0;
  *(undefined2 *)(extraout_A0_00 + 0x2c) = 0x100;
  *(undefined4 *)(extraout_A0_00 + 0x6e) = uVar1;
  return;
}
