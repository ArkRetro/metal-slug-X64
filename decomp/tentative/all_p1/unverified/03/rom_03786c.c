/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03786c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3786c(void)

{
  undefined4 uVar1;
  int unaff_A6;

  candidate_37854();
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  uVar1 = 0x8000;
  if (*(byte *)(unaff_A6 + 0x98) != 6) {
    if (6 < *(byte *)(unaff_A6 + 0x98)) {
      uVar1 = 0xffff8000;
    }
    *(undefined4 *)(unaff_A6 + 0x34) = uVar1;
  }
  return;
}
