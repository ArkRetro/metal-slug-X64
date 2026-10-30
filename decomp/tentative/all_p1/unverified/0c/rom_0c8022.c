/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8022.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c8022(void)

{
  int unaff_A5;

  if (-1 < *(short *)(unaff_A5 + 0x69c8)) {
    *(ushort *)(unaff_A5 + 0x69c8) = *(ushort *)(unaff_A5 + 0x69c8) | 3;
    return;
  }
  *(ushort *)(unaff_A5 + 0x69c8) = *(ushort *)(unaff_A5 + 0x69c8) & 0xfffc;
  *(uint *)(unaff_A5 + 0x69cc) = *(int *)(unaff_A5 + 0x69c8) + 0x8000U & 0xffff0000;
  return;
}
