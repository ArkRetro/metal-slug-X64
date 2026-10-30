/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae748.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Instruction at (ram,0x000ae7d2) overlaps instruction at (ram,0x000ae7d0)
    */
/* WARNING: Control flow encountered bad instruction data */

short candidate_ae748(void)

{
  short sVar1;
  short in_D0w;
  short unaff_D4w;
  int extraout_A1;
  int iVar2;
  byte *unaff_A6;
  short *psVar3;
  short *psVar4;
  char cVar5;
  bool bVar6;
  bool bVar7;
  short sStack_2;

  if (*(ushort *)(unaff_A6 + 0x86) < 0xf2) {
LAB_000ae76c:
    psVar3 = &sStack_2;
    psVar4 = &sStack_2;
    sStack_2 = in_D0w;
    register0x0000003c = (BADSPACEBASE *)&sStack_2;
    if (*(ushort *)(unaff_A6 + 0x86) < 0x1e) goto LAB_000ae7f4;
    if (DAT_0010c2e7 == '\0') {
      iVar2 = 0xaadca;
    }
    else {
      iVar2 = 0xaae9e;
    }
    if (*(short *)(iVar2 + (short)((*(short *)(unaff_A6 + 0x86) + -0x1e) * 2)) == 0) {
      return in_D0w;
    }
    candidate_9f88();
    register0x0000003c = (BADSPACEBASE *)psVar3;
    if (DAT_0010c2e7 == '\0') goto code_r0x000ae7ca;
  }
  else {
    __m68k_trap(0xf);
    sVar1 = *(short *)(&stack0x00000065 + unaff_D4w * 4);
    bVar7 = SBORROW2(sVar1,7);
    sVar1 = sVar1 + -7;
    *(short *)(&stack0x00000065 + unaff_D4w * 4) = sVar1;
    cVar5 = sVar1 < 0;
    bVar6 = sVar1 == 0;
    sStack_2 = -0x18a6;
                    /* WARNING: Call to offcut address within same function */
    in_D0w = func_0x000ae7ce();
    if (!bVar7) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    psVar4 = (short *)register0x0000003c;
    if (bVar6 || bVar7 != (bool)cVar5) {
code_r0x000ae7ca:
      *(undefined4 *)((int)psVar4 + -4) = 0xae7d0;
      candidate_9ea8();
      register0x0000003c = (BADSPACEBASE *)psVar4;
      goto LAB_000ae7f4;
    }
    if (!bVar6 && bVar7 == (bool)cVar5) {
      *(short *)(extraout_A1 + 0x6d65) = *(short *)(extraout_A1 + 0x6d65) + 2;
      goto LAB_000ae76c;
    }
    *unaff_A6 = *unaff_A6 | 0xfa;
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0xae7e8;
  candidate_9ea8();
LAB_000ae7f4:
  sVar1 = *(short *)register0x0000003c;
  *(short *)(unaff_A6 + 0x86) = sVar1 + *(short *)(unaff_A6 + 0x86);
  return sVar1;
}
