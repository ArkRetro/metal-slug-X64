/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048060.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_48060(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(undefined1 *)((int)unaff_A6 + 0x96) = 3;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x9b) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
  candidate_4256();
  *(undefined1 *)(unaff_A6[0x1f] + 0x60) = 0xff;
  *unaff_A6 = 0x480b6;
  uVar3 = 0;
  candidate_49e9e();
  candidate_4940c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_48116;
  }
  candidate_499a4();
  func_0x000143e4();
  candidate_49a92();
  candidate_427a();
  bVar4 = false;
  candidate_49e74();
  if (bVar4) {
    *unaff_A6 = candidate_48170;
  }
  if (*(short *)(unaff_A6 + 0x21) == 0) {
    *unaff_A6 = candidate_481e0;
    bVar4 = false;
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x21);
    bVar4 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
  }
  candidate_49ece();
  if (bVar4) {
    *unaff_A6 = candidate_4783a;
  }
  candidate_4900c();
  return;
}
