/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccc64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccc64(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined2 *)(unaff_A6 + 0x26) = 0xb4;
  *unaff_A6 = 0xccc70;
  bVar1 = false;
  candidate_ce448();
  if (bVar1) {
    *(undefined1 *)(unaff_A6 + 0x20) = 4;
  }
  candidate_ce420();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    *unaff_A6 = candidate_ccca8;
  }
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    *unaff_A6 = candidate_ccca8;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
