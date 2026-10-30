/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f456.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f456(void)

{
  short sVar1;
  undefined1 uVar2;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x93) & 8) == 0) {
    return;
  }
  sVar1 = candidate_1b29c();
  uVar2 = 0;
  if ((0x6f < sVar1) && (uVar2 = 1, 0xaf < sVar1)) {
    uVar2 = 2;
  }
  *(undefined1 *)(unaff_A6 + 0x88) = uVar2;
  return;
}
