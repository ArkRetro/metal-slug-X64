/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0042ec.
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

void thunk_FUN_000044bc(void)

{
  byte bVar1;
  ushort uVar2;
  undefined4 *in_A0;
  byte *pbVar3;
  byte *in_A1;
  byte *pbVar4;
  int unaff_A6;

  pbVar4 = in_A1 + 3;
  uVar2 = (ushort)*(byte *)(unaff_A6 + (short)(ushort)*in_A1) * 2;
  if (uVar2 < *(ushort *)(in_A1 + 1)) {
    pbVar4 = pbVar4 + *(short *)(pbVar4 + (short)uVar2);
  }
  pbVar4 = pbVar4 + (short)*(ushort *)(in_A1 + 1);
  while( true ) {
    *in_A0 = pbVar4;
    if (*pbVar4 < 0x34) break;
    __m68k_trap(0xf);
    pbVar3 = pbVar4 + -1;
    *(short *)pbVar3 = *(short *)pbVar3 + 8;
    bVar1 = *pbVar3;
    (**(code **)pbVar4)();
    pbVar4 = pbVar4 + (short)(ushort)bVar1;
  }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar4))();
  return;
}
