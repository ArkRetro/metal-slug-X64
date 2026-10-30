/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046b86.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46b86(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  candidate_1abc8();
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
