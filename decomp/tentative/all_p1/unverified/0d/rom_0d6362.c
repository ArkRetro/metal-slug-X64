/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6362.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6362(void)

{
  short sVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x3c;
  *unaff_A6 = 0xd637e;
  candidate_1abc8();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x72);
  sVar2 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar2;
  if (sVar2 == 0) {
    *unaff_A6 = 0xd624a;
  }
  bVar3 = sVar2 != 0 && sVar1 == 0;
  candidate_19424();
  if (bVar3) {
    *unaff_A6 = candidate_d63ae;
  }
  return;
}
