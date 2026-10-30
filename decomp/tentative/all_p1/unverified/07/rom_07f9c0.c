/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f9c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f9c0(void)

{
  short sVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  candidate_4256();
  *unaff_A6 = 0x7f9d6;
  uVar3 = 0;
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_7fa6c;
  }
  unaff_A6[0xd] = unaff_A6[0xd] - (unaff_A6[0xd] - *(int *)((int)unaff_A6 + 0x6e) >> 3);
  *(int *)((int)unaff_A6 + 0x72) =
       *(int *)((int)unaff_A6 + 0x72) - (*(int *)((int)unaff_A6 + 0x72) >> 3);
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x04';
  iVar2 = candidate_2e7c();
  unaff_A6[0xc] = *(int *)((int)unaff_A6 + 0x72) + iVar2;
  bVar4 = false;
  candidate_4c24();
  if ((bVar4) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    *unaff_A6 = candidate_7fa6c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7fa6c;
  }
  candidate_7fc46();
  sVar1 = *(short *)(unaff_A6 + 0x1e) + -1;
  *(short *)(unaff_A6 + 0x1e) = sVar1;
  if (sVar1 == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  }
  return;
}
