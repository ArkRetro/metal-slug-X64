/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03fa74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3fa74(void)

{
  int unaff_A6;

  if (*(int *)(unaff_A6 + 0x30) != 0) {
    *(int *)(unaff_A6 + 0x28) = *(int *)(unaff_A6 + 0x30) + *(int *)(unaff_A6 + 0x28);
  }
  if (*(int *)(unaff_A6 + 0x34) != 0) {
    *(int *)(unaff_A6 + 0x2c) = *(int *)(unaff_A6 + 0x34) + *(int *)(unaff_A6 + 0x2c);
  }
  *(int *)(unaff_A6 + 0x30) = *(short *)(unaff_A6 + 0x84) * 0x100 + *(int *)(unaff_A6 + 0x30);
  *(int *)(unaff_A6 + 0x34) = *(short *)(unaff_A6 + 0x86) * 0x100 + *(int *)(unaff_A6 + 0x34);
  return;
}
