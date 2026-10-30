/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08790a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8790a(void)

{
  int unaff_A6;
  undefined1 in_CF;

  candidate_1aeda();
  if ((bool)in_CF) {
    candidate_1af3c();
    if ((bool)in_CF) {
      *(byte *)(unaff_A6 + 0x8b) = *(byte *)(unaff_A6 + 0x8b) & 0xdf;
      func_0x0001af66();
      if ((bool)in_CF) {
        *(byte *)(unaff_A6 + 0x8b) = *(byte *)(unaff_A6 + 0x8b) & 0xef;
        return;
      }
      goto LAB_0008794c;
    }
  }
  *(byte *)(unaff_A6 + 0x8b) = *(byte *)(unaff_A6 + 0x8b) | 0x20;
LAB_0008794c:
  *(byte *)(unaff_A6 + 0x8b) = *(byte *)(unaff_A6 + 0x8b) | 0x10;
  return;
}
