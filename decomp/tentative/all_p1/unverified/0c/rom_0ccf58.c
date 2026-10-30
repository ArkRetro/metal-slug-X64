/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccf58.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccf58(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_ce448();
  if ((bool)in_CF) {
    *(undefined1 *)(unaff_A6 + 0x20) = 4;
    in_CF = 0;
  }
  candidate_ce420();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_ccf78;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
