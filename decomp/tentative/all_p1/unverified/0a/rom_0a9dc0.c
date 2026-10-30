/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a9dc0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a9dc0(void)

{
  uint uVar1;
  short sVar2;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A6;
  undefined8 uVar3;

  if (*(short *)(unaff_A6 + 0x78) != 0) {
    sVar2 = *(short *)(unaff_A6 + 0x78) + -1;
    do {
      candidate_8cdb2();
      candidate_29c6();
      uVar3 = candidate_2e7c();
      *(undefined8 *)(extraout_A0 + 0x30) = uVar3;
      uVar1 = candidate_29c6();
      *(uint *)(extraout_A0_00 + 0x34) = (uVar1 & 0x1fff) * 0x10 + 0x50000;
      sVar2 = sVar2 + -1;
    } while (sVar2 != -1);
    return;
  }
  return;
}
