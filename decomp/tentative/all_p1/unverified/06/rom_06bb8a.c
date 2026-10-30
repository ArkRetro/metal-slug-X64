/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06bb8a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6bb8a(void)

{
  ushort uVar1;
  ushort uVar2;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x92) & 0x80) == 0) {
    return;
  }
  if (*(short *)(unaff_A6 + 0x86) != 0) {
    uVar2 = *(short *)(unaff_A6 + 0x86) -
            (short)((uint)(*(int *)(unaff_A6 + 0x3c) - *(int *)(unaff_A6 + 0x44)) >> 0x10);
    uVar1 = uVar2 ^ *(ushort *)(unaff_A6 + 0x86);
    *(ushort *)(unaff_A6 + 0x86) = uVar1;
    if (-1 < (short)uVar1) {
      *(ushort *)(unaff_A6 + 0x86) = uVar2;
      return;
    }
  }
  *(undefined2 *)(unaff_A6 + 0x86) = 0;
  return;
}
