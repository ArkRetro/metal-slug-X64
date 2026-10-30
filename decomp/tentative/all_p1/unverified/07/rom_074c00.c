/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074c00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_74c00(void)

{
  char *in_A0;
  int unaff_A6;

  *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) & 0x7f;
  if (*in_A0 != '\0') {
    *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) | 0x80;
  }
  *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) & 0xef;
  if (in_A0[1] != '\0') {
    *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) | 0x10;
  }
  candidate_4256();
  return;
}
