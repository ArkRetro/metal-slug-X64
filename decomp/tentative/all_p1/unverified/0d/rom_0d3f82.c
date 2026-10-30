/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3f82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3f82(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0xd3f98;
  if (*(int *)((int)unaff_A6 + 0x9e) != 0) {
    candidate_4e76();
    *(undefined4 *)((int)unaff_A6 + 0x9e) = 0;
  }
  candidate_d5566();
  candidate_1abc8();
  candidate_427a();
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    *unaff_A6 = candidate_d3fd8;
  }
  uVar1 = 0;
  candidate_d50a8();
  candidate_d50ec();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d40a6;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d41de;
  }
  return;
}
