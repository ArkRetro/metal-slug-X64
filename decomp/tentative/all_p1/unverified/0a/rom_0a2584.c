/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2584.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2584(void)

{
  byte bVar1;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x92) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x92);
  bVar1 = *(byte *)(*(int *)(unaff_A6 + 0xc) + 0x60);
  if ((-1 < (char)bVar1) && (bVar1 != *(byte *)(unaff_A6 + 0x60))) {
    *(byte *)(unaff_A6 + 0x60) = bVar1;
    if (((bVar1 & 0xf) != 5) && ((bVar1 & 0xf) != 4)) {
      *(undefined1 *)(unaff_A6 + 0x86) = 0;
      *(undefined1 *)(unaff_A6 + 0x88) = 0x20;
    }
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0x61) = 0;
  }
  return;
}
