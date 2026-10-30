/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce420.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ce420(void)

{
  ushort uVar1;
  int unaff_A6;
  bool bVar2;

  uVar1 = *(ushort *)(unaff_A6 + 0x2c);
  bVar2 = 0xffd3 < uVar1;
  *(ushort *)(unaff_A6 + 0x2c) = uVar1 + 0x2c;
  candidate_427a();
  if (!bVar2) {
    *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + -0x2c;
    return;
  }
  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + -0x2c;
  return;
}
