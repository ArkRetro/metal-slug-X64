/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096728.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96728(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A6;
  bool bVar1;

  if (*(char *)(unaff_A6 + 0x8a) == '\0') {
    bVar1 = false;
    candidate_aa956();
    if (bVar1) {
      candidate_40bc();
      candidate_1b220();
      bVar1 = false;
      candidate_40bc();
      if (bVar1) {
        *(char *)(unaff_A6 + 0x8a) = *(char *)(unaff_A6 + 0x8a) + '\x01';
        *(undefined2 *)(extraout_A0 + 0x6e) = 0xc000;
        candidate_1b220();
        *(undefined1 *)(extraout_A0_00 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
        candidate_120f8();
      }
    }
  }
  return;
}
