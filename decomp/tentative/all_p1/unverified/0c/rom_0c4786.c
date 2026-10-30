/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c4786.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c4786(void)

{
  char *pcVar1;
  int iVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A1;
  int unaff_A5;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x70);
  if ((DAT_0010f001 & 4) == 0) {
    if (*(char *)(unaff_A5 + 0x4720 + (int)sVar3) == '\0') {
      return;
    }
    pcVar1 = (char *)(unaff_A5 + 0x4720 + (int)sVar3);
    *pcVar1 = *pcVar1 + -1;
  }
  if (*(char *)(unaff_A5 + 0x4722 + (int)sVar3) == '\0') {
    return;
  }
  candidate_411c();
  *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x70);
  *(undefined1 *)(extraout_A0 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
  candidate_1ac94();
  candidate_14a02();
  iVar2 = *(int *)(unaff_A6 + 0x38);
  iVar4 = *(int *)(unaff_A6 + 0x30);
  iVar5 = *(int *)(unaff_A6 + 0x34);
  if (iVar2 != 0) {
    iVar4 = (*(int *)(iVar2 + 0x14) + iVar4) - *(int *)(iVar2 + 0x1c);
    iVar5 = (*(int *)(iVar2 + 0x18) + iVar5) - *(int *)(iVar2 + 0x20);
  }
  if (extraout_A1 != 0) {
    iVar4 = *(int *)(extraout_A1 + 0x1c) + (iVar4 - *(int *)(extraout_A1 + 0x14));
    iVar5 = *(int *)(extraout_A1 + 0x20) + (iVar5 - *(int *)(extraout_A1 + 0x18));
  }
  *(int *)(extraout_A0_00 + 0x30) = iVar4;
  *(int *)(extraout_A0_00 + 0x34) = iVar5;
  return;
}
