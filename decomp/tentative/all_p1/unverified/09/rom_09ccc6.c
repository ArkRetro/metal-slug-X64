/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ccc6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ccc6(void)

{
  int unaff_A6;

  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  func_0x000143e4();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    func_0x000143dc();
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
      *(undefined4 *)(unaff_A6 + 0x30) = 0;
      *(undefined4 *)(unaff_A6 + 0x34) = 0;
    }
  }
  else {
    candidate_1456a();
  }
  return;
}
