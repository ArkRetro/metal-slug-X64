/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0294a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_294a2(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_12192();
  candidate_19dfc();
  if ((*(byte *)(unaff_A6 + 0x21) & 0x10) != 0) {
    candidate_146e8();
  }
  candidate_1b1dc();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_4256();
  *unaff_A6 = 0x294e4;
  uVar1 = 0;
  if ((*(byte *)(unaff_A6 + 0x21) & 0x10) == 0) {
    candidate_299a8();
  }
  else {
    candidate_1abc8();
  }
  candidate_427a();
  if (((bool)uVar1) && ((*(byte *)(unaff_A6 + 0x21) & 1) == 0)) {
    *unaff_A6 = candidate_29566;
    uVar1 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar1) {
    *unaff_A6 = 0x295bc;
  }
  return;
}
