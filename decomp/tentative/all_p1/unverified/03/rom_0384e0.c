/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0384e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_384e0(void)

{
  ushort uVar1;
  int in_A0;
  int unaff_A6;

  if ((*(byte *)(in_A0 + 0x5f) & 0x80) != 0) {
    return;
  }
  uVar1 = *(short *)(in_A0 + 0x28) - *(short *)(unaff_A6 + 0x28);
  if ((short)uVar1 < 0) {
    uVar1 = -uVar1;
  }
  if (uVar1 < 9) {
    uVar1 = *(short *)(in_A0 + 0x2c) - *(short *)(unaff_A6 + 0x2c);
    if ((short)uVar1 < 0) {
      uVar1 = -uVar1;
    }
    if (uVar1 < 6) {
      if (*(char *)(in_A0 + 0x9a) != '\x03') {
        *(byte *)(in_A0 + 0x5f) = *(byte *)(in_A0 + 0x5f) | 0x80;
      }
      if (*(char *)(unaff_A6 + 0x9a) != '\x03') {
        *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
      }
    }
  }
  return;
}
