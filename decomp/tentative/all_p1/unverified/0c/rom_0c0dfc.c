/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0dfc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0dfc(void)

{
  char *pcVar1;
  ushort in_D0w;
  char in_D1b;
  int unaff_A5;

  if (1 < in_D0w) {
    __m68k_trap(0xf);
  }
  if ((in_D1b == *(char *)(unaff_A5 + 0x471e + (int)(short)in_D0w)) &&
     (*(char *)(unaff_A5 + 0x471a + (int)(short)in_D0w) != '\0')) {
    pcVar1 = (char *)(unaff_A5 + 0x471a + (int)(short)in_D0w);
    *pcVar1 = *pcVar1 + -1;
  }
  return;
}
