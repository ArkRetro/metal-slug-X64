/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c109c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c109c(void)

{
  ushort in_D0w;
  int iVar1;
  int unaff_A6;

  if (((in_D0w & 0x50) != 0) && ((*(byte *)(unaff_A6 + 0x9a) & 8) != 0)) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) ^ 1;
  }
  iVar1 = *(int *)(unaff_A6 + 0x34);
  candidate_143ee();
  if (-1 < iVar1) {
    if ((*(byte *)(unaff_A6 + 0x5f) & 2) != 0) {
      iVar1 = 0;
    }
    *(int *)(unaff_A6 + 0x34) = iVar1;
  }
  return;
}
