/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b68b4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b68b4(void)

{
  bool bVar1;
  short sVar2;
  int extraout_A0;
  undefined8 uVar3;
  short sStack_2;

  candidate_120f8();
  sStack_2 = 6;
  do {
    candidate_411c();
    candidate_29c6();
    candidate_1acf2();
    candidate_29c6();
    candidate_29c6();
    uVar3 = candidate_2e7c();
    *(undefined8 *)(extraout_A0 + 0x30) = uVar3;
    candidate_1dea2();
    sVar2 = sStack_2 + -1;
    bVar1 = 0 < sStack_2;
    sStack_2 = sVar2;
  } while (sVar2 != 0 && bVar1);
  return;
}
