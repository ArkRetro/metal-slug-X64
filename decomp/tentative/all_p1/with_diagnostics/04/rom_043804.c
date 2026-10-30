/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043804.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

byte candidate_43804(void)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  int unaff_A6;

  cVar1 = candidate_2eb4();
  bVar2 = ((byte)(cVar1 + 4U) >> 3) - 8;
  if (*(char *)(unaff_A6 + 0x80) == '\x01') {
    bVar3 = 0xf;
    bVar4 = 0x11;
  }
  else {
    bVar3 = 0xc;
    bVar4 = 0x14;
  }
  if (bVar2 < bVar3) {
    bVar2 = bVar3;
  }
  if (bVar4 < bVar2) {
    bVar2 = bVar4;
  }
  if (8 < (byte)(bVar2 - 0xc)) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return bVar2 - 0xc;
}
