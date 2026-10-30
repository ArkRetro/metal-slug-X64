/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca776.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ca776(void)

{
  undefined2 uVar1;
  int extraout_A0;
  undefined4 unaff_A6;

  candidate_ca562();
  uVar1 = candidate_ca502();
  *(undefined2 *)(extraout_A0 + 0x20) = uVar1;
  DAT_0010fdc4 = 4;
  _DAT_0010fdce = 0x250;
  DAT_0010fdd0 = 1;
  func_0x00c00468();
  DAT_0010fdc4 = 3;
  _DAT_0010fdce = 0x250;
  DAT_0010fdd0 = 1;
  _DAT_0010fdc8 = 0x10f068;
  _DAT_0010fdcc = 0x22;
  func_0x00c00468();
  DAT_0010fdc2 = 0;
  func_0x00c0046e(unaff_A6);
  if (DAT_0010fdc6 == '\0') {
    return;
  }
  return;
}
