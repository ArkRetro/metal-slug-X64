/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0b32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0b32(void)

{
  short sVar1;
  byte bVar2;
  char extraout_D1b;
  char extraout_D1b_00;
  int extraout_A0;
  int unaff_A5;
  bool bVar3;

  bVar3 = false;
  func_0x000040ac();
  if (!bVar3) {
    return;
  }
  candidate_aa898();
  if (extraout_D1b_00 == '\0') {
    sVar1 = *(short *)(extraout_A0 + 0x70);
    if (0x62 < *(byte *)(unaff_A5 + 0x4720 + (int)sVar1)) {
      return;
    }
    bVar2 = *(char *)(unaff_A5 + 0x4720 + (int)sVar1) + extraout_D1b;
    if (99 < bVar2) {
      bVar2 = 99;
    }
    *(byte *)(unaff_A5 + 0x4720 + (int)sVar1) = bVar2;
  }
  candidate_120f8();
  return;
}
