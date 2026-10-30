/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01da40.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1da40(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 in_A1;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 4) != 0) {
    *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) | 4;
  }
  *(undefined4 *)(iVar1 + 0x86) = in_A1;
  return;
}
