/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d190c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d190c(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff0;
  candidate_9ea8();
  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0xd1952;
  candidate_d1b28();
  if (0x180 < *(short *)(unaff_A6 + 0xf)) {
    return;
  }
  *unaff_A6 = candidate_d196a;
  return;
}
