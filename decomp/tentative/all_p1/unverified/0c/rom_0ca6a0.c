/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca6a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ca6a0(void)

{
  short sVar1;
  int extraout_A0;

  DAT_0010fdc4 = 2;
  _DAT_0010fdce = 0x250;
  DAT_0010fdd0 = 1;
  _DAT_0010fdc8 = 0x10f068;
  _DAT_0010fdcc = 0x22;
  func_0x00c00468();
  if (DAT_0010fdc6 == '\0') {
    sVar1 = candidate_ca502();
    if (sVar1 == *(short *)(extraout_A0 + 0x20)) {
      return;
    }
  }
  candidate_ca598();
  return;
}
