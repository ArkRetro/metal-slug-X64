/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c90aa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c90aa(void)

{
  char cVar1;
  int unaff_A5;

  cVar1 = *(char *)(unaff_A5 + 0x69de);
  *(char *)(unaff_A5 + 0x69de) = cVar1 + -1;
  if (cVar1 == '\0') {
    __m68k_trap(0xf);
  }
  return;
}
