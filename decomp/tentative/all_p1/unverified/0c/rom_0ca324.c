/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca324.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ca324(void)

{
  undefined2 uVar1;
  short in_D0w;
  undefined2 in_D1w;
  int unaff_A5;

  uVar1 = in_D1w;
  if (in_D0w == 0) {
    _DAT_0010ea00 = in_D1w;
    uVar1 = _DAT_0010ea02;
  }
  _DAT_0010ea02 = uVar1;
  if ((char)in_D0w != '\0') {
    *(undefined4 *)(unaff_A5 + 0x69fc) = 0xca386;
    return;
  }
  *(undefined4 *)(unaff_A5 + 0x69f8) = 0xca386;
  return;
}
