/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09954a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9954a(void)

{
  ushort uVar2;
  short sVar3;
  int iVar1;
  int unaff_A6;

  uVar2 = *(ushort *)(unaff_A6 + 0x6e);
  if ((short)uVar2 < 0) {
    uVar2 = candidate_29c6();
  }
  sVar3 = (uVar2 & 3) * 0xc;
  *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(sVar3 + 0x995a6);
  *(undefined4 *)(unaff_A6 + 0x78) = *(undefined4 *)(sVar3 + 0x995aa);
  iVar1 = *(int *)(sVar3 + 0x995ae);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = -iVar1;
  }
  *(int *)(unaff_A6 + 0x30) = iVar1;
  return;
}
