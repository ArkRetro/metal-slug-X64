/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d66e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d66e(void)

{
  uint uVar1;
  int unaff_A6;

  uVar1 = *(uint *)(unaff_A6 + 0x62);
  if ((*(byte *)(unaff_A6 + 0x60) & 1) != 0) {
    uVar1 = uVar1 * 2;
  }
  if (*(short *)(unaff_A6 + 0x8a) < *(short *)(unaff_A6 + 0x3c)) {
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) - (uVar1 >> 3);
    if (-*(int *)(unaff_A6 + 0x30) != uVar1 && *(int *)(unaff_A6 + 0x30) <= (int)-uVar1) {
      *(uint *)(unaff_A6 + 0x30) = -uVar1;
    }
  }
  else {
    *(int *)(unaff_A6 + 0x30) = (uVar1 >> 3) + *(int *)(unaff_A6 + 0x30);
    if ((int)uVar1 < *(int *)(unaff_A6 + 0x30)) {
      *(uint *)(unaff_A6 + 0x30) = uVar1;
    }
  }
  return;
}
