/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0029f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_29f2(void)

{
  ushort in_D0w;
  uint uVar1;

  uVar1 = candidate_29c6();
  uVar1 = (uVar1 & 0xffff) * (uint)in_D0w;
  return uVar1 * 0x10000 | uVar1 >> 0x10;
}
