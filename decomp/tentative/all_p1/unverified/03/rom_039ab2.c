/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039ab2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39ab2(void)

{
  int unaff_A6;
  bool bVar1;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    *(int *)(unaff_A6 + 0x34) =
         (-0xc0000 - *(int *)(unaff_A6 + 0x34) >> 4) + *(int *)(unaff_A6 + 0x34);
    *(int *)(unaff_A6 + 0x30) =
         (*(int *)(unaff_A6 + 0x7e) - *(int *)(unaff_A6 + 0x30) >> 1) + *(int *)(unaff_A6 + 0x30);
    *(undefined4 *)(unaff_A6 + 0x62) = *(undefined4 *)(unaff_A6 + 0x30);
    func_0x000143e4();
  }
  else {
    *(int *)(unaff_A6 + 0x62) =
         (*(int *)(unaff_A6 + 0x7e) - *(int *)(unaff_A6 + 0x62) >> 3) + *(int *)(unaff_A6 + 0x62);
    candidate_1456a();
  }
  bVar1 = (*(byte *)(unaff_A6 + 0x15) & 0x40) != 0;
  *(char *)(unaff_A6 + 0x8f) = -bVar1;
  if (bVar1) {
    *(undefined4 *)(unaff_A6 + 0x7e) = 0;
  }
  return;
}
