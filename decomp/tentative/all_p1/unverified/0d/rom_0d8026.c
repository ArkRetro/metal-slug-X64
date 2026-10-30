/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8026.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8026(void)

{
  undefined4 *unaff_A6;

  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x2d;
  *(undefined1 *)((int)unaff_A6 + 0x85) = 2;
  *(undefined1 *)(unaff_A6 + 0x22) = 8;
  *unaff_A6 = 0xd8042;
  candidate_1abc8();
  candidate_d90d2();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  if ((*(short *)((int)unaff_A6 + 0x76) < 1) && ((*(byte *)((int)unaff_A6 + 0x61) & 0x10) != 0)) {
    *unaff_A6 = candidate_d806c;
  }
  return;
}
