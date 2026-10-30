/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07b680.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7b680(void)

{
  int iVar1;
  int unaff_A6;
  undefined8 uVar2;

  candidate_2eb4();
  iVar1 = *(int *)(unaff_A6 + 0x76) + -0xa000;
  if (iVar1 < 0) {
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
    *(undefined4 *)(unaff_A6 + 0x76) = 0;
  }
  else {
    *(int *)(unaff_A6 + 0x76) = iVar1;
    uVar2 = candidate_2e7c();
    *(undefined8 *)(unaff_A6 + 0x30) = uVar2;
  }
  return;
}
