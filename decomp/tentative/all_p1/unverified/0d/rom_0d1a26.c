/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1a26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1a26(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined1 *)(unaff_A6[3] + 0x60) = 2;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -1;
  *(ushort *)((int)unaff_A6 + 0x7a) = *(byte *)(unaff_A6 + 0x1f) & 0xf;
  candidate_4256();
  *unaff_A6 = 0xd1a5c;
  candidate_d1b6c();
  candidate_d1b28();
  bVar1 = *(byte *)(unaff_A6[3] + 0x60);
  if (bVar1 == 3) {
    *unaff_A6 = candidate_d1a7c;
  }
  bVar2 = bVar1 < 3;
  candidate_19424();
  if (bVar2) {
    *unaff_A6 = candidate_d1ac6;
  }
  return;
}
