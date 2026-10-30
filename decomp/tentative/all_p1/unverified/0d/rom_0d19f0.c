/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d19f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d19f0(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_4256();
  *unaff_A6 = 0xd1a06;
  candidate_d1b6c();
  candidate_d1b28();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 1;
  if (*(ushort *)(unaff_A6 + 0xb) == 0x28) {
    *unaff_A6 = candidate_d1a26;
  }
  bVar1 = *(ushort *)(unaff_A6 + 0xb) < 0x28;
  candidate_19424();
  if (bVar1) {
    *unaff_A6 = candidate_d1ac6;
  }
  return;
}
