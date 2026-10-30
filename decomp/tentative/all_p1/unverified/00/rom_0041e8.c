/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0041e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_41e8(void)

{
  int in_A0;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x10) != 0) {
    __m68k_trap(0xf);
  }
  if (*(short *)(in_A0 + 0x10) != 0) {
    __m68k_trap(0xf);
  }
  candidate_4192();
  return;
}
