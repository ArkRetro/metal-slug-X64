/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05845a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5845a(void)

{
  short sVar1;
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x7c) = 0xf;
  *(undefined2 *)(unaff_A6 + 0x82) = 300;
  if (*(short *)(unaff_A6 + 0x78) != 3) {
    *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + (int)*(short *)(unaff_A6 + 0x78)) = uRam000585d0;
  }
  sVar1 = *(short *)(unaff_A6 + 0x78);
  *(short *)(unaff_A6 + 0x78) = sVar1 + -1;
  if (sVar1 == 0) {
    *(undefined2 *)(unaff_A6 + 0x78) = 0;
    *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + (int)*(short *)(unaff_A6 + 0x78)) = uRam000585d1;
  }
  if (*(short *)(unaff_A6 + 0x78) == 0) {
    *(undefined1 *)(unaff_A6 + 0x85) = 0;
  }
  for (sVar1 = 0;
      *(char *)(*(int *)(unaff_A6 + 0x72) + (int)*(short *)(unaff_A6 + 0x78)) !=
      *(char *)(sVar1 + 0x585d0); sVar1 = sVar1 + 1) {
  }
  *(short *)(unaff_A6 + 0x7a) = sVar1;
  return;
}
