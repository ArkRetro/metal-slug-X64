/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d62d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d62d6(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 10;
  *unaff_A6 = 0xd62f2;
  candidate_1abc8();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x72);
  bVar2 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar1;
  if (sVar1 == 0) {
    if (*(byte *)(*(int *)((int)unaff_A6 + 0x8e) + 0xa6) < 0xd) {
      *unaff_A6 = candidate_d6362;
      bVar2 = false;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
      *unaff_A6 = candidate_d632e;
      bVar2 = false;
    }
  }
  candidate_19424();
  if (bVar2) {
    *unaff_A6 = candidate_d63ae;
  }
  return;
}
