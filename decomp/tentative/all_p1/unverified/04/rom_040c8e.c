/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x040c8e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_40c8e(void)

{
  uint uVar1;
  int iVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar1 = candidate_29c6();
  iVar2 = (uVar1 & 0xffff) * 4 + -0x10000;
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x76) = iVar2;
  unaff_A6[0xc] = iVar2;
  uVar1 = candidate_29c6();
  unaff_A6[0xd] = (uVar1 & 0xffff) * 8 + 0x40000;
  bVar4 = false;
  uVar3 = candidate_29c6();
  if (bVar4) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  *(ushort *)((int)unaff_A6 + 0x26) = (uVar3 & 0x3f) + 0x80;
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
  *unaff_A6 = candidate_40d06;
  return;
}
