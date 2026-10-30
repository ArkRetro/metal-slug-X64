/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06fe46.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_6fe46(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = ((char)((ushort)(*(short *)(unaff_A6 + 0x7e) + 0x80) >> 8) + '\x02') * '\x05' +
          (char)((ushort)(*(short *)(unaff_A6 + 0x7c) + 0x80) >> 8) + '\x02';
  if (0x18 < bVar1) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (bVar1 != *(byte *)(unaff_A6 + 0x87)) {
    *(byte *)(unaff_A6 + 0x87) = bVar1;
    candidate_4256();
  }
  return;
}
