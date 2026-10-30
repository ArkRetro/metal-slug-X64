/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x040200.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_40200(void)

{
  int iVar1;
  int iVar2;
  int unaff_D5;
  int unaff_D6;
  int iVar3;
  int extraout_A1;
  int unaff_A5;
  int unaff_A6;

  iVar2 = candidate_4015c();
  if (unaff_D5 < 0) {
    return 0xffffffff;
  }
  iVar3 = *(int *)(unaff_A6 + 0x30);
  iVar1 = *(int *)(unaff_A6 + 0x38);
  if (iVar1 != 0) {
    iVar3 = (*(int *)(iVar1 + 0x14) + iVar3) - *(int *)(iVar1 + 0x1c);
  }
  if (-1 < ((iVar2 + *(int *)(unaff_A5 + 0x6984) + (iVar3 - *(int *)(unaff_A5 + 0x697c))) - unaff_D6
           ^ iVar2 - unaff_D6)) {
    return 0xffffffff;
  }
  return (uint)*(byte *)(extraout_A1 + 9);
}
