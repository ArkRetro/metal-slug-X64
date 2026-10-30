/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0e9b14.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_e9b14(void)

{
  short in_D0w;
  byte *in_A0;

  *in_A0 = *in_A0 | 0x13;
  in_A0[-1] = in_A0[-1] | 0x13;
  in_A0[in_D0w] = in_A0[in_D0w] | 0x13;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
