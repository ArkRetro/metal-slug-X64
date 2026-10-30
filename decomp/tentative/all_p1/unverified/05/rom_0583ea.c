/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0583ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_583ea(void)

{
  short sVar1;
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x7c) = 0xf;
  *(undefined2 *)(unaff_A6 + 0x82) = 300;
  if (*(char *)(*(short *)(unaff_A6 + 0x7a) + 0x585d0) == '\x7f') {
    *(undefined2 *)(unaff_A6 + 0x7a) = 0;
    *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + (int)*(short *)(unaff_A6 + 0x78)) = uRam000585d0;
    return;
  }
  *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x78) + 1;
  if (3 < *(short *)(unaff_A6 + 0x78)) {
    return;
  }
  if (*(short *)(unaff_A6 + 0x78) != 3) {
    if (*(short *)(unaff_A6 + 0x78) != 0) {
      sVar1 = *(short *)(unaff_A6 + 0x78) + -1;
      *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 1 + (int)sVar1) =
           *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + (int)sVar1);
    }
    candidate_58530();
  }
  return;
}
