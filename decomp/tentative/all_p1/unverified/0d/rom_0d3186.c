/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3186.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3186(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  if (*(short *)(iVar1 + 0x3c) < 0x121) {
    *(byte *)(iVar1 + 0x5f) = *(byte *)(iVar1 + 0x5f) & 0xbf;
    candidate_c7c12();
    return;
  }
  *(byte *)(iVar1 + 0x5f) = *(byte *)(iVar1 + 0x5f) | 0x40;
  candidate_c7b0c();
  return;
}
