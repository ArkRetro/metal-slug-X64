/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x026d04.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_26d04(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_9bb0();
  candidate_4256();
  unaff_A6[0xd] = 0xffff0000;
  sVar1 = *(short *)(unaff_A6 + 0x10);
  candidate_1abc8();
  *(short *)(unaff_A6 + 0xb) = (sVar1 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
  candidate_1abc8();
  candidate_1da40();
  candidate_1da40();
  candidate_1dbaa();
  *unaff_A6 = 0x26d62;
  candidate_1ac38();
  if (*(short *)(unaff_A6 + 0x10) < 0xa0) {
    *unaff_A6 = candidate_26d7c;
  }
  candidate_427a();
  candidate_27bd4();
  return;
}
