/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0820f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_820f8(void)

{
  int unaff_A6;
  bool bVar1;

  if (*(char *)(unaff_A6 + 0x60) != '\0') {
    return;
  }
  if (0 < *(short *)(unaff_A6 + 0x8e)) {
    *(short *)(unaff_A6 + 0x8e) = *(short *)(unaff_A6 + 0x8e) + -1;
    return;
  }
  bVar1 = *(byte *)(unaff_A6 + 0x9c) < 3;
  if (*(byte *)(unaff_A6 + 0x9c) == 3) {
    return;
  }
  candidate_81e6e();
  if (!bVar1) {
    return;
  }
  candidate_40bc();
  *(undefined1 *)(unaff_A6 + 0x60) = 1;
  return;
}
