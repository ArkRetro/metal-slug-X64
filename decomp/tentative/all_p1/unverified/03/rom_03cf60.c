/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03cf60.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

short candidate_3cf60(void)

{
  uint uVar1;
  short in_D0w;
  undefined2 in_D1w;
  short unaff_D2w;
  short unaff_D3w;
  short sVar2;
  uint uVar3;

  do {
    sVar2 = unaff_D3w;
    uVar1 = CONCAT22(in_D0w,in_D1w);
    do {
      uVar3 = uVar1;
      if (0x74ffffff < uVar3) {
        __m68k_trap(0xf);
      }
      sVar2 = sVar2 + -1;
      uVar1 = CONCAT31((int3)(uVar3 + 0x10000 >> 8),(char)(uVar3 + 0x10000) + '\x10');
    } while (sVar2 != 0);
    in_D1w = CONCAT11((char)((ushort)in_D1w >> 8),(char)in_D1w + '\x01');
    in_D0w = in_D0w + 0x20;
    unaff_D2w = unaff_D2w + -1;
  } while (unaff_D2w != 0);
  _DAT_003c0000 = CONCAT22(0x8200,(short)uVar3);
  return in_D0w;
}
