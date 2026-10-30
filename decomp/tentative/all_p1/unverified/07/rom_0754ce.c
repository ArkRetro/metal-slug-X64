/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0754ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_754ce(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  sVar1 = *(short *)(unaff_A6 + 0x21);
  uVar3 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x21) = sVar1;
  if (sVar1 == 0) {
    uVar2 = candidate_29c6();
    if ((uVar2 & 0xf) < 6) {
      *unaff_A6 = 0x759c0;
      uVar3 = false;
    }
    else {
      *unaff_A6 = 0x754f8;
      uVar3 = false;
    }
  }
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x758e4;
    uVar3 = 0;
  }
  candidate_427a();
  candidate_7685c();
  if ((bool)uVar3) {
    *unaff_A6 = 0x759c0;
    uVar3 = 0;
  }
  candidate_76844();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_758ba;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_75216;
  }
  return;
}
