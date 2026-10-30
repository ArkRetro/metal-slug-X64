/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0756e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_756e2(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  sVar1 = *(short *)((int)unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x1d);
  if (sVar1 < 0) {
    *(undefined2 *)(unaff_A6 + 0x1d) = 0xe000;
    *unaff_A6 = candidate_75750;
    sVar1 = 0;
  }
  *(short *)((int)unaff_A6 + 0x76) = sVar1;
  uVar2 = 0;
  candidate_767b0();
  candidate_21288();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_75216;
    uVar2 = 0;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x758e4;
    uVar2 = 0;
  }
  candidate_76844();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_758ba;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_75216;
  }
  return;
}
