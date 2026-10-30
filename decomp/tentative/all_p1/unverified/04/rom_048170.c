/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048170.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_48170(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  candidate_4256();
  *unaff_A6 = 0x4818e;
  candidate_49e9e();
  candidate_4940c();
  func_0x000143e4();
  candidate_49a92();
  candidate_427a();
  bVar2 = false;
  candidate_49e74();
  if (!bVar2) {
    *unaff_A6 = 0x48096;
  }
  if (*(short *)(unaff_A6 + 0x21) == 0) {
    *unaff_A6 = candidate_481e0;
    bVar2 = false;
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x21);
    bVar2 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
  }
  candidate_49ece();
  if (bVar2) {
    *unaff_A6 = candidate_4783a;
  }
  candidate_4900c();
  return;
}
