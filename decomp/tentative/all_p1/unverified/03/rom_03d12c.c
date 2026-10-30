/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d12c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_3d12c(void)

{
  ushort unaff_D4w;
  ushort uVar1;
  ushort unaff_D7w;

  if (unaff_D7w < unaff_D4w) {
    __m68k_trap(0xf);
  }
  uVar1 = 0;
  do {
    uVar1 = uVar1 + 1;
  } while (unaff_D4w != uVar1);
  return 0;
}
