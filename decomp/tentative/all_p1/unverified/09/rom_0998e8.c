/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0998e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_998e8(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar1;

  candidate_99bb2();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_99922;
    in_CF = 0;
  }
  candidate_99c3c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_999c2;
  }
  if (*(int *)((int)unaff_A6 + 0x76) == 0) {
    *unaff_A6 = candidate_99a22;
  }
  uVar1 = false;
  candidate_19504();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_99b22;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_99b22;
  }
  return;
}
