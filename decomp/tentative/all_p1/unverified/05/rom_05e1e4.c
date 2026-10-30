/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e1e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e1e4(void)

{
  int unaff_A6;

  candidate_1436a();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    candidate_1456a();
    if (*(int *)(unaff_A6 + 0x30) == 0) {
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 1;
    }
    return;
  }
  if (-1 < *(short *)(unaff_A6 + 0x34)) {
    *(undefined4 *)(unaff_A6 + 0x34) = 0xffff0000;
  }
  candidate_143dc();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
  }
  return;
}
