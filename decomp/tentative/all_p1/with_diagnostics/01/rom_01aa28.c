/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01aa28.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_1aa28(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  short *in_A0;
  short *extraout_A0;
  short *psVar4;
  undefined4 *puVar5;
  short *psVar6;

  candidate_1a95c();
  if (in_A0 != (short *)0x0) {
    while( true ) {
      uRam0010c338 = 0xff;
      if (in_A0[1] == 0) break;
      if (in_A0[1] == 3) goto LAB_0001aab2;
      candidate_1a97a();
      in_A0 = extraout_A0;
    }
    psVar4 = (short *)((int)*in_A0 + (int)in_A0);
    sVar3 = 0;
    uRam0010c338 = 0;
    sVar1 = *psVar4;
    uVar2 = psVar4[1];
    while (uVar2 != 3) {
      if (2 < uVar2) {
        __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      puVar5 = (undefined4 *)((int)register0x0000003c + -4);
      register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
      *puVar5 = psVar4;
      sVar3 = sVar3 + 1;
      psVar4 = (short *)((int)sVar1 + (int)psVar4);
      sVar1 = *psVar4;
      uVar2 = psVar4[1];
    }
    while (sVar3 != 0) {
      psVar6 = (short *)((int)register0x0000003c + 2);
      *(short *)((int)register0x0000003c + 2) = sVar3 + -1;
      *(undefined4 *)((int)register0x0000003c + -2) = 0x1aaae;
      candidate_1a97a();
      register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + 4);
      sVar3 = *psVar6;
    }
  }
LAB_0001aab2:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x1aab6;
  candidate_1ab38();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x1aac0;
  candidate_a002();
  return;
}
