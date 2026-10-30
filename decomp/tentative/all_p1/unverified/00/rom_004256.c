/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004256.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4256(void)

{
  undefined2 in_D0w;
  undefined4 in_A1;
  int unaff_A5;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  if (unaff_A5 != 0x108000) {
    __m68k_trap(0xf);
  }
  *(undefined4 *)(unaff_A6 + 0x20) = in_A1;
  *(undefined2 *)(unaff_A6 + 0x24) = 0;
  *(undefined2 *)(unaff_A6 + 0x26) = in_D0w;
  return;
}
