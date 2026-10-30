/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x051220.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_51220(void)

{
  short sVar1;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x9d) < '\0') {
    return;
  }
  if ((*(byte *)(unaff_A6 + 0x5f) & 0x80) != 0) {
    return;
  }
  sVar1 = *(short *)(unaff_A6 + 0x90);
  *(short *)(unaff_A6 + 0x90) = *(short *)(unaff_A6 + 0x4e);
  if (sVar1 != *(short *)(unaff_A6 + 0x4e)) {
    if (*(short *)(unaff_A6 + 0x3c) < 0x180) {
      sVar1 = candidate_260c8();
      if (*(char *)(unaff_A6 + 0x9c) == '\x01') {
        sVar1 = candidate_260c8();
      }
      if (*(char *)(unaff_A6 + 0x9c) == '\a') {
        sVar1 = candidate_260c8();
      }
      if (*(char *)(unaff_A6 + 0x9c) == '\t') {
        sVar1 = candidate_260c8();
      }
      if (*(char *)(unaff_A6 + 0x9c) == '\n') {
        sVar1 = candidate_260c8();
      }
      if (*(char *)(unaff_A6 + 0x9c) == '\v') {
        sVar1 = candidate_260c8();
      }
      if (*(char *)(unaff_A6 + 0x9c) == '\x0e') {
        sVar1 = candidate_260c8();
      }
      if (*(char *)(unaff_A6 + 0x9c) == '\f') {
        sVar1 = candidate_260c8();
      }
      *(int *)(unaff_A6 + 0x28) = sVar1 * 0x100 + *(int *)(unaff_A6 + 0x28);
      *(undefined1 *)(unaff_A6 + 0x9c) = 0;
    }
    else {
      *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + 2;
    }
    if (*(char *)(unaff_A6 + 0xa2) == '\0') {
      candidate_120f8();
      *(undefined1 *)(unaff_A6 + 0xa2) = 3;
    }
  }
  return;
}
