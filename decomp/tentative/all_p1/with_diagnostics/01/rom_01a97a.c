/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a97a.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0001aa40) overlaps instruction at (ram,0x0001aa3e)
    */
/* WARNING: Type propagation algorithm not settling */

void candidate_1a97a(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  short unaff_D7w;
  short *in_A0;
  int extraout_A0;
  short *extraout_A0_00;
  short *psVar4;
  int unaff_A3;
  int unaff_A5;
  undefined4 *puVar5;
  short *psVar6;
  undefined4 *puVar7;
  char cVar8;
  bool bVar9;
  undefined1 uVar10;
  bool bVar11;
  bool bVar12;
  int aiStack_8 [2];

  aiStack_8[1] = (int)in_A0 + (int)*in_A0;
  puVar7 = aiStack_8 + 1;
  if (in_A0[1] == 1) {
    sVar1 = candidate_1aaf4();
    if (sVar1 == 0) {
      candidate_a002();
      return;
    }
    __m68k_trap(0xf);
    cVar8 = '\0';
    bVar9 = false;
    bVar11 = false;
    bVar12 = false;
    candidate_1aa10();
    psVar4 = *(short **)(unaff_A3 + 0x72 + (int)unaff_D7w);
    if (bVar11) {
      if (bVar11 == (bool)cVar8) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (bVar9) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (bVar11 == (bool)cVar8) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (bVar12 || bVar9) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (!bVar12) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      unaff_A5 = 0x108000;
      candidate_1a95c();
      if (psVar4 != (short *)0x0) goto LAB_0001aa3e;
      goto LAB_0001aab2;
    }
    if (!bVar12) {
                    /* WARNING: Call to offcut address within same function */
      func_0x0001aa1d();
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
  }
  else if (in_A0[1] != 2) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  sVar1 = candidate_1aaf4();
  if (sVar1 == 0) {
    if (*(short *)(extraout_A0 + 4) == 0) {
      candidate_c6c8a();
    }
  }
  else {
    __m68k_trap(0xf);
    uVar10 = 0;
    bVar9 = false;
    bVar11 = false;
                    /* WARNING: Call to offcut address within same function */
    func_0x0001aa4a();
    if (bVar9) {
      while (puVar7 = aiStack_8 + 1, !(bool)uVar10) {
        candidate_1a97a();
        psVar4 = extraout_A0_00;
LAB_0001aa3e:
        *(undefined1 *)(unaff_A5 + 0x4338) = 0xff;
        if (psVar4[1] == 0) {
          psVar4 = (short *)((int)*psVar4 + (int)psVar4);
          sVar1 = 0;
          *(undefined1 *)(unaff_A5 + 0x4338) = 0;
          sVar2 = *psVar4;
          uVar3 = psVar4[1];
          puVar7 = aiStack_8 + 1;
          while (uVar3 != 3) {
            if (2 < uVar3) {
              __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
              halt_baddata();
            }
            puVar5 = puVar7 + 0xffffffff;
            puVar7 = puVar7 + 0xffffffff;
            *puVar5 = psVar4;
            sVar1 = sVar1 + 1;
            psVar4 = (short *)((int)sVar2 + (int)psVar4);
            sVar2 = *psVar4;
            uVar3 = psVar4[1];
          }
          while (sVar1 != 0) {
            psVar6 = (short *)((int)puVar7 + 2);
            *(short *)((int)puVar7 + 2) = sVar1 + -1;
            *(undefined4 *)((int)puVar7 + -2) = 0x1aaae;
            candidate_1a97a();
            puVar7 = (undefined4 *)((int)puVar7 + 4);
            sVar1 = *psVar6;
          }
          break;
        }
        uVar10 = psVar4[1] == 3;
      }
LAB_0001aab2:
      puVar7[0xffffffff] = 0x1aab6;
      candidate_1ab38();
      puVar7[0xffffffff] = 0x1aac0;
      candidate_a002();
      return;
    }
    if (!bVar11) {
                    /* WARNING: Call to offcut address within same function */
      func_0x0001aa57();
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
  }
  candidate_c6ce4();
  return;
}
