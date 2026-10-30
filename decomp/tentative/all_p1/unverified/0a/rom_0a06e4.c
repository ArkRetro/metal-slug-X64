/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a06e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a06e4(void)

{
  int extraout_A0;
  int iVar1;
  int unaff_A6;
  undefined1 in_CF;

  candidate_a0630();
  iVar1 = extraout_A0;
  if (!(bool)in_CF) {
    iVar1 = *(int *)(unaff_A6 + 0x6e);
  }
  if (iVar1 != 0) {
    if (*(short *)(iVar1 + 0x3c) < *(short *)(unaff_A6 + 0x3c)) {
      if ((*(byte *)(unaff_A6 + 0x90) & 1) != 0) {
        return;
      }
    }
    else if ((*(byte *)(unaff_A6 + 0x90) & 1) == 0) {
      return;
    }
  }
  return;
}
