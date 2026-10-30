/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07bbe0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7bbe0(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x7bbf6;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7bc10;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7be22;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7be86;
  }
  return;
}
