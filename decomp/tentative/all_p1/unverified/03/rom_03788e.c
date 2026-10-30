/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03788e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3788e(void)

{
  ushort uVar1;
  int unaff_A6;
  bool bVar2;

  if (-1 < *(int *)(unaff_A6 + 0x72)) {
    uVar1 = *(short *)(*(int *)(unaff_A6 + 0x72) + 0x2c) - *(short *)(unaff_A6 + 0x2c);
    if ((short)uVar1 < 0) {
      uVar1 = -uVar1;
    }
    if (uVar1 < 9) {
      bVar2 = false;
      candidate_40bc();
      if (bVar2) {
        candidate_1b220();
      }
    }
  }
  return;
}
