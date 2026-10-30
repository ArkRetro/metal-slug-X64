/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x026d7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_26d7c(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_120f8();
  unaff_A6[0xd] = 0x80000;
  sVar1 = *(short *)(unaff_A6 + 0x10);
  candidate_1abc8();
  *(short *)(unaff_A6 + 0xb) = (sVar1 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
  candidate_1abc8();
  *unaff_A6 = 0x26db0;
  candidate_1ac38();
  if (0x160 < *(short *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_26dca;
  }
  candidate_427a();
  candidate_27bd4();
  return;
}
