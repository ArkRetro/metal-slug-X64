/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0999ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_999ac(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar1;

  candidate_1af9a();
  if (!(bool)in_CF) {
    if (*(short *)(unaff_A6 + 0xf) < *(short *)(extraout_A0 + 0x3c)) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
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
