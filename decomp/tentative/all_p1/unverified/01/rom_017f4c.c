/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x017f4c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_17f4c(void)

{
  int unaff_A5;

  candidate_17fd0();
  if (*(ushort *)(unaff_A5 + 0x42fc) < 0x16) {
    if (*(short *)(unaff_A5 + -0x6c8c) != 0) {
      __m68k_trap(0xf);
    }
    candidate_4126();
    *(undefined4 *)(unaff_A5 + -0x6c9c) = 0x18282;
    return;
  }
  return;
}
