/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x070fd0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_70fd0(void)

{
  int extraout_A0;
  int unaff_A6;
  bool bVar1;

  if ((*(char *)(unaff_A6 + 0x89) != '\0') && (*(char *)(unaff_A6 + 0x8a) == '\0')) {
    *(undefined1 *)(unaff_A6 + 0x89) = 0;
    bVar1 = false;
    candidate_aa956();
    if (bVar1) {
      candidate_40bc();
      *(undefined1 *)(extraout_A0 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
      candidate_14a02();
      *(undefined1 *)(unaff_A6 + 0x8a) = 0x20;
      return;
    }
  }
  return;
}
