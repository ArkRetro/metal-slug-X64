/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06feb0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6feb0(void)

{
  int extraout_A0;
  int unaff_A6;
  bool bVar1;

  if ((*(char *)(unaff_A6 + 0x82) != '\0') && (*(char *)(unaff_A6 + 0x83) == '\0')) {
    *(undefined1 *)(unaff_A6 + 0x82) = 0;
    bVar1 = false;
    candidate_aa956();
    if (bVar1) {
      candidate_40bc();
      candidate_1b242();
      *(undefined1 *)(extraout_A0 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
      *(undefined1 *)(extraout_A0 + 0x81) = 0;
      candidate_14a02();
      *(undefined1 *)(unaff_A6 + 0x83) = 0x20;
      return;
    }
  }
  return;
}
