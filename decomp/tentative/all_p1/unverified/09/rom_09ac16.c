/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ac16.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ac16(void)

{
  undefined2 uVar1;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x72) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  if (*(short *)(unaff_A6 + 0x6e) == 3) {
    uVar1 = candidate_29f2();
    *(undefined2 *)(unaff_A6 + 0x6e) = uVar1;
  }
  if (*(short *)(unaff_A6 + 0x78) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x20;
  }
  candidate_1436a();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
  *(undefined2 *)(unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8820;
  candidate_3c526();
  return;
}
