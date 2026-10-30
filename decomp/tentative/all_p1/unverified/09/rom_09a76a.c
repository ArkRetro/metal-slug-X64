/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a76a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a76a(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_4c24();
  if ((bool)in_CF) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
    *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
    if (*(short *)((int)unaff_A6 + 0x72) < 1) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_9a7b2;
  }
  bVar1 = false;
  candidate_c8bec();
  if (bVar1) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
