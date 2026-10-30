/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d2da.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

int candidate_8d2da(void)

{
  ushort in_D0w;

  if (0x59 < in_D0w) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return (uint)in_D0w * 0x18;
}
