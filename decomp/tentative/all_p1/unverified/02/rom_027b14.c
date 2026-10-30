/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027b14.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_27b14(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar1;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined1 *)(iVar1 + 0x8f) = *(undefined1 *)(unaff_A6 + 0x8f);
  *(undefined1 *)(iVar1 + 0x85) = 0x60;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_01;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_02;
  }
  *(undefined1 *)(iVar1 + 0x8f) = *(undefined1 *)(unaff_A6 + 0x8f);
  *(undefined1 *)(iVar1 + 0x85) = 0x20;
  return;
}
