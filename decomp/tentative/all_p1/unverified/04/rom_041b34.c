/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x041b34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_41b34(void)

{
  int iVar1;
  byte *pbVar2;
  undefined2 uVar3;
  int unaff_A6;

  candidate_120f8();
  candidate_c8ce6();
  *(undefined4 *)(unaff_A6 + 0x50) = 0x44e00;
  *(undefined1 *)(unaff_A6 + 0x5a) = 0;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
  pbVar2 = (byte *)(*(int *)(unaff_A6 + 0x72) + 0x86);
  *pbVar2 = *pbVar2 & 0xfd;
  uVar3 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x4e) = uVar3;
  uVar3 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x9a) = uVar3;
  candidate_4520();
  candidate_42284();
  iVar1 = *(int *)(unaff_A6 + 0x72);
  *(undefined1 *)(iVar1 + 0x72) = 4;
  *(undefined1 *)(iVar1 + 0x8e) = 0;
  candidate_1ab98();
  *(undefined4 *)(unaff_A6 + 0x86) = uRam0010d7b6;
  *(undefined4 *)(unaff_A6 + 0x8a) = uRam0010d7ba;
  *(undefined4 *)(unaff_A6 + 0x8e) = 0;
  *(undefined4 *)(unaff_A6 + 0x92) = 0;
  return;
}
