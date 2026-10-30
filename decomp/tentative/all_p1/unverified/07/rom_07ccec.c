/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ccec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ccec(void)

{
  int iVar1;
  ushort uVar2;
  short sVar3;
  int unaff_A6;

  uVar2 = candidate_260c8();
  *(undefined4 *)(unaff_A6 + 0x7e) = *(undefined4 *)((short)((uVar2 & 3) << 2) + 0xe4690);
  sVar3 = (*(ushort *)(unaff_A6 + 0x8f) & 3) * 0xc;
  iVar1 = *(int *)(unaff_A6 + 0x7e);
  *(undefined4 *)(unaff_A6 + 0x30) = *(undefined4 *)(iVar1 + sVar3);
  *(undefined4 *)(unaff_A6 + 0x76) = *(undefined4 *)(iVar1 + 4 + (int)sVar3);
  *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(iVar1 + 8 + (int)sVar3);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(int *)(unaff_A6 + 0x30) = -*(int *)(unaff_A6 + 0x30);
  }
  return;
}
