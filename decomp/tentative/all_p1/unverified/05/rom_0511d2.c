/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0511d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_511d2(void)

{
  int unaff_A6;

  if (4 < *(ushort *)(unaff_A6 + 0x8e)) {
    return;
  }
  if (*(ushort *)(unaff_A6 + 0x4e) <= *(ushort *)(unaff_A6 + 0x8a)) {
    *(ushort *)(unaff_A6 + 0x8a) = *(ushort *)(unaff_A6 + 0x8a) - *(short *)(unaff_A6 + 0x8c);
    *(short *)(unaff_A6 + 0x8e) = *(short *)(unaff_A6 + 0x8e) + 1;
    candidate_4256();
    if (3 < *(ushort *)(unaff_A6 + 0x8e)) {
      *(undefined2 *)(unaff_A6 + 0x8a) = 0;
    }
  }
  return;
}
