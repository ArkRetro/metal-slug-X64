/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043bc0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43bc0(void)

{
  short sVar1;
  int in_A0;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x92);
  if ((*(byte *)(unaff_A6 + 0x9f) & 2) != 0) {
    sVar1 = -sVar1;
  }
  sVar1 = sVar1 + *(short *)(in_A0 + 0x3c);
  if (0x130 < sVar1) {
    *(byte *)(unaff_A6 + 0x9f) = *(byte *)(unaff_A6 + 0x9f) | 2;
  }
  if (sVar1 < 0x10) {
    *(byte *)(unaff_A6 + 0x9f) = *(byte *)(unaff_A6 + 0x9f) & 0xfd;
  }
  candidate_4482e();
  candidate_446d4();
  return;
}
