/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09c67a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9c67a(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x1e;
  *unaff_A6 = 0x9c68c;
  candidate_1abc8();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
  if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
    *unaff_A6 = candidate_41ac;
  }
  bVar1 = false;
  candidate_19504();
  if (bVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
