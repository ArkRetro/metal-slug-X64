/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0d34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0d34(void)

{
  char *pcVar1;
  ushort in_D0w;
  int unaff_A5;

  if (1 < in_D0w) {
    __m68k_trap(0xf);
  }
  pcVar1 = (char *)(unaff_A5 + 0x4718 + (int)(short)in_D0w);
  *pcVar1 = *pcVar1 + '\x01';
  return;
}
