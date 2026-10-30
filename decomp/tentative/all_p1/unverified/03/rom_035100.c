/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x035100.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_35100(void)

{
  uint in_D0;
  char cVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  if (((in_D0 & 0x40) != 0) && (*(char *)(unaff_A6 + 0x88) == '\0')) {
    cVar1 = func_0x000c07e0();
    if (cVar1 != '\0') {
      bVar2 = false;
      candidate_c07f8();
      if (!bVar2) {
        return;
      }
      candidate_c0808();
    }
    candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(unaff_A6 + 0x70);
    *(undefined1 *)(unaff_A6 + 0x88) = 0xff;
  }
  return;
}
