/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e2a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_5e2a0(void)

{
  ushort uVar1;
  int unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_5e454();
  if ((bool)in_CF) {
    return 0;
  }
  bVar2 = *(byte *)(unaff_A6 + 0x8c) < 0xc0;
  if (*(byte *)(unaff_A6 + 0x8c) != 0xc0) {
    candidate_5e4cc();
    if (bVar2) {
      candidate_5e482();
      uVar1 = candidate_29c6();
      if ((uVar1 & 1) != 0) {
        return 2;
      }
    }
  }
  return 1;
}
