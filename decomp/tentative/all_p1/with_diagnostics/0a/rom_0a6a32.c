/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6a32.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

char candidate_a6a32(void)

{
  char cVar1;

  cVar1 = candidate_19dca();
  if (cVar1 < '\x01') {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return cVar1 + -1;
}
