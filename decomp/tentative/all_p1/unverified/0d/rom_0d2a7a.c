/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2a7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2a7a(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xd2aaa;
  candidate_1abc8();
  sVar1 = *(short *)((int)unaff_A6 + 0x1e);
  bVar2 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x1e) = sVar1 + -1;
  candidate_427a();
  if (bVar2) {
    *(undefined1 *)(unaff_A6[3] + 0x61) = 1;
    *unaff_A6 = candidate_d280a;
  }
  uVar3 = 0;
  if (*(int *)((int)unaff_A6 + 0x82) != 0) {
    candidate_49e8();
  }
  candidate_d341e();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_d280a;
  }
  return;
}
