/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e30a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e30a(void)

{
  int unaff_A6;
  undefined1 in_CF;
  undefined1 uVar1;

  candidate_5e2e4();
  if (!(bool)in_CF) {
    return;
  }
  uVar1 = *(byte *)(unaff_A6 + 0x8d) < 5;
  if ((char)*(byte *)(unaff_A6 + 0x8d) < '\x06') {
    if (0 < *(short *)(unaff_A6 + 0x7c)) {
      return;
    }
    *(undefined2 *)(unaff_A6 + 0x7c) = 0;
    uVar1 = false;
    candidate_5e4cc();
    if (!(bool)uVar1) {
      return;
    }
  }
  if ((((*(byte *)(unaff_A6 + 0x93) & 0x10) != 0) && (candidate_5e784(), (bool)uVar1)) &&
     (candidate_5e482(), !(bool)uVar1)) {
    return;
  }
  return;
}
