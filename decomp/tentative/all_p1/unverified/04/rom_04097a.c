/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04097a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4097a(void)

{
  uint unaff_D6;
  int unaff_A6;
  bool bVar1;

  if ((*(byte *)(unaff_A6 + 0x5f) & 0x40) == 0) {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x40;
    *(undefined1 *)(unaff_A6 + 0x6e) = 0;
  }
  *(undefined4 *)(unaff_A6 + 0x50) = 0;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
  candidate_405ea();
  *(undefined4 *)(unaff_A6 + 0x50) = 0;
  candidate_19dfc();
  candidate_1495c();
  bVar1 = (unaff_D6 & 8) != 0;
  *(char *)(unaff_A6 + 0x72) = -bVar1;
  if (bVar1) {
    *(undefined2 *)(unaff_A6 + 0x66) = 0xfff2;
    *(undefined2 *)(unaff_A6 + 0x1e) = 0xfff2;
  }
  *(char *)(unaff_A6 + 0x70) = -((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0);
  *(undefined4 *)(unaff_A6 + 0x76) = 0;
  *(undefined4 *)(unaff_A6 + 0x62) = 0;
  *(undefined1 *)(unaff_A6 + 0x71) = 0;
  *(undefined1 *)(unaff_A6 + 0x6f) = 0;
  *(undefined1 *)(unaff_A6 + 0x74) = 0;
  return;
}
