/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d500.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d500(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x6d516;
  uVar1 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_6d3e8;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_6d4ca;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar1 = 0;
    }
    else {
      *unaff_A6 = candidate_6d57a;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_6d7c2;
  }
  return;
}
