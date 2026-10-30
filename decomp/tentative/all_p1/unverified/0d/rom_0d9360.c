/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9360.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x000d9386) */

ushort candidate_d9360(void)

{
  short in_D0w;
  ushort uVar1;

  if (in_D0w < 0x71) {
    in_D0w = 0x70;
  }
  if (0xbf < in_D0w) {
    in_D0w = 0xc0;
  }
  uVar1 = (ushort)(in_D0w - 0x68U) >> 3;
  if (9 < uVar1) {
    uVar1 = 10;
  }
  return uVar1 & 0xfe;
}
