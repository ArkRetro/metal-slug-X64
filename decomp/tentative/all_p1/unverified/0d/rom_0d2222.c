/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2222.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2222(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_d3462();
  candidate_d3220();
  candidate_d3268();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_d2266;
  }
  if (*(short *)((int)unaff_A6 + 0x6e) < 0x10) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x20;
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  bVar1 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x10;
  }
  candidate_d341e();
  if (bVar1) {
    *unaff_A6 = candidate_d231a;
  }
  return;
}
