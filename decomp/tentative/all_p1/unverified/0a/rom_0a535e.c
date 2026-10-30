/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a535e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined1 candidate_a535e(void)

{
  short sVar1;
  undefined1 uVar2;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1af9a();
  if ((bool)in_CF) {
    return *(undefined1 *)(unaff_A6 + 0x89);
  }
  sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
  uVar2 = 0;
  if (((-0x61 < sVar1) && (uVar2 = 1, -1 < sVar1)) && (uVar2 = 2, 0x5f < sVar1)) {
    uVar2 = 3;
  }
  return uVar2;
}
