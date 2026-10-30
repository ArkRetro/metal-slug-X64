/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f6de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f6de(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;

  uVar1 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x1d) = (uVar1 & 7) + 0xc;
  *(short *)(unaff_A6 + 0x21) = -*(short *)(unaff_A6 + 0x21);
  candidate_4256();
  *unaff_A6 = 0x3f70a;
  candidate_3faba();
  candidate_1abc8();
  candidate_1ad8c();
  unaff_A6[0xc] = *(short *)(unaff_A6 + 0x21) * 0x100 + unaff_A6[0xc];
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_3f778;
  }
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    if (-1 < (int)unaff_A6[0xc]) {
      *unaff_A6 = candidate_3f778;
    }
  }
  else if ((int)unaff_A6[0xc] < 1) {
    *unaff_A6 = candidate_3f778;
  }
  candidate_427a();
  candidate_3fada();
  return;
}
