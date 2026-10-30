/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x067fc0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_67fc0(void)

{
  undefined4 *unaff_A6;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  candidate_4256();
  candidate_1ad6e();
  *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *unaff_A6 = candidate_68020;
  return;
}
