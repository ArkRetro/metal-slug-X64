/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01aeda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_1aeda(void)

{
  undefined4 uVar1;
  int in_A0;
  int unaff_A5;

  if (unaff_A5 != 0x108000) {
    __m68k_trap(0xf);
  }
  uVar1 = 0;
  if ((in_A0 != 0x1014c4) && (uVar1 = 1, in_A0 != 0x101574)) {
    return 0;
  }
  return uVar1;
}
