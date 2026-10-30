/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a35e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a35e8(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if (*(int *)((int)unaff_A6 + 0x76) != 0) {
    candidate_a3ef0();
  }
  candidate_a3ef0();
  candidate_4256();
  candidate_1b1dc();
  candidate_12192();
  *unaff_A6 = 0xa3672;
  uVar1 = 0;
  candidate_a3f52();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_a3352;
    uVar1 = 0;
  }
  candidate_a3fc0();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_a30b2;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a36ac;
    uVar1 = false;
    candidate_1b206();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_a36f4;
      uVar1 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_a378a;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = 0xa385c;
  }
  return;
}
