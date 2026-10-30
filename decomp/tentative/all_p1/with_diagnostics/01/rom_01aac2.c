/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01aac2.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0001aa3e) */
/* WARNING: Removing unreachable block (ram,0x0001aa5e) */
/* WARNING: Removing unreachable block (ram,0x0001aa6c) */
/* WARNING: Removing unreachable block (ram,0x0001aa74) */
/* WARNING: Removing unreachable block (ram,0x0001aa78) */
/* WARNING: Removing unreachable block (ram,0x0001aa92) */
/* WARNING: Removing unreachable block (ram,0x0001aa9e) */
/* WARNING: Removing unreachable block (ram,0x0001aaa0) */
/* WARNING: Removing unreachable block (ram,0x0001aaa4) */
/* WARNING: Removing unreachable block (ram,0x0001aa50) */
/* WARNING: Removing unreachable block (ram,0x0001aa58) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1aac2(void)

{
  uint uVar1;
  uint uVar2;

  uVar1 = 0;
  while (uVar2 = uVar1, uVar2 < 0x80000000) {
    _DAT_003c0000 = uVar2;
    uVar1 = uVar2 + 0x10000;
  }
  candidate_c7a24();
  candidate_1a95c();
  candidate_1ab38();
  candidate_a002();
  return;
}
