/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078256.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78256(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x7826c;
  candidate_1abc8();
  candidate_427a();
  uVar2 = 0;
  if (*(short *)(unaff_A6 + 0x23) == 0) {
    candidate_7901c();
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x23);
    uVar2 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x23) = sVar1 + -1;
  }
  candidate_78fd4();
  candidate_78fa2();
  candidate_78d7a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
