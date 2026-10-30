/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x036254.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_36254(void)

{
  int unaff_A6;
  undefined8 uVar1;

  candidate_29c6();
  uVar1 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x7a) = uVar1;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  if (-1 < *(int *)(unaff_A6 + 0x7a)) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  return;
}
