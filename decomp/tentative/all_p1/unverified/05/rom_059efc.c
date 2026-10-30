/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x059efc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_59efc(void)

{
  undefined4 uVar1;
  int unaff_A6;
  undefined1 in_CF;

  uVar1 = candidate_1affc();
  if ((bool)in_CF) {
    return uVar1;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0x3c) < 0xa0) {
      uVar1 = 0;
    }
    else {
      uVar1 = 0xffffffff;
    }
  }
  else if (*(short *)(unaff_A6 + 0x3c) < 0xa1) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
