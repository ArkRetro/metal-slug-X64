/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d31be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d31be(void)

{
  byte bVar1;
  int in_A0;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x77);
  *(char *)(unaff_A6 + 0x77) = *(char *)(unaff_A6 + 0x77) + '\x01';
  *(undefined2 *)(unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x7c) = *(undefined2 *)(in_A0 + (short)((bVar1 & 0xf) * 2));
  bVar1 = *(byte *)(unaff_A6 + 0x77) & 0xf;
  *(byte *)(unaff_A6 + 0x77) = bVar1;
  if (bVar1 != 0) {
    return;
  }
  return;
}
