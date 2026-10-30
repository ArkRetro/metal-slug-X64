/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020c36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20c36(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_20994();
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    sVar1 = *(short *)((int)unaff_A6 + 0x6e) + -1;
    *(short *)((int)unaff_A6 + 0x6e) = sVar1;
    if (sVar1 == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
    }
  }
  else if (*(short *)(unaff_A6 + 0x1c) == 0) {
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -2;
    *(undefined2 *)(unaff_A6 + 0x1c) = 0xffff;
    candidate_120f8();
  }
  else {
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 10;
    if (0x16f < *(short *)(unaff_A6 + 0xb)) {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 2;
    }
    if (0x1af < *(short *)(unaff_A6 + 0xb)) {
      *unaff_A6 = candidate_20c9c;
    }
  }
  return;
}
