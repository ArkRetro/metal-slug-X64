/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d40a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d40a6(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_120f8();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x7f) = 5;
  *(undefined1 *)(unaff_A6 + 0x20) = 5;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 5;
  *(undefined1 *)((int)unaff_A6 + 0x82) = 5;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 5;
  *unaff_A6 = 0xd40e4;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d4116;
    uVar1 = 0;
  }
  candidate_d50cc();
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d41de;
  }
  return;
}
