/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x041c78.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_41c78(void)

{
  byte *pbVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  candidate_c8cd6();
  pbVar1 = (byte *)(*(int *)(unaff_A6 + 0x72) + 0x86);
  *pbVar1 = *pbVar1 | 1;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 4;
  candidate_1ad52();
  candidate_446d4();
  *(undefined1 *)(unaff_A6 + 0xa3) = 1;
  *(undefined4 *)(unaff_A6 + 0x8e) = 0;
  *(undefined4 *)(unaff_A6 + 0x92) = 0;
  candidate_4491c();
  candidate_41ee8();
  candidate_41e02();
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)(unaff_A6 + 0x72);
    *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x80);
  }
  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x89) = 0;
  return;
}
