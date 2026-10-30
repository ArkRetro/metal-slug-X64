/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0af108.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_af108(void)

{
  undefined4 *unaff_A6;
  uint6 uVar1;

  uVar1 = candidate_40200();
  if (-1 < (int6)uVar1) {
    *(char *)((int)unaff_A6 + 0x7e) = (char)(uVar1 >> 0x20);
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x20) != 0) && ((uVar1 & 1) != 0)) {
      *unaff_A6 = candidate_b00c0;
      return;
    }
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x10) != 0) && ((uVar1 & 2) != 0)) {
      *unaff_A6 = candidate_b01ce;
    }
  }
  return;
}
