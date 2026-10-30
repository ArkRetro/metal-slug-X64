/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0999c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_999c2(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if (*(short *)(unaff_A6 + 0xf) < 0xa1) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_4256();
  *unaff_A6 = 0x999f6;
  uVar1 = 0;
  candidate_99bb2();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_99a0e;
    uVar1 = 0;
  }
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
