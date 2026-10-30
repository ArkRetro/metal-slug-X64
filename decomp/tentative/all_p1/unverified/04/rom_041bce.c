/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x041bce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_41bce(void)

{
  int iVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x6e) = 4;
  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x6f) = 2;
  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x70) = 0;
  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x71) = 0;
  candidate_446cc();
  *(undefined4 *)(unaff_A6 + 0x8e) = 0;
  *(undefined4 *)(unaff_A6 + 0x92) = 0;
  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  *(undefined4 *)(unaff_A6 + 0x50) = 0;
  *(undefined1 *)(unaff_A6 + 0x5a) = 0;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
  candidate_44666();
  iVar1 = *(int *)(unaff_A6 + 0x72);
  *(undefined1 *)(iVar1 + 0x72) = 1;
  *(undefined1 *)(iVar1 + 0x8e) = 0;
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)(unaff_A6 + 0x72);
    *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x80);
  }
  *(undefined2 *)(extraout_A0 + 0x86) = 0x3c;
  candidate_ae220();
  candidate_446a0();
  candidate_1228c();
  return;
}
