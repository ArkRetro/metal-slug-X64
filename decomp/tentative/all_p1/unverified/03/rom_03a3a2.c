/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a3a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a3a2(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_120f8();
  candidate_c801a();
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  *(undefined2 *)((int)unaff_A6 + 0x72) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  *unaff_A6 = 0x3a3e8;
  candidate_1436a();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x72);
  *(short *)((int)unaff_A6 + 0x72) = sVar1 + -1;
  if (sVar1 == 0) {
    candidate_1da40();
    candidate_1da40();
    candidate_40bc();
    *unaff_A6 = candidate_3a42a;
  }
  if (*(char *)((int)unaff_A6 + 0x75) != '\0') {
    candidate_49e8();
  }
  candidate_4c24();
  return;
}
