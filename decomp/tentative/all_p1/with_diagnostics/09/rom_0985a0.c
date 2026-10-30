/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0985a0.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_985a0(void)

{
  char *pcVar1;
  short sVar2;
  short unaff_D3w;
  int extraout_A0;
  short *unaff_A2;
  int unaff_A6;

  if (5 < *(ushort *)(unaff_A6 + 0x78)) {
    __m68k_trap(0xf);
    pcVar1 = (char *)(*(int *)((int)unaff_A2 + 0x5f444f47) + unaff_D3w * 8);
    *pcVar1 = *pcVar1 + '\x02';
    *unaff_A2 = *unaff_A2 + 1;
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  for (sVar2 = 1; sVar2 < *(short *)(unaff_A6 + 0x78); sVar2 = sVar2 + 1) {
    candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x6e);
    *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(unaff_A6 + 0x70);
    *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)(unaff_A6 + 0x72);
    *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(unaff_A6 + 0x74);
    *(undefined2 *)(extraout_A0 + 0x76) = *(undefined2 *)(unaff_A6 + 0x76);
    *(undefined2 *)(extraout_A0 + 0x7a) = *(undefined2 *)(unaff_A6 + 0x7a);
    *(undefined1 *)(extraout_A0 + 0x69) = *(undefined1 *)(unaff_A6 + 0x69);
    *(short *)(extraout_A0 + 0x7c) = sVar2;
    candidate_1acf2();
  }
  *(undefined2 *)(unaff_A6 + 0x7c) = 0;
  return;
}
