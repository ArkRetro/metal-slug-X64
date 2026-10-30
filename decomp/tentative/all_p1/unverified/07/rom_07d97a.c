/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07d97a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7d97a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1abc8();
  candidate_427a();
  uVar1 = *(char *)((int)unaff_A6 + 0x61) != -1;
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_7d9c4;
  }
  candidate_7dc96();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7d9c4;
    uVar1 = 0;
  }
  candidate_8d972();
  if ((bool)uVar1) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    uVar1 = 0;
  }
  candidate_7dcbc();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7da98;
  }
  return;
}
