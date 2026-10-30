/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078bd2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78bd2(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  func_0x00078f90();
  candidate_4256();
  *unaff_A6 = 0x78bec;
  candidate_78cf4();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x23) == 0) {
    *unaff_A6 = candidate_78c32;
    uVar2 = false;
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x23);
    uVar2 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x23) = sVar1 + -1;
  }
  candidate_78fc4();
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar2) {
    *unaff_A6 = 0x77e5c;
    uVar2 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
