/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3436.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_d3436(void)

{
  int unaff_A6;

  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x70) & 1) != 0) {
    return 1;
  }
  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x70) & 2) == 0) {
    return 0;
  }
  return 2;
}
