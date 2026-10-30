/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3ec0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3ec0(void)

{
  int unaff_A6;

  candidate_1abc8();
  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x15) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(*(int *)(unaff_A6 + 0xc) + 0x1e);
  return;
}
