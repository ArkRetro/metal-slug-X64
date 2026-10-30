/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f348.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f348(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 uVar2;

  if (((*(short *)(unaff_A6 + 0x3c) < 0x10) || (0x130 < *(short *)(unaff_A6 + 0x3c))) ||
     ((*(byte *)(unaff_A6 + 0x98) & 0x80) != 0)) {
    return;
  }
  uVar2 = false;
  if ((0 < *(short *)(unaff_A6 + 0x88)) || (candidate_9f2bc(), (bool)uVar2)) {
    return;
  }
  candidate_9f07a();
  candidate_1b08a();
  if ((bool)uVar2) {
    return;
  }
  sVar1 = candidate_9f094();
  if (*(short *)(unaff_A6 + 0x82) < sVar1) {
    return;
  }
  return;
}
