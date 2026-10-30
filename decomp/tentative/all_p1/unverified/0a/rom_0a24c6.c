/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a24c6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a24c6(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
    bVar1 = false;
    candidate_1af9a();
    if (!bVar1) {
      *unaff_A6 = *(undefined4 *)((short)((*(byte *)((int)unaff_A6 + 0x8a) & 3) << 2) + 0xa2d46);
    }
  }
  return;
}
