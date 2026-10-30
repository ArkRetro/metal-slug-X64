/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c7ece.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c7ece(void)

{
  short sVar1;
  int in_D0;
  int in_D1;
  int iVar2;
  int unaff_A5;

  sVar1 = *(short *)(unaff_A5 + 0x69b6);
  if (sVar1 == 1) {
    iVar2 = *(int *)(unaff_A5 + 0x69a2) - (in_D0 - *(int *)(unaff_A5 + 0x69a6));
  }
  else if (sVar1 == 2) {
    iVar2 = *(int *)(unaff_A5 + 0x69a2) + (in_D1 - *(int *)(unaff_A5 + 0x69aa));
  }
  else if (sVar1 == 3) {
    iVar2 = *(int *)(unaff_A5 + 0x69a2) - (in_D1 - *(int *)(unaff_A5 + 0x69aa));
  }
  else {
    iVar2 = *(int *)(unaff_A5 + 0x69a2) + (in_D0 - *(int *)(unaff_A5 + 0x69a6));
  }
  if (iVar2 < 0) {
    return;
  }
  return;
}
