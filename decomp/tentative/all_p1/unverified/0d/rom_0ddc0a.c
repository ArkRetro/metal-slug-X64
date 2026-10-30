/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ddc0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ddc0a(void)

{
  undefined1 uVar1;
  int unaff_A6;

  if (0 < *(short *)(unaff_A6 + 0x90)) {
    *(short *)(unaff_A6 + 0x90) = *(short *)(unaff_A6 + 0x90) + -1;
    return;
  }
  if (*(char *)(unaff_A6 + 0x9d) == -1) {
    uVar1 = candidate_260c8();
    *(undefined1 *)(unaff_A6 + 0x9c) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined1 *)(unaff_A6 + 0x9d) = uVar1;
  }
  if ('\0' < *(char *)(unaff_A6 + 0x9c)) {
    *(char *)(unaff_A6 + 0x9c) = *(char *)(unaff_A6 + 0x9c) + -1;
    return;
  }
  if (*(char *)(unaff_A6 + 0x9d) == '\0') {
    *(undefined1 *)(unaff_A6 + 0xa4) = 1;
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x61) = 2;
  candidate_4256();
  uVar1 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x9c) = uVar1;
  *(char *)(unaff_A6 + 0x9d) = *(char *)(unaff_A6 + 0x9d) + -1;
  return;
}
