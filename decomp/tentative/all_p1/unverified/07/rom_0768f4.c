/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0768f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_768f4(void)

{
  ushort uVar1;
  int unaff_A6;

  uVar1 = _DAT_0010000e & 0xff02;
  if ((_DAT_0010000e & 2) == 0) {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1a);
  }
  return uVar1;
}
