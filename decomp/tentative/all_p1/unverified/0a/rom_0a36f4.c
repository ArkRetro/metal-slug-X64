/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a36f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a36f4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_a3f22();
  if (*(int *)((int)unaff_A6 + 0x76) != 0) {
    candidate_a3ef0();
  }
  candidate_a3ef0();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
  candidate_1b1dc();
  candidate_19dfc();
  unaff_A6[0x14] = 0;
  *unaff_A6 = 0xa3752;
  uVar1 = 0;
  candidate_a3f52();
  candidate_a4036();
  candidate_427a();
  if (((bool)uVar1) && ((*(byte *)((int)unaff_A6 + 0x96) & 1) == 0)) {
    *unaff_A6 = candidate_a37ba;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = 0xa385c;
  }
  return;
}
