/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07cd3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_7cd3c(void)

{
  undefined2 uVar1;
  short sVar2;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  uVar1 = candidate_1af9a();
  if (!(bool)in_CF) {
    sVar2 = *(short *)(unaff_A6 + 0x28) - *(short *)(extraout_A0 + 0x28);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      sVar2 = -sVar2;
    }
    uVar1 = 0;
    if (((sVar2 < 0x61) && (uVar1 = 1, sVar2 < 1)) && (uVar1 = 2, sVar2 < -0x5f)) {
      uVar1 = 3;
    }
    return uVar1;
  }
  return uVar1;
}
