/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0808.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_c0808(void)

{
  char *pcVar1;
  int iVar2;
  uint in_D0;
  short sVar4;
  uint uVar3;
  int iVar5;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A1;
  int unaff_A5;
  int unaff_A6;

  sVar4 = (short)(in_D0 & 0xffff0001);
  if ((DAT_0010f001 & 4) == 0) {
    if (*(char *)(unaff_A5 + 0x4720 + (int)sVar4) == '\0') {
      return in_D0 & 0xffff0001;
    }
    pcVar1 = (char *)(unaff_A5 + 0x4720 + (int)sVar4);
    *pcVar1 = *pcVar1 + -1;
  }
  if (*(char *)(unaff_A5 + 0x4722 + (int)sVar4) != '\0') {
    candidate_411c();
    *(undefined1 *)(extraout_A0 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
    *(short *)(extraout_A0 + 0x6e) = sVar4;
    candidate_14a02();
    iVar2 = *(int *)(unaff_A6 + 0x38);
    uVar3 = *(uint *)(unaff_A6 + 0x30);
    iVar5 = *(int *)(unaff_A6 + 0x34);
    if (iVar2 != 0) {
      uVar3 = (*(int *)(iVar2 + 0x14) + uVar3) - *(int *)(iVar2 + 0x1c);
      iVar5 = (*(int *)(iVar2 + 0x18) + iVar5) - *(int *)(iVar2 + 0x20);
    }
    if (extraout_A1 != 0) {
      uVar3 = *(int *)(extraout_A1 + 0x1c) + (uVar3 - *(int *)(extraout_A1 + 0x14));
      iVar5 = *(int *)(extraout_A1 + 0x20) + (iVar5 - *(int *)(extraout_A1 + 0x18));
    }
    *(uint *)(extraout_A0_00 + 0x30) = uVar3;
    *(int *)(extraout_A0_00 + 0x34) = iVar5;
    return uVar3;
  }
  return (int)sVar4;
}
