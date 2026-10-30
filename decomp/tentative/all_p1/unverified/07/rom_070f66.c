/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x070f66.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_70f66(void)

{
  uint in_D0;
  int iVar1;
  int unaff_A6;

  iVar1 = 0;
  if ((in_D0 & 8) != 0) {
    iVar1 = 0x40000;
  }
  if ((in_D0 & 4) != 0) {
    iVar1 = -0x40000;
  }
  *(int *)(unaff_A6 + 0x30) = (iVar1 - *(int *)(unaff_A6 + 0x30) >> 2) + *(int *)(unaff_A6 + 0x30);
  iVar1 = 0;
  if ((in_D0 & 1) != 0) {
    iVar1 = 0x40000;
  }
  if ((in_D0 & 2) != 0) {
    iVar1 = -0x40000;
  }
  *(int *)(unaff_A6 + 0x34) = (iVar1 - *(int *)(unaff_A6 + 0x34) >> 2) + *(int *)(unaff_A6 + 0x34);
  return;
}
