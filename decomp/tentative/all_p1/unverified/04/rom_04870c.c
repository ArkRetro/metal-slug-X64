/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04870c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4870c(void)

{
  undefined4 *unaff_A6;

  *(char *)(unaff_A6 + 0x25) = *(char *)((int)unaff_A6 + 0x61) << 4;
  *unaff_A6 = 0x4871c;
  candidate_1abc8();
  if (*(char *)(unaff_A6 + 0x25) == '\0') {
    if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    candidate_40bc();
    *unaff_A6 = candidate_4876c;
  }
  else {
    *(char *)(unaff_A6 + 0x25) = *(char *)(unaff_A6 + 0x25) + -1;
  }
  candidate_490ae();
  return;
}
