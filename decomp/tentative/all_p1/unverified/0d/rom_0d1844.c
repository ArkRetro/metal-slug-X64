/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1844.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1844(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x864;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x864;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xd188a;
  if (*(short *)(unaff_A5 + 0x699e) == 0x1000) {
    candidate_4256();
    *unaff_A6 = 0xd18aa;
    if (*(short *)(unaff_A5 + 0x699e) != 0x1000) {
      *unaff_A6 = 0xd1874;
    }
  }
  candidate_d1b28();
  return;
}
