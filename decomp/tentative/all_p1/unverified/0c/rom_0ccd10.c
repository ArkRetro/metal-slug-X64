/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccd10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccd10(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_ce448();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_ccd3a;
  }
  candidate_ce420();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    *unaff_A6 = candidate_ccd3a;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
