/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x044598.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_44598(void)

{
  byte bVar1;
  int extraout_A0;
  undefined4 *unaff_A6;

  candidate_1abc8();
  bVar1 = *(byte *)((int)unaff_A6 + 0x8d);
  *(byte *)((int)unaff_A6 + 0x8d) = bVar1 - *(byte *)(unaff_A6 + 0x23);
  if (bVar1 < *(byte *)(unaff_A6 + 0x23)) {
    candidate_1da40();
    *(undefined2 *)(extraout_A0 + 0x66) = 400;
    *(undefined2 *)(extraout_A0 + 0x1e) = 400;
    if ((*(short *)((int)unaff_A6 + 0x8a) != 0) &&
       (*(char *)(*(int *)((int)unaff_A6 + 0x72) + 0x8e) == '\0')) {
      candidate_120f8();
    }
  }
  bVar1 = *(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x6e +
                   (int)(short)(ushort)*(byte *)((int)unaff_A6 + 0x83));
  if (bVar1 != *(byte *)(unaff_A6 + 0x21)) {
    *(byte *)(unaff_A6 + 0x21) = bVar1;
    *unaff_A6 = *(undefined4 *)
                 (*(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x83) << 2) + 0x453a6) +
                 (int)(short)((bVar1 & 0x7f) * 4));
  }
  *(undefined1 *)(unaff_A6 + 0x20) = *(undefined1 *)(*(int *)((int)unaff_A6 + 0x6e) + 0x80);
  return;
}
