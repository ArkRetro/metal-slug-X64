/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004486.
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

void FUN_00004486(void)

{
  byte bVar1;
  undefined4 *in_A0;
  byte *pbVar2;
  byte *pbVar3;
  int in_A1;

  candidate_120f8();
  pbVar3 = (byte *)(in_A1 + 3);
  while( true ) {
    *in_A0 = pbVar3;
    if (*pbVar3 < 0x34) break;
    __m68k_trap(0xf);
    pbVar2 = pbVar3 + -1;
    *(short *)pbVar2 = *(short *)pbVar2 + 8;
    bVar1 = *pbVar2;
    (**(code **)pbVar3)();
    pbVar3 = pbVar3 + (short)(ushort)bVar1;
  }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar3))();
  return;
}
