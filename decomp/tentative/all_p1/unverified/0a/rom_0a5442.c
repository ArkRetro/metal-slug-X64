/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5442.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5442(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x82) + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_a54ac;
  }
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_a54b8;
  }
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a54b8;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_a54e2;
  }
  return;
}
