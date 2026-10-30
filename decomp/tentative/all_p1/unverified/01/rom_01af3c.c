/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01af3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1af3c(void)

{
  short sVar1;
  int in_A0;
  short *in_A1;
  int unaff_A6;

  sVar1 = *(short *)(in_A0 + 0x40) - *(short *)(unaff_A6 + 0x40);
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    sVar1 = -sVar1;
  }
  if ((in_A1[1] <= sVar1) && (sVar1 <= (short)(in_A1[3] + in_A1[1]))) {
    sVar1 = *(short *)(in_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      sVar1 = -sVar1;
    }
    if ((*in_A1 <= sVar1) && (sVar1 <= (short)(in_A1[2] + *in_A1))) {
      return;
    }
  }
  return;
}
