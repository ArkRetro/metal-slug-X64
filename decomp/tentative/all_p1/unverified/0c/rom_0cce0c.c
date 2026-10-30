/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cce0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cce0c(void)

{
  undefined4 *unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x20) = 4;
  *(undefined2 *)(unaff_A6 + 0x26) = 0x1e;
  *unaff_A6 = 0xcce1e;
  candidate_ce448();
  candidate_ce420();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    *unaff_A6 = candidate_cca24;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
