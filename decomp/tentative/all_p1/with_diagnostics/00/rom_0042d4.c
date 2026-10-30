/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0042d4.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0000432c) overlaps instruction at (ram,0x0000432a)
    */
/* WARNING: Removing unreachable block (ram,0x000042b2) */
/* WARNING: Removing unreachable block (ram,0x000042b8) */
/* WARNING: Removing unreachable block (ram,0x00004322) */
/* WARNING: Removing unreachable block (ram,0x00004328) */
/* WARNING: Removing unreachable block (ram,0x00004330) */
/* WARNING: Removing unreachable block (ram,0x00004338) */
/* WARNING: Removing unreachable block (ram,0x00004352) */
/* WARNING: Removing unreachable block (ram,0x00004340) */
/* WARNING: Removing unreachable block (ram,0x00004342) */
/* WARNING: Removing unreachable block (ram,0x0000434e) */
/* WARNING: Removing unreachable block (ram,0x00004348) */
/* WARNING: Removing unreachable block (ram,0x0000432a) */

void thunk_FUN_000044da(void)

{
  byte *pbVar1;
  int *in_A0;
  byte *in_A1;

  while( true ) {
    pbVar1 = in_A1 + 1 + (short)(ushort)*in_A1;
    (**(code **)(in_A1 + 1))();
    *in_A0 = (int)pbVar1;
    if (*pbVar1 < 0x34) break;
    __m68k_trap(0xf);
    in_A1 = pbVar1 + -1;
    *(short *)in_A1 = *(short *)in_A1 + 8;
  }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar1))();
  return;
}
