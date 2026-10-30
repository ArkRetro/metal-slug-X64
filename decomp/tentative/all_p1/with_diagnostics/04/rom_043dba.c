/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043dba.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_43dba(void)

{
  byte in_D0b;
  char in_D1b;
  byte bVar1;
  int unaff_A6;

  bVar1 = in_D1b - in_D0b;
  if ((char)bVar1 < '\0') {
    bVar1 = -bVar1;
  }
  if (8 < bVar1) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if ((*(byte *)(unaff_A6 + 0xa0) & 1) == 0) {
    in_D0b = -(in_D0b - 8);
  }
  if (8 < in_D0b) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return;
}
