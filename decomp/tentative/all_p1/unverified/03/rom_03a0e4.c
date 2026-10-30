/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a0e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a0e4(void)

{
  undefined2 uVar1;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A6;
  bool bVar2;

  if ((*(short *)(unaff_A6 + 0x7a) != 0) &&
     (*(short *)(unaff_A6 + 0x7c) = *(short *)(unaff_A6 + 0x7c) + 1,
     *(short *)(unaff_A6 + 0x7a) == *(short *)(unaff_A6 + 0x7c))) {
    *(undefined2 *)(unaff_A6 + 0x7c) = 0;
    bVar2 = false;
    candidate_40bc();
    if (bVar2) {
      candidate_1b220();
    }
    uVar1 = candidate_3a196();
    *(undefined2 *)(extraout_A0 + 0x72) = uVar1;
    return;
  }
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  uVar1 = candidate_3a196();
  *(undefined2 *)(extraout_A0_00 + 0x72) = uVar1;
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  uVar1 = candidate_3a196();
  *(undefined2 *)(extraout_A0_01 + 0x72) = uVar1;
  return;
}
