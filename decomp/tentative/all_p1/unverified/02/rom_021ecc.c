/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021ecc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21ecc(void)

{
  undefined4 *unaff_A6;

  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  *(int *)((int)unaff_A6 + 0x62) = (int)*(short *)((int)unaff_A6 + 0x72) << 8;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
  }
  *unaff_A6 = 0x21f02;
  candidate_2223c();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_21f22;
  }
  candidate_220bc();
  return;
}
