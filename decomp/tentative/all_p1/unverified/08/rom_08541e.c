/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08541e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8541e(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A6;
  bool bVar2;

  if (-1 < *(char *)(unaff_A6 + 0x8f)) {
    bVar2 = false;
    candidate_40bc();
    iVar1 = extraout_A0;
    if (bVar2) {
      candidate_1b220();
      bVar2 = true;
      iVar1 = extraout_A0_00;
    }
    if (bVar2) {
      *(undefined2 *)(iVar1 + 0x7a) = 0xffff;
      *(undefined1 *)(iVar1 + 0x8a) = *(undefined1 *)(unaff_A6 + 0x8a);
      *(undefined2 *)(iVar1 + 0xa8) = *(undefined2 *)(unaff_A6 + 0xa8);
      *(undefined2 *)(iVar1 + 0x82) = *(undefined2 *)(unaff_A6 + 0x82);
      *(undefined1 *)(iVar1 + 0x8f) = *(undefined1 *)(unaff_A6 + 0x8f);
      *(undefined1 *)(iVar1 + 0x84) = *(undefined1 *)(unaff_A6 + 0x84);
      *(char *)(unaff_A6 + 0x8f) = *(char *)(unaff_A6 + 0x8f) + -1;
      return;
    }
  }
  return;
}
