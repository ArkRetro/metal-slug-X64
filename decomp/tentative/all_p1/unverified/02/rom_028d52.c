/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028d52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28d52(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x3c);
  if ((*(byte *)(unaff_A6 + 0x7d) & 0x40) == 0) {
    if (*(short *)(unaff_A6 + 0x30) < 0) {
      if (sVar1 < 0x28) {
        return;
      }
    }
    else if (0x90 < sVar1) {
      return;
    }
  }
  else if (*(short *)(unaff_A6 + 0x30) < 0) {
    if (sVar1 < 0xb0) {
      return;
    }
  }
  else if (0x118 < sVar1) {
    return;
  }
  return;
}
