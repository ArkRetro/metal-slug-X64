/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049324.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0004939d) */

void candidate_49324(void)

{
  int extraout_A0;
  int unaff_A6;
  bool bVar1;

  if (0x13 < *(ushort *)(unaff_A6 + 0x6e)) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *(ushort *)(unaff_A6 + 0x7a) = *(ushort *)(unaff_A6 + 0x6e) * 0x10 + 0x100;
  *(undefined2 *)(unaff_A6 + 0x78) = 0xa0;
  bVar1 = false;
  candidate_1af9a();
  if (!bVar1) {
    *(undefined2 *)(unaff_A6 + 0x78) = *(undefined2 *)(extraout_A0 + 0x3c);
  }
  *(int *)(unaff_A6 + 0x7c) = extraout_A0;
  return;
}
