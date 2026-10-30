/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a3f6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a3f6(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  sVar1 = *(short *)(unaff_A6 + 0x1e) + -1;
  *(short *)(unaff_A6 + 0x1e) = sVar1;
  if (sVar1 < 0) {
    candidate_8a5dc();
  }
  if (*(short *)((int)unaff_A6 + 0x76) == 2) {
    return;
  }
  uVar2 = sRam00101500 - *(short *)(unaff_A6 + 0xf);
  if ((short)uVar2 < 0) {
    uVar2 = -uVar2;
  }
  if (0x10 < uVar2) {
    uVar2 = sRam001015b0 - *(short *)(unaff_A6 + 0xf);
    if ((short)uVar2 < 0) {
      uVar2 = -uVar2;
    }
    if (0x10 < uVar2) {
      return;
    }
  }
  bVar3 = uVar2 < 0x10;
  candidate_1affc();
  if (bVar3) {
    return;
  }
  if ((0x17 < *(short *)(unaff_A6 + 0xf)) && (*(short *)(unaff_A6 + 0xf) < 0x129)) {
    *unaff_A6 = candidate_8a00a;
  }
  return;
}
