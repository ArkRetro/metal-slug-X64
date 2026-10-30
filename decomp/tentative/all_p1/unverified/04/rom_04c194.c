/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c194.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c194(void)

{
  undefined4 *unaff_A6;
  undefined8 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  if (*(char *)(unaff_A6[3] + 0xa4) == '\0') {
    candidate_260c8();
  }
  else {
    candidate_260c8();
  }
  uVar1 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar1;
  *unaff_A6 = candidate_4c298;
  return;
}
