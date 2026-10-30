/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055bae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55bae(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A6;
  bool bVar2;

  *(undefined1 *)(unaff_A6 + 0x95) = 0;
  *(undefined1 *)(unaff_A6 + 0x94) = 0;
  bVar2 = false;
  candidate_1affc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1affc();
    iVar1 = extraout_A0_00;
    if (bVar2) {
      iVar1 = -1;
      goto LAB_00055bf2;
    }
  }
  if (0x30 < (short)(*(short *)(iVar1 + 0x40) - *(short *)(unaff_A6 + 0x40))) {
    iVar1 = -1;
  }
LAB_00055bf2:
  *(int *)(unaff_A6 + 0x84) = iVar1;
  return;
}
