/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x081fd6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_81fd6(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = candidate_29f2();
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    sVar1 = sVar1 + 0xc0;
  }
  else {
    sVar1 = sVar1 + 0x20;
  }
  return sVar1;
}
