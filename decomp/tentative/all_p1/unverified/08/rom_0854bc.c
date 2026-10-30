/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0854bc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_854bc(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    bVar2 = true;
    iVar1 = extraout_A0_00;
  }
  if (bVar2) {
    *(undefined4 *)(iVar1 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    *(undefined2 *)(iVar1 + 0x82) = *(undefined2 *)(unaff_A6 + 0x82);
  }
  return;
}
