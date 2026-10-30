/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069a90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69a90(void)

{
  short sVar1;
  int iVar2;
  byte bVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
  *unaff_A6 = 0x69a9c;
  iVar2 = *(int *)((int)unaff_A6 + 0x6e);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  unaff_A6[0xc] = iVar2;
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  unaff_A6[0xd] = 0;
  uVar4 = 0;
  candidate_1456a();
  candidate_69d4a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_69a46;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_699e0;
    uVar4 = 0;
  }
  candidate_69d94();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_69b06;
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
