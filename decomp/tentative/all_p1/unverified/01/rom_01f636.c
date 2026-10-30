/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f636.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1f636(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if (*(short *)((int)unaff_A6 + 0x6e) == 1) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x1f66a;
  uVar1 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x1f5b2;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar1 = 0;
    }
    else {
      *unaff_A6 = candidate_1f6c0;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_1f7ea;
  }
  return;
}
