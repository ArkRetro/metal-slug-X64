/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0919f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_919f4(void)

{
  ushort uVar1;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x90) & 7) == 0) {
    uVar1 = (*(byte *)(unaff_A6 + 0x90) & 0x1f) >> 3;
  }
  else {
    uVar1 = 3;
  }
  return uVar1;
}
