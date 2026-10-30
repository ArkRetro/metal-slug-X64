/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0584ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_584ae(void)

{
  short sVar1;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x78) == 3) {
    return;
  }
  *(undefined2 *)(unaff_A6 + 0x7c) = 0xf;
  *(undefined2 *)(unaff_A6 + 0x82) = 300;
  *(short *)(unaff_A6 + 0x7a) = *(short *)(unaff_A6 + 0x7a) + 1;
  sVar1 = *(short *)(unaff_A6 + 0x7a);
  if (*(char *)(sVar1 + 0x585d0) == '\0') {
    *(undefined2 *)(unaff_A6 + 0x7a) = 0;
    sVar1 = 0;
  }
  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + (int)*(short *)(unaff_A6 + 0x78)) =
       *(undefined1 *)(sVar1 + 0x585d0);
  return;
}
