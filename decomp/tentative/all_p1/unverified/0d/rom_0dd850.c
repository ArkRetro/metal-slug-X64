/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd850.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000dd868: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000dd86a) */
/* WARNING: Removing unreachable block (ram,0x000dd8de) */
/* WARNING: Removing unreachable block (ram,0x000dd8f2) */
/* WARNING: Removing unreachable block (ram,0x000dd8ec) */
/* WARNING: Removing unreachable block (ram,0x000dd8f4) */
/* WARNING: Removing unreachable block (ram,0x000dd86c) */

void candidate_dd850(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A3;
  int unaff_A5;
  int unaff_A6;
  bool bVar2;

  if (*(ushort *)(unaff_A6 + 0x70) < 0x14) {
    *(ushort *)(unaff_A6 + 0x8c) = *(ushort *)(unaff_A6 + 0x70) * 0x10 + 0x100;
    *(undefined2 *)(unaff_A6 + 0x8a) = 0xa0;
    bVar2 = false;
    candidate_1af9a();
    iVar1 = extraout_A0_00;
    if (bVar2) {
      return;
    }
  }
  else {
    __m68k_trap(0xf);
    *(undefined2 *)(unaff_A3 + -2) = *(undefined2 *)(unaff_A5 + 0xdf);
    func_0x000dd8d1();
    func_0x000dd8db();
    iVar1 = extraout_A0;
  }
  *(undefined2 *)(unaff_A6 + 0x8a) = *(undefined2 *)(iVar1 + 0x3c);
  return;
}
