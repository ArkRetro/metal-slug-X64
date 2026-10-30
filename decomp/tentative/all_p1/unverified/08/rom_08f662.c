/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f662.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f662(void)

{
  byte bVar1;
  undefined4 extraout_A0;
  int unaff_A6;

  *(char *)(unaff_A6 + 0x90) = *(char *)(unaff_A6 + 0x90) + '\x01';
  bVar1 = *(byte *)(unaff_A6 + 0x90) & 3;
  *(byte *)(unaff_A6 + 0x90) = bVar1;
  if (bVar1 == 0) {
    candidate_1af9a();
    *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0;
  }
  return;
}
