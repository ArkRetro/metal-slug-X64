/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cb8d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cb8d8(void)

{
  undefined2 uVar1;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x70) == 0) {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x6e) = uVar1;
  }
  else {
    *(short *)(unaff_A6 + 0x70) = *(short *)(unaff_A6 + 0x70) + -1;
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x6e) = uVar1;
  }
  return;
}
