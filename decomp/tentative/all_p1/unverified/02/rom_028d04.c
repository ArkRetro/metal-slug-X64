/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028d04.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28d04(void)

{
  ushort uVar1;
  int unaff_A6;

  uVar1 = *(ushort *)(unaff_A6 + 0x78);
  *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x78) + 1;
  uVar1 = uVar1 >> 4;
  if (3 < uVar1) {
    uVar1 = 3;
  }
  *(ushort *)(unaff_A6 + 0x26) = (uVar1 + 1) * 0x80;
  return;
}
