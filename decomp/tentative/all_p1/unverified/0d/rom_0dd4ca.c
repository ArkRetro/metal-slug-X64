/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd4ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_dd4ca(void)

{
  undefined4 in_D0;
  uint uVar1;
  int extraout_A0;
  int unaff_A6;

  uVar1 = CONCAT31((int3)(CONCAT22((short)((uint)in_D0 >> 0x10),_DAT_0010000e) >> 8),
                   (char)_DAT_0010000e) & 0xffffff03;
  if ((_DAT_0010000e & 3) == 0) {
    candidate_1da40();
    uVar1 = 0x28;
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      uVar1 = 0xffd8;
    }
    *(short *)(extraout_A0 + 0x28) = (short)uVar1 + *(short *)(extraout_A0 + 0x28);
    *(short *)(extraout_A0 + 0x2c) = *(short *)(extraout_A0 + 0x2c) + -0x18;
  }
  return uVar1;
}
