/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0577e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_577e2(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  undefined4 *unaff_A6;

  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x76) = 0;
  candidate_40bc();
  *(byte *)(extraout_A0_00 + 0x15) = *(byte *)(extraout_A0_00 + 0x15) | 1;
  *(undefined2 *)(extraout_A0_00 + 0x76) = 0x4000;
  candidate_40bc();
  *(byte *)(extraout_A0_01 + 0x15) = *(byte *)(extraout_A0_01 + 0x15) | 1;
  *(undefined2 *)(extraout_A0_01 + 0x76) = 0x8000;
  candidate_40bc();
  *(undefined2 *)(extraout_A0_02 + 0x76) = 0xc000;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x4000;
  *unaff_A6 = 0x57852;
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -0x100;
  return;
}
