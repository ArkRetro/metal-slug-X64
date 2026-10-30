/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055574.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55574(void)

{
  short sVar1;
  undefined2 uVar2;
  int unaff_A6;

  sVar1 = candidate_552f8();
  if (sVar1 == 3) {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x80) = uVar2;
  }
  if (sVar1 == 2) {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x80) = uVar2;
  }
  if (sVar1 < 2) {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x80) = uVar2;
  }
  sVar1 = candidate_29f2();
  if (sVar1 == 0) {
    *(short *)(unaff_A6 + 0x80) = *(short *)(unaff_A6 + 0x80) << 1;
  }
  return;
}
