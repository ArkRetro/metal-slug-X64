/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3462.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_d3462(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;

  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + 0x38;
  sVar1 = candidate_1b12e();
  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + -0x38;
  sVar1 = (short)(sVar1 + -0x24) >> 3;
  if (sVar1 != *(short *)(unaff_A6 + 0x8e)) {
    sVar2 = 1;
    if (sVar1 <= *(short *)(unaff_A6 + 0x8e)) {
      sVar2 = -1;
    }
    sVar1 = *(short *)(unaff_A6 + 0x8e) + sVar2;
    if (sVar1 < 0) {
      sVar2 = 0;
    }
    if (6 < sVar1) {
      sVar2 = 0;
    }
    *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x74) = 0;
    return sVar2;
  }
  *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x74) = 1;
  return 0;
}
