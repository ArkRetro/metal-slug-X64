/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c9254.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

short candidate_c9254(void)

{
  short sVar1;
  short in_D0w;
  short sVar2;
  short sVar3;
  uint uVar4;
  int unaff_A5;
  int unaff_A6;

  if (*(char *)(unaff_A5 + 0x69e1) != '\0') {
    *(undefined1 *)(unaff_A6 + 0x86) = 0;
    sVar1 = *(short *)(unaff_A6 + 0x74);
    DAT_00300001 = (char)sVar1;
    for (sVar2 = 0x10; sVar2 != 0; sVar2 = sVar2 + -1) {
      uVar4 = CONCAT22(sVar1,800);
      for (sVar3 = 3; sVar3 != 0; sVar3 = sVar3 + -1) {
        if (0x74ffffff < uVar4) {
          __m68k_trap(0xf);
        }
        uVar4 = uVar4 + 0x10000;
      }
      sVar1 = sVar1 + 0x20;
    }
    _DAT_003c0004 = 0;
    _DAT_003c0000 = 0x82000000;
    return sVar1;
  }
  return in_D0w;
}
