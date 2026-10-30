/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03040e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3040e(void)

{
  uint uVar1;
  int iVar2;
  int unaff_A6;

  uVar1 = *(uint *)(unaff_A6 + 0x72);
  if ((uVar1 & 0xf0000) == 0x90000) {
    iVar2 = uVar1 + 0x70000;
  }
  else {
    iVar2 = uVar1 + 0x10000;
  }
  *(int *)(unaff_A6 + 0x72) = iVar2;
  *(undefined4 *)(unaff_A6 + 0x76) = 0x10000;
  candidate_57948();
  return;
}
