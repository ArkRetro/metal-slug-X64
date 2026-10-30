/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0422a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_422a0(void)

{
  short *psVar1;
  int unaff_A6;
  bool bVar2;

  bVar2 = *(short *)(*(int *)(unaff_A6 + 0x72) + 0x7c) == 0;
  if (bVar2) {
    candidate_446be();
    if (bVar2) {
      return;
    }
  }
  else {
    psVar1 = (short *)(*(int *)(unaff_A6 + 0x72) + 0x7c);
    *psVar1 = *psVar1 + -1;
  }
  return;
}
