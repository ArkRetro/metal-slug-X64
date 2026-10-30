/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a364.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a364(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x1e;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  *unaff_A6 = 0x9a376;
  candidate_9ac86();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
  if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
    *unaff_A6 = candidate_9a5ee;
  }
  bVar1 = false;
  candidate_c8bec();
  if (bVar1) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
