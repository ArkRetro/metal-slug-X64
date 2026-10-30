/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01aa10.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0001aa40) overlaps instruction at (ram,0x0001aa3e)
    */

short candidate_1aa10(void)

{
  ushort *puVar1;
  ushort in_D0w;
  ushort uVar2;
  short sVar3;
  int in_D1;
  ushort uVar4;
  short unaff_D2w;
  ushort unaff_D3w;
  int unaff_D4;
  short unaff_D7w;
  ushort *extraout_A0;
  ushort *puVar5;
  int unaff_A3;
  ushort *unaff_A4;
  int unaff_A5;
  int unaff_A6;
  undefined4 *puVar6;
  short *psVar7;
  char in_NF;
  bool in_ZF;
  bool in_VF;
  bool in_CF;
  undefined4 in_FPSR;

  puVar5 = *(ushort **)(unaff_A3 + -4);
  if (in_ZF || in_VF != (bool)in_NF) {
    if (in_ZF) goto LAB_0001aaa4;
    if (in_VF) {
      saveFPUStateFrame(*(undefined4 *)(unaff_A6 + 0x2d + unaff_D4 * 2));
      if (in_D1 < 0 || *piRam00006600 < in_D1) {
        __m68k_trap(6);
      }
      *(byte *)((int)puVar5 + -1) = *(byte *)((int)puVar5 + -1) | 0x2d;
      if (in_D1 < 0 || **(int **)((int)unaff_A4 + (short)unaff_D3w * 8 + 0x2d) < in_D1) {
        __m68k_trap(6);
      }
      if (in_D1 < 0 || **(int **)(unaff_A6 + 1 + (short)unaff_D3w * 4) < in_D1) {
        __m68k_trap(6);
      }
      uVar2 = in_D0w + unaff_D2w;
      if (unaff_D3w <= (ushort)(in_D0w + unaff_D2w)) {
        uVar2 = unaff_D3w;
      }
      *(ushort *)(unaff_A5 + 0x4334) = uVar2;
      return (short)in_D1 + (in_D0w - uVar2);
    }
  }
  else {
    if (!in_ZF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    if (in_ZF) {
      puVar1 = (ushort *)(unaff_A6 + -1 + (int)(short)in_D0w);
      *puVar1 = *puVar1 | 0x1b7c;
      if (in_D1 < 0 || *piRam00003028 < in_D1) {
        __m68k_trap(6);
      }
      *(byte *)puVar5 = *(byte *)puVar5 | 0x40;
      *unaff_A4 = *unaff_A4 | 0x6100;
      saveFPUStateFrame(*(undefined4 *)(puVar5 + -2));
      goto code_r0x0001aa58;
    }
    if (!in_VF) {
      if (in_VF == (bool)in_NF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (in_ZF) {
        halt_baddata();
      }
      if (in_VF == (bool)in_NF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (in_CF || in_ZF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      if (!in_CF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      unaff_A5 = 0x108000;
      candidate_1a95c();
      if (puVar5 != (ushort *)0x0) {
        while( true ) {
          *(undefined1 *)(unaff_A5 + 0x4338) = 0xff;
          if (puVar5[1] == 0) {
            puVar5 = (ushort *)((int)(short)*puVar5 + (int)puVar5);
            unaff_D7w = 0;
            *(undefined1 *)(unaff_A5 + 0x4338) = 0;
            uVar2 = *puVar5;
            uVar4 = puVar5[1];
            goto LAB_0001aa6c;
          }
          if (puVar5[1] == 3) break;
code_r0x0001aa58:
          candidate_1a97a();
          puVar5 = extraout_A0;
        }
      }
      goto LAB_0001aab2;
    }
  }
  if (in_VF == (bool)in_NF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_ZF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_VF == (bool)in_NF) {
    if (!(bool)((byte)((uint)in_FPSR >> 0x1a) & 1)) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
  }
  else {
    if (in_CF || in_ZF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    if (!in_CF) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    uVar2 = in_D0w | 8;
    while( true ) {
      unaff_D7w = unaff_D7w + 1;
      puVar5 = (ushort *)((int)(short)uVar2 + (int)puVar5);
      uVar2 = *puVar5;
      uVar4 = puVar5[1];
LAB_0001aa6c:
      if (uVar4 == 3) break;
      if (2 < uVar4) {
        __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      puVar6 = (undefined4 *)((int)register0x0000003c + -4);
      register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
      *puVar6 = puVar5;
    }
    while (unaff_D7w != 0) {
LAB_0001aaa4:
      psVar7 = (short *)((int)register0x0000003c + 2);
      *(short *)((int)register0x0000003c + 2) = unaff_D7w + -1;
      *(undefined4 *)((int)register0x0000003c + -2) = 0x1aaae;
      candidate_1a97a();
      register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + 4);
      unaff_D7w = *psVar7;
    }
  }
LAB_0001aab2:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x1aab6;
  candidate_1ab38();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x1aac0;
  sVar3 = candidate_a002();
  return sVar3;
}
