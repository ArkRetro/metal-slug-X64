/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042178.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42178(void)

{
  byte *pbVar1;
  undefined2 uVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  candidate_c8ce6();
  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x70) = 2;
  *(undefined4 *)(unaff_A6 + 0x50) = 0x44de4;
  *(undefined1 *)(unaff_A6 + 0x5a) = 0;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
  pbVar1 = (byte *)(*(int *)(unaff_A6 + 0x72) + 0x86);
  *pbVar1 = *pbVar1 & 0xfd;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x4e) = uVar2;
  candidate_423d8();
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)(unaff_A6 + 0x72);
    *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x80);
  }
  return;
}
