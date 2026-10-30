/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0042c0.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0000432c) overlaps instruction at (ram,0x0000432a)
    */
/* WARNING: Removing unreachable block (ram,0x000042b2) */
/* WARNING: Removing unreachable block (ram,0x000042b8) */
/* WARNING: Removing unreachable block (ram,0x0000432a) */

void thunk_FUN_000042f4(void)

{
  short sVar1;
  byte bVar2;
  int *in_A0;
  byte *pbVar3;
  byte *pbVar4;
  char *in_A1;
  int unaff_A5;

  *(char *)(unaff_A5 + -0x1c20) = *(char *)(unaff_A5 + -0x1c20) + '\x01';
  if (*in_A1 == '\0') {
    if (-1 < *(char *)(in_A0 + 1)) {
      return;
    }
    *(undefined2 *)(in_A0 + 1) = 0;
    pbVar3 = (byte *)*in_A0;
  }
  else {
    sVar1 = *(short *)(in_A0 + 1);
    if (sVar1 < 1) {
      *(char *)(in_A0 + 1) = *in_A1 + *(char *)(in_A0 + 1);
    }
    if (-1 < sVar1) {
      *(short *)(in_A0 + 1) = *(short *)(in_A0 + 1) - *(short *)((int)in_A0 + 6);
    }
    if (*(short *)(in_A0 + 1) < 1) {
      if (in_A1[1] == '(') {
        *(undefined1 *)(in_A0 + 1) = 0;
        return;
      }
      *in_A0 = (int)(in_A1 + 1);
    }
    if (-1 < sVar1) {
      return;
    }
    pbVar3 = (byte *)*in_A0;
  }
  while (0x33 < *pbVar3) {
    __m68k_trap(0xf);
    pbVar4 = pbVar3 + -1;
    *(short *)pbVar4 = *(short *)pbVar4 + 8;
    bVar2 = *pbVar4;
    (**(code **)pbVar3)();
    *in_A0 = (int)(pbVar3 + (short)(ushort)bVar2);
    pbVar3 = pbVar3 + (short)(ushort)bVar2;
  }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar3))();
  return;
}
