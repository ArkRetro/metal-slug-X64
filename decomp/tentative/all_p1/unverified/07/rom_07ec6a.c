/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ec6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ec6a(void)

{
  uint uVar1;
  int iVar2;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_1b1dc();
  unaff_A6[0xd] = 0x70000;
  unaff_A6[0x1f] = 0xffff6000;
  uVar1 = candidate_29c6();
  iVar2 = (uVar1 & 0xffff) * 2 + 0x20000;
  if (*(char *)(unaff_A6[3] + 0x5b) < '\0') {
    iVar2 = -iVar2;
  }
  unaff_A6[0xc] = iVar2;
  *unaff_A6 = candidate_7ecd2;
  return;
}
