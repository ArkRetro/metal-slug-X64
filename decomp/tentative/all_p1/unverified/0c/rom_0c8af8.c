/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8af8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

short candidate_c8af8(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  uint uVar4;

  sVar1 = 0x7026;
  DAT_00300001 = 0x26;
  for (sVar2 = 0x26; sVar2 != 0; sVar2 = sVar2 + -1) {
    uVar4 = CONCAT22(sVar1,0xff);
    for (sVar3 = 0x19; sVar3 != 0; sVar3 = sVar3 + -1) {
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
