/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ae4a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1ae4a(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined1 uVar1;

  *(undefined1 *)(unaff_A5 + 0x433c) = 0;
  uVar1 = false;
  func_0x000040ac();
  if (((bool)uVar1) && ((*(byte *)(extraout_A0 + 0x5f) & 0x80) == 0)) {
    uVar1 = 0;
    *(byte *)(unaff_A5 + 0x433c) = *(byte *)(unaff_A5 + 0x433c) | 1;
  }
  func_0x000040ac();
  if (((bool)uVar1) && ((*(byte *)(extraout_A0_00 + 0x5f) & 0x80) == 0)) {
    *(byte *)(unaff_A5 + 0x433c) = *(byte *)(unaff_A5 + 0x433c) | 2;
  }
  return;
}
