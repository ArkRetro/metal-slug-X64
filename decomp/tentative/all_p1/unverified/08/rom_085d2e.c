/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085d2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_85d2e(void)

{
  uint uVar1;
  uint uVar2;
  int unaff_A6;
  undefined1 in_CF;
  undefined8 uVar3;

  candidate_1aeda();
  if ((bool)in_CF) {
    candidate_85ec6();
    uVar3 = candidate_85fe4();
  }
  else {
    uVar3 = 0;
  }
  *(int *)(unaff_A6 + 0x30) =
       ((int)((longlong)uVar3 >> 0x23) - *(int *)(unaff_A6 + 0x30) >> 4) + *(int *)(unaff_A6 + 0x30)
  ;
  *(int *)(unaff_A6 + 0x34) =
       (((int)uVar3 >> 3) - *(int *)(unaff_A6 + 0x34) >> 4) + *(int *)(unaff_A6 + 0x34);
  if (0x80000 < *(int *)(unaff_A6 + 0x30)) {
    *(undefined4 *)(unaff_A6 + 0x30) = 0x80000;
  }
  if (*(int *)(unaff_A6 + 0x30) < -0x80000) {
    *(undefined4 *)(unaff_A6 + 0x30) = 0xfff80000;
  }
  if (0x80000 < *(int *)(unaff_A6 + 0x34)) {
    *(undefined4 *)(unaff_A6 + 0x34) = 0x80000;
  }
  if (*(int *)(unaff_A6 + 0x34) < -0x80000) {
    *(undefined4 *)(unaff_A6 + 0x34) = 0xfff80000;
  }
  func_0x000143e4();
  uVar1 = candidate_29c6();
  uVar2 = uVar1 & 0xffff001f;
  if ((uVar1 & 0x1f) == 0) {
    uVar2 = candidate_85fd2();
  }
  return uVar2;
}
