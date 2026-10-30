/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078fc4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78fc4(void)

{
  undefined4 *unaff_A6;

  if (*(char *)(unaff_A6[3] + 0x9b) == '\x02') {
    return;
  }
  candidate_1ad52();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xffff;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffa0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    *(short *)(unaff_A6 + 0x1c) = -*(short *)(unaff_A6 + 0x1c);
  }
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xffff;
  *(undefined2 *)(unaff_A6 + 0x1d) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *unaff_A6 = candidate_99374;
  return;
}
