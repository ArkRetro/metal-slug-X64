/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0375ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_375ca(void)

{
  ushort uVar1;
  int unaff_A6;

  uVar1 = *(short *)(unaff_A6 + 0x28) - *(short *)(unaff_A6 + 0x8c);
  if ((short)uVar1 < 0) {
    uVar1 = -uVar1;
  }
  if (uVar1 < 5) {
    uVar1 = *(short *)(unaff_A6 + 0x2c) - *(short *)(unaff_A6 + 0x8e);
    if ((short)uVar1 < 0) {
      uVar1 = -uVar1;
    }
    if (uVar1 < 5) {
      return;
    }
  }
  return;
}
