/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020b16.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20b16(void)

{
  short sVar1;
  short extraout_D1w;
  short unaff_D2w;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x6e) + -1;
  *(short *)(unaff_A6 + 0x6e) = sVar1;
  if (sVar1 == 0) {
    *(undefined2 *)(unaff_A6 + 0x6e) = 4;
    *(char *)(unaff_A6 + 0x60) = *(char *)(unaff_A6 + 0x60) + '\x01';
  }
  if ((*(byte *)(unaff_A6 + 0x60) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1a);
  }
  candidate_c8d68();
  if (extraout_D1w != -1) {
    if (*(char *)(unaff_A6 + 0x7c) == '\0') {
      if (unaff_D2w != *(short *)(unaff_A6 + 0x7a)) {
        *(short *)(unaff_A6 + 0x7a) = unaff_D2w;
        candidate_120f8();
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x7c) = 0;
      *(short *)(unaff_A6 + 0x7a) = unaff_D2w;
    }
    candidate_4256();
    candidate_20994();
  }
  return;
}
