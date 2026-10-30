/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07668a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7668a(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int extraout_A0_00;
  int iVar3;
  int unaff_A6;

  candidate_1b04a();
  *(int *)(unaff_A6 + 0x80) = extraout_A0;
  sVar1 = *(short *)(unaff_A6 + 0x74);
  iVar3 = extraout_A0;
  if ((char)sVar1 == '\x03') {
    sVar1 = candidate_29f2();
    iVar3 = extraout_A0_00;
  }
  if (*(short *)(iVar3 + 0x3c) < 0xd1) {
    if (*(short *)(iVar3 + 0x3c) < 0x71) goto LAB_000766d2;
    uVar2 = candidate_29c6();
    if ((uVar2 & 1) == 0) goto LAB_000766d2;
  }
  sVar1 = sVar1 + 3;
LAB_000766d2:
  if (5 < sVar1) {
    __m68k_trap(0xf);
  }
  return;
}
