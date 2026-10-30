/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d961a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d961a(void)

{
  ushort uVar1;
  int unaff_A6;

  uVar1 = (ushort)*(byte *)(unaff_A6 + 0xaa);
  if (0x7f < uVar1) {
    uVar1 = 0xff - uVar1;
  }
  *(ushort *)(unaff_A6 + 0x6e) = uVar1 >> 3;
  return;
}
