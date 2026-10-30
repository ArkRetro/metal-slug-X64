/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6c2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6c2e(void)

{
  byte bVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_d71ca();
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d6c66;
  }
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
  if (*(short *)((int)unaff_A6 + 0x72) < 1) {
    *unaff_A6 = candidate_d6c66;
  }
  sVar2 = (short)*(int *)((int)unaff_A6 + 0x96);
  if (*(int *)((int)unaff_A6 + 0x96) != 0) {
    sVar2 = candidate_49e8();
  }
  if (sVar2 != 0) {
    *unaff_A6 = candidate_d6a72;
  }
  bVar3 = false;
  candidate_4c24();
  if (bVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0x7a);
  if (bVar1 == 5) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar3 = bVar1 < 5;
  candidate_19504();
  if (bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
