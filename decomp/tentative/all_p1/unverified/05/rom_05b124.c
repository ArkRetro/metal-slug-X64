/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05b124.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5b124(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  unaff_A6[0x14] = 0x5b766;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x3c;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_4256();
  *unaff_A6 = 0x5b158;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  bVar2 = false;
  if (*(short *)((int)unaff_A6 + 0x76) != 0) {
    sVar1 = *(short *)((int)unaff_A6 + 0x76);
    bVar2 = sVar1 == 0;
    sVar1 = sVar1 + -1;
    *(short *)((int)unaff_A6 + 0x76) = sVar1;
    if (sVar1 != 0) goto LAB_0005b18a;
  }
  candidate_4c24();
  if (bVar2) {
    *unaff_A6 = candidate_5b194;
  }
LAB_0005b18a:
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  candidate_5b714();
  return;
}
