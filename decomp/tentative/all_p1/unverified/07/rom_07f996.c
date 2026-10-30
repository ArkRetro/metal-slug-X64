/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f996.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f996(void)

{
  ushort uVar1;
  short sVar2;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)((int)unaff_A6 + 0x7a);
  *unaff_A6 = 0x7f9a2;
  func_0x000143e4();
  candidate_427a();
  uVar1 = *(ushort *)((int)unaff_A6 + 0x76) - 1;
  *(ushort *)((int)unaff_A6 + 0x76) = uVar1;
  if ((int)((uint)uVar1 << 0x10) < 0) {
    *unaff_A6 = candidate_7f9c0;
  }
  unaff_A6[0xd] = unaff_A6[0xd] - (unaff_A6[0xd] - *(int *)((int)unaff_A6 + 0x6e) >> 3);
  *(int *)((int)unaff_A6 + 0x72) =
       *(int *)((int)unaff_A6 + 0x72) - (*(int *)((int)unaff_A6 + 0x72) >> 3);
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x04';
  iVar3 = candidate_2e7c();
  unaff_A6[0xc] = *(int *)((int)unaff_A6 + 0x72) + iVar3;
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
  sVar2 = *(short *)(unaff_A6 + 0x1e) + -1;
  *(short *)(unaff_A6 + 0x1e) = sVar2;
  if (sVar2 == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  }
  return;
}
