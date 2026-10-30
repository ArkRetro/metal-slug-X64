/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0da272.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_da272(void)

{
  short in_D0w;
  ushort uVar1;
  int unaff_A6;

  uVar1 = in_D0w - *(short *)(unaff_A6 + 0x88);
  if (uVar1 != 0) {
    if ((uVar1 & 0x10) == 0) {
      uVar1 = 1;
    }
    else {
      uVar1 = 0xff;
    }
  }
  return uVar1;
}
