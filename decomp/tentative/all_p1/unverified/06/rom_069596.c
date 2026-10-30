/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069596.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69596(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x9e) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x9e) = *(byte *)(unaff_A6 + 0x9e) | 1;
    candidate_411c();
    *(undefined1 *)(extraout_A0 + 0x9c) = *(undefined1 *)(unaff_A6 + 0x9c);
    candidate_1b220();
    if (*(char *)(unaff_A6 + 0x9d) < '\0') {
      *(byte *)(extraout_A0_00 + 0x15) = *(byte *)(extraout_A0_00 + 0x15) ^ 1;
    }
  }
  return;
}
