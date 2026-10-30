/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x099a8a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_99a8a(void)

{
  short sVar2;
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  sVar2 = candidate_260c8();
  iVar1 = sVar2 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = sVar2 * -0x100;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x1e;
  candidate_4256();
  *unaff_A6 = 0x99ac6;
  uVar3 = 0;
  candidate_99bb2();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
    uVar3 = 0;
    if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
      *unaff_A6 = candidate_99b22;
      uVar3 = 0;
    }
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_99b22;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_99b22;
  }
  return;
}
