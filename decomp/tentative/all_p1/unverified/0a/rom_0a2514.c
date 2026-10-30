/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2514.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2514(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = *(byte *)(*(int *)(unaff_A6 + 0xc) + 0x88);
  if (bVar1 != *(byte *)(unaff_A6 + 0x88)) {
    bVar1 = bVar1 & 0x7f;
    if (bVar1 < 0x20) {
      bVar1 = 0x20;
    }
    if (0x60 < bVar1) {
      bVar1 = 0x60;
    }
    if ((char)bVar1 < *(char *)(unaff_A6 + 0x88)) {
      *(char *)(unaff_A6 + 0x88) = *(char *)(unaff_A6 + 0x88) + -1;
    }
    else {
      *(char *)(unaff_A6 + 0x88) = *(char *)(unaff_A6 + 0x88) + '\x01';
    }
    return;
  }
  return;
}
