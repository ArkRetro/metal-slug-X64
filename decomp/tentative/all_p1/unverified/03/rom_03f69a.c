/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f69a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f69a(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;

  uVar1 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x1d) = (uVar1 & 7) + *(short *)(unaff_A6 + 0x1d);
  *unaff_A6 = 0x3f6ae;
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_3f6de;
  }
  candidate_3faba();
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_3fada();
  return;
}
