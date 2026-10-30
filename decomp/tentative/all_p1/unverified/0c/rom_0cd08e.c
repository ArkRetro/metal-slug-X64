/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd08e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd08e(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined2 *)(unaff_A6 + 0x24) = *(undefined2 *)(unaff_A6 + 0xf);
  *(short *)(unaff_A6 + 0xf) = *(short *)(unaff_A6 + 0xf) - (*(short *)(unaff_A6 + 0xf) + -0x120);
  candidate_40bc();
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(short *)(iVar1 + 0x1e) = *(short *)(iVar1 + 0x1e) + 1;
  candidate_1db3e();
  candidate_1db3e();
  *(undefined2 *)(unaff_A6 + 0xf) = *(undefined2 *)(unaff_A6 + 0x24);
  *unaff_A6 = candidate_4188;
  candidate_1abc8();
  candidate_ce420();
  return;
}
