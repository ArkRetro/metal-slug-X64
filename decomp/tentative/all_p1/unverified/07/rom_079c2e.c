/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079c2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79c2e(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  bVar1 = *(byte *)((int)unaff_A6 + 0x8a) < 4;
  if (*(byte *)((int)unaff_A6 + 0x8a) == 4) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *unaff_A6 = candidate_79b6e;
    return;
  }
  candidate_4c24();
  if (bVar1) {
    if ((*(char *)((int)unaff_A6 + 0x8b) != '\x01') && (*(short *)((int)unaff_A6 + 0x4e) < 0x33)) {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 1;
      candidate_120f8();
      candidate_1da40();
      candidate_1da40();
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_79b14;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  return;
}
