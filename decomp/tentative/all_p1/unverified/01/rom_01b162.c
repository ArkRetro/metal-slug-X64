/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01b162.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_1b162(void)

{
  int iVar1;
  short sVar2;
  short sVar3;
  int in_A0;
  int unaff_A6;

  if (in_A0 == 0) {
    return 0xa0;
  }
  sVar2 = *(short *)(in_A0 + 0x3c);
  iVar1 = *(int *)(in_A0 + 0x50);
  if (iVar1 != 0) {
    sVar3 = *(short *)(iVar1 + 2) + (*(short *)(iVar1 + 6) >> 1);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      sVar3 = -sVar3;
    }
    sVar2 = sVar3 + sVar2;
  }
  return sVar2;
}
