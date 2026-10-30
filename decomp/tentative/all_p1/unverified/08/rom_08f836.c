/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f836.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_8f836(void)

{
  undefined1 in_D0b;
  short sVar1;
  undefined1 uVar2;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x6e) = in_D0b;
  sVar1 = *(short *)(*(int *)(unaff_A6 + 0x7a) + 0x40) - *(short *)(unaff_A6 + 0x40);
  if ((-9 < sVar1) && (sVar1 < 0x29)) {
    sVar1 = *(short *)(*(int *)(unaff_A6 + 0x7a) + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      sVar1 = -sVar1;
    }
    uVar2 = 0;
    if (sVar1 < 1) {
      uVar2 = 8;
    }
    return CONCAT11((char)((ushort)sVar1 >> 8),uVar2);
  }
  return CONCAT11((char)((ushort)sVar1 >> 8),*(undefined1 *)(unaff_A6 + 0x6e));
}
