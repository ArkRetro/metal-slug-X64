/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09cdf2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9cdf2(void)

{
  int unaff_A6;

  if ((*(char *)(unaff_A6 + 0x8e) == '\0') &&
     ((*(byte *)(*(int *)(unaff_A6 + 0x72) + 0x90) & 2) != 0)) {
    *(byte *)(unaff_A6 + 0x90) = *(byte *)(unaff_A6 + 0x90) | 2;
    return;
  }
  *(byte *)(unaff_A6 + 0x90) = *(byte *)(unaff_A6 + 0x90) & 0xfd;
  return;
}
