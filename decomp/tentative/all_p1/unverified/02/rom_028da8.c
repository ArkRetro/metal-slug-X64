/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028da8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28da8(void)

{
  int unaff_A6;
  bool bVar1;

  *(short *)(unaff_A6 + 0x76) = *(short *)(unaff_A6 + 0x76) + -1;
  if (*(short *)(unaff_A6 + 0x76) < 1) {
    *(undefined2 *)(unaff_A6 + 0x76) = 0;
    bVar1 = false;
    candidate_1af9a();
    if (((!bVar1) && (0xf < *(short *)(unaff_A6 + 0x3c))) && (*(short *)(unaff_A6 + 0x3c) < 0x121))
    {
      *(undefined2 *)(unaff_A6 + 0x76) = 0;
      return;
    }
  }
  return;
}
