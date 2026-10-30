/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074c6e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_74c6e(void)

{
  undefined4 in_A1;
  int unaff_A5;
  int unaff_A6;

  *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) & 0xf7;
  if (unaff_A5 != 0x108000) {
    __m68k_trap(0xf);
  }
  *(undefined4 *)(unaff_A6 + 0x76) = in_A1;
  *(undefined2 *)(unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x7c) = 0x100;
  return;
}
