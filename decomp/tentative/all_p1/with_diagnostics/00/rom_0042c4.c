/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0042c4.
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

void thunk_FUN_0000435c(void)

{
  byte bVar1;
  int *extraout_A0;
  byte *pbVar2;
  byte *pbVar3;
  byte *extraout_A1;

  candidate_4364();
  pbVar2 = extraout_A1;
  while (0x33 < *pbVar2) {
    __m68k_trap(0xf);
    pbVar3 = pbVar2 + -1;
    *(short *)pbVar3 = *(short *)pbVar3 + 8;
    bVar1 = *pbVar3;
    (**(code **)pbVar2)();
    *extraout_A0 = (int)(pbVar2 + (short)(ushort)bVar1);
    pbVar2 = pbVar2 + (short)(ushort)bVar1;
  }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar2))();
  return;
}
