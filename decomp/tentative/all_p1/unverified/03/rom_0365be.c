/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0365be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_365be(void)

{
  short sVar1;
  ushort uVar2;
  short extraout_D1w;
  ushort uVar3;
  int unaff_A6;

  sVar1 = candidate_365a4();
  uVar2 = sVar1 - *(short *)(unaff_A6 + 0x28);
  if ((short)uVar2 < 0) {
    uVar2 = -uVar2;
  }
  uVar3 = extraout_D1w - *(short *)(unaff_A6 + 0x2c);
  if ((short)uVar3 < 0) {
    uVar3 = -uVar3;
  }
  if ((uVar2 < 4) && (uVar3 < 4)) {
    return;
  }
  return;
}
