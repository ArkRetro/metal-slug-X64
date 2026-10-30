/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0699e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_699e0(void)

{
  short sVar1;
  undefined4 uVar2;
  byte bVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
  *unaff_A6 = 0x699ec;
  uVar2 = 0x1000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = 0xfffff000;
  }
  unaff_A6[0xc] = uVar2;
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x6e);
  uVar4 = 0;
  func_0x000143e4();
  candidate_69d18();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 2) != 0) {
    *unaff_A6 = candidate_69b06;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
    *unaff_A6 = candidate_69a90;
    uVar4 = 0;
  }
  candidate_69dd0();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_69bf4;
  }
  bVar3 = *(char *)((int)unaff_A6 + 0x7e) + 1;
  if (0x23 < bVar3) {
    bVar3 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x7e) = bVar3;
  uVar4 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x1f);
    uVar4 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_69b74;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = 0x69c9a;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xfd;
  return;
}
