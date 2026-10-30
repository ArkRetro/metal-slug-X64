/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ffc8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1ffc8(void)

{
  int unaff_A6;

  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x15) & 4) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfb;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 4;
  }
  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x15) & 2) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 2;
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
