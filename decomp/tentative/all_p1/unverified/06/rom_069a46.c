/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069a46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69a46(void)

{
  short sVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined1 *)((int)unaff_A6 + 0x83) = 2;
  *unaff_A6 = 0x69a52;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = -*(int *)((int)unaff_A6 + 0x6e);
  uVar3 = 0;
  func_0x000143e4();
  candidate_69d18();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_69a90;
    uVar3 = 0;
  }
  candidate_69dd0();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_69bf4;
  }
  bVar2 = *(char *)((int)unaff_A6 + 0x7e) + 1;
  if (0x23 < bVar2) {
    bVar2 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x7e) = bVar2;
  uVar3 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x1f);
    uVar3 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_69b74;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = 0x69c9a;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xfd;
  return;
}
