/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cce84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cce84(void)

{
  undefined1 uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  uVar1 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x72) = uVar1;
  *(undefined2 *)(unaff_A6 + 0x26) = 0x2d;
  *unaff_A6 = 0xccea0;
  bVar2 = false;
  candidate_ce448();
  if (bVar2) {
    *(undefined1 *)(unaff_A6 + 0x20) = 4;
  }
  candidate_ce420();
  if ((*(char *)(unaff_A6 + 0x28) == '\x01') && ('\0' < *(char *)((int)unaff_A6 + 0x72))) {
    candidate_ce72e();
  }
  if ((*(char *)((int)unaff_A6 + 0x72) < '\x01') &&
     (*(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1,
     *(short *)(unaff_A6 + 0x26) < 1)) {
    *unaff_A6 = candidate_ccefc;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_ccefc;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
