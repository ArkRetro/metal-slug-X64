/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0e40b0.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_e40b0(void)

{
  byte *pbVar1;
  ushort in_D0w;
  int in_A0;
  int unaff_A3;

  *(undefined1 *)(unaff_A3 + -1) = *(undefined1 *)(unaff_A3 + -1);
  *(undefined1 *)(unaff_A3 + -2) = *(undefined1 *)(unaff_A3 + -2);
  pbVar1 = (byte *)(in_A0 + 0x34 + (int)(short)(in_D0w | 8));
  *pbVar1 = *pbVar1 | 8;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
