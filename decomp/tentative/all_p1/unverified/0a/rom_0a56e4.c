/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a56e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a56e4(void)

{
  short sVar1;
  ushort uVar2;
  int unaff_A6;

  uVar2 = (*(short *)(unaff_A6 + 0x72) - *(short *)(unaff_A6 + 0x4e)) + *(short *)(unaff_A6 + 0x78);
  *(ushort *)(unaff_A6 + 0x78) = uVar2;
  if (99 < uVar2) {
    *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x78) + -100;
    candidate_a6be2();
  }
  sVar1 = *(short *)(unaff_A6 + 0x4e);
  if (sVar1 <= *(short *)(unaff_A6 + 0x98)) {
    *(short *)(unaff_A6 + 0x98) = sVar1;
  }
  sVar1 = sVar1 - *(short *)(unaff_A6 + 0x98);
  *(undefined2 *)(unaff_A6 + 0x98) = 0;
  *(short *)(unaff_A6 + 0x4e) = sVar1;
  if (sVar1 != *(short *)(unaff_A6 + 0x72)) {
    *(byte *)(unaff_A6 + 0x17) = *(byte *)(unaff_A6 + 0x17) | 6;
  }
  *(short *)(unaff_A6 + 0x72) = sVar1;
  if ((*(byte *)(unaff_A6 + 0x5f) & 0x40) != 0) {
    *(undefined1 *)(unaff_A6 + 0x17) = 0;
  }
  candidate_454c();
  if ('\0' < *(char *)(unaff_A6 + 0x17)) {
    *(char *)(unaff_A6 + 0x17) = *(char *)(unaff_A6 + 0x17) + -1;
  }
  return;
}
