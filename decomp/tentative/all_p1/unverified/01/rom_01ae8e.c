/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ae8e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

char candidate_1ae8e(void)

{
  char cVar1;
  int unaff_A5;

  if (unaff_A5 != 0x108000) {
    __m68k_trap(0xf);
  }
  cVar1 = *(char *)(unaff_A5 + 0x433c);
  if (*(char *)(unaff_A5 + 0x433c) == '\0') {
    return cVar1;
  }
  return cVar1;
}
