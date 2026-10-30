/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0481e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_481e0(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;
  undefined1 uVar2;

  candidate_49862();
  if (!(bool)in_CF) {
    candidate_4256();
    *unaff_A6 = 0x481fe;
    candidate_499a4();
    func_0x000143e4();
    candidate_49a92();
    candidate_427a();
    bVar1 = false;
    candidate_49e74();
    if (bVar1) {
      *unaff_A6 = candidate_4828e;
    }
    candidate_4900c();
    return;
  }
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  candidate_4256();
  *unaff_A6 = 0x48254;
  uVar2 = 0;
  candidate_49e9e();
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x96) = 3;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    *unaff_A6 = candidate_481e0;
    uVar2 = 0;
  }
  candidate_49ece();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4783a;
  }
  candidate_4900c();
  return;
}
