/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e574.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e574(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x5f) & 1) == 0) {
    candidate_1affc();
    if (!(bool)in_CF) {
      sVar1 = candidate_5e770();
      if (sVar1 < *(short *)((short)((*(byte *)(unaff_A6 + 0x84) & 3) << 3) + 0x2cfe74)) {
        return;
      }
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x91) = 0x1e;
  }
  return;
}
