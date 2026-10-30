/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e720.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e720(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) & 0xfe;
    return;
  }
  *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) | 1;
  return;
}
