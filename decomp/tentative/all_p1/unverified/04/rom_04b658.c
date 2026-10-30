/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b658.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b658(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  bVar1 = false;
  candidate_40bc();
  if (!bVar1) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
  }
  *unaff_A6 = 0x4b680;
  candidate_1abc8();
  candidate_4d9de();
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    *unaff_A6 = candidate_4b6ae;
  }
  bVar1 = false;
  candidate_4d74a();
  if (bVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = 0x4b732;
  }
  return;
}
