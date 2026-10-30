/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01dcce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1dcce(void)

{
  undefined2 in_D0w;
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
  *(undefined1 *)(iVar1 + 0x94) = 1;
  *(undefined2 *)(iVar1 + 0x6e) = 0;
  *(undefined2 *)(iVar1 + 0x70) = in_D0w;
  *(undefined2 *)(iVar1 + 0x72) = 0x3f;
  *(undefined2 *)(iVar1 + 0x74) = 0x1f;
  return;
}
