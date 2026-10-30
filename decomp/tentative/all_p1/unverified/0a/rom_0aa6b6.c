/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa6b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_aa6b6(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort in_D0w;
  ushort uVar3;
  short sVar4;
  short sVar5;
  short *extraout_A0;
  code *in_A1;
  int iVar6;
  short *psVar7;
  int unaff_A5;
  int unaff_A6;

  if ((in_D0w & 1) == 0) {
    iVar6 = unaff_A5 + -0x6b3c;
    psVar7 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    iVar6 = unaff_A5 + -0x6a8c;
    psVar7 = (short *)(unaff_A5 + 0x462c);
  }
  uVar3 = (*in_A1)();
  if (((-1 < (char)uVar3) && (*psVar7 == 0)) && (*(char *)(psVar7 + 0xe) == '\0')) {
    if ((char)uVar3 != '\0') {
      return uVar3;
    }
    sVar4 = *(short *)(iVar6 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    sVar5 = *(short *)(iVar6 + 0x44) - *(short *)(unaff_A6 + 0x44);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      sVar4 = -sVar4;
      sVar5 = -sVar5;
    }
    uVar1 = sVar4 - *extraout_A0;
    uVar3 = extraout_A0[2];
    if ((uVar1 < uVar3) ||
       (uVar3 = (sVar5 - *extraout_A0) - uVar3 ^ uVar1 - uVar3, (short)uVar3 < 0)) {
      uVar2 = (*(short *)(iVar6 + 0x40) - *(short *)(unaff_A6 + 0x40)) - extraout_A0[1];
      uVar1 = extraout_A0[3];
      uVar3 = uVar2 - uVar1;
      if ((uVar2 < uVar1) ||
         (uVar3 = ((*(short *)(iVar6 + 0x48) - *(short *)(unaff_A6 + 0x48)) - extraout_A0[1]) -
                  uVar1 ^ uVar3, (short)uVar3 < 0)) {
        return uVar3;
      }
    }
  }
  return uVar3;
}
