/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0042d0.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x000042d2) overlaps instruction at (ram,0x000042d0)
    */
/* WARNING: Removing unreachable block (ram,0x000042b2) */
/* WARNING: Removing unreachable block (ram,0x000042b8) */
/* WARNING: Removing unreachable block (ram,0x000044ac) */
/* WARNING: Removing unreachable block (ram,0x000044b6) */
/* WARNING: Removing unreachable block (ram,0x000044b8) */
/* WARNING: Removing unreachable block (ram,0x00004440) */
/* WARNING: Removing unreachable block (ram,0x00004444) */
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
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void thunk_FUN_00004412(void)

{
  ushort uVar1;
  byte bVar2;
  ushort uVar3;
  int *extraout_A0;
  byte *pbVar4;
  byte *pbVar5;
  byte *in_A1;
  int unaff_A6;

  uVar1 = *(ushort *)(in_A1 + 1);
  uVar3 = (ushort)*(byte *)(unaff_A6 + (short)(ushort)*in_A1) << 3;
  if (uVar1 <= uVar3) {
    uVar3 = uVar1 - 8;
  }
  if (in_A1[(short)uVar3 + 3] != 4) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  candidate_4364();
  pbVar4 = in_A1 + (short)uVar1 + 3;
  while (0x33 < *pbVar4) {
    __m68k_trap(0xf);
    pbVar5 = pbVar4 + -1;
    *(short *)pbVar5 = *(short *)pbVar5 + 8;
    bVar2 = *pbVar5;
    (**(code **)pbVar4)();
    *extraout_A0 = (int)(pbVar4 + (short)(ushort)bVar2);
    pbVar4 = pbVar4 + (short)(ushort)bVar2;
  }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar4))();
  return;
}
