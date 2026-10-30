/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d16dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d16dc(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x96;
  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0xd1706;
  candidate_d1b46();
  candidate_d1b28();
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *unaff_A6 = candidate_d173e;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar1 = false;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  candidate_19424();
  if (bVar1) {
    *unaff_A6 = candidate_d17f6;
  }
  return;
}
