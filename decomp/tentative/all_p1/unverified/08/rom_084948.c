/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x084948.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_84948(void)

{
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8820;
  candidate_405ea();
  *(undefined1 *)(unaff_A6 + 0x5a) = 5;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x5b) = *(byte *)(unaff_A6 + 0x5b) | 0x80;
  }
  candidate_406b6();
  return;
}
