/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d545c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d545c(void)

{
  ushort uVar1;
  int unaff_A6;
  bool bVar2;

  *(short *)(unaff_A6 + 0x76) = *(short *)(unaff_A6 + 0x76) + -1;
  bVar2 = false;
  if (*(short *)(unaff_A6 + 0x76) < 1) {
    candidate_1affc();
    if (!bVar2) {
      uVar1 = candidate_29c6();
      *(ushort *)(unaff_A6 + 0x76) = (uVar1 & 0x3f) + 0x1e;
      return;
    }
  }
  return;
}
