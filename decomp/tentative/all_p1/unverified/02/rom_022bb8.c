/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x022bb8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_22bb8(void)

{
  undefined4 *unaff_A6;

  candidate_1da40();
  candidate_1da40();
  *(undefined2 *)(unaff_A6 + 0x25) = 0x30;
  *unaff_A6 = 0x22bdc;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x25) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 4;
    candidate_1da40();
    candidate_1da40();
    candidate_1da40();
    candidate_1da40();
    *unaff_A6 = candidate_22c36;
  }
  else {
    *(short *)(unaff_A6 + 0x25) = *(short *)(unaff_A6 + 0x25) + -1;
  }
  return;
}
