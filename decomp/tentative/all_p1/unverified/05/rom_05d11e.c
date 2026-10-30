/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d11e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d11e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_5e234();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_5d144;
    in_CF = 0;
  }
  candidate_5e244();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_5cd08;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f) + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar1;
  if (sVar1 < 0) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db92;
  }
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
