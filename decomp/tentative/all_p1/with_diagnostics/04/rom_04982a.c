/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04982a.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x000498a7) */

void candidate_4982a(void)

{
  ushort uVar1;
  int unaff_A6;

  uVar1 = *(ushort *)(*(int *)(unaff_A6 + 0xc) + 0x6e);
  if (0x13 < uVar1) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *(ushort *)(unaff_A6 + 0x7a) = uVar1 * 0x10 + 0x100;
  *(undefined2 *)(unaff_A6 + 0x78) = 0xa0;
  return;
}
