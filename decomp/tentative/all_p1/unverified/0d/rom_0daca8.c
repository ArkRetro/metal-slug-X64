/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0daca8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_daca8(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_57948();
  unaff_A6[0x14] = 0;
  candidate_dbbf4();
  *unaff_A6 = 0xdacde;
  candidate_1abc8();
  bVar1 = false;
  candidate_da488();
  if (bVar1) {
    *unaff_A6 = 0xdbec6;
  }
  if (*(char *)((int)unaff_A6 + 0x72) != *(char *)((int)unaff_A6 + 0x61)) {
    candidate_dad10();
  }
  *(undefined1 *)((int)unaff_A6 + 0x72) = *(undefined1 *)((int)unaff_A6 + 0x61);
  return;
}
