/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068b08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68b08(void)

{
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x6e) == 2) {
    if (*(short *)(unaff_A6 + 0x72) < 0) {
      if ((short)(*(short *)(unaff_A6 + 0x28) - *(short *)(unaff_A6 + 0x8e)) <=
          *(short *)(unaff_A6 + 0x72)) {
        *(undefined2 *)(unaff_A6 + 0x6e) = 1;
      }
    }
    else if (*(short *)(unaff_A6 + 0x72) <=
             (short)(*(short *)(unaff_A6 + 0x28) - *(short *)(unaff_A6 + 0x8e))) {
      *(undefined2 *)(unaff_A6 + 0x6e) = 1;
    }
  }
  return;
}
