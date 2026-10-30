/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01dd50.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1dd50(void)

{
  undefined2 in_D0w;
  undefined2 in_D1w;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined1 *)(iVar1 + 0x94) = 0;
  *(undefined2 *)(iVar1 + 0x6e) = in_D0w;
  *(undefined2 *)(iVar1 + 0x70) = in_D1w;
  *(undefined2 *)(iVar1 + 0x72) = unaff_D2w;
  *(undefined2 *)(iVar1 + 0x74) = unaff_D3w;
  return;
}
