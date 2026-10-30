/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069b06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69b06(void)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  byte bVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar3;
  *unaff_A6 = 0x69b1c;
  candidate_1436a();
  candidate_69d4a();
  sVar1 = *(short *)((int)unaff_A6 + 0x76);
  sVar2 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x76) = sVar2;
  if (-1 >= sVar2) {
    *unaff_A6 = candidate_69b46;
  }
  bVar5 = -1 < sVar2 && sVar1 == 0;
  candidate_69dd0();
  if (bVar5) {
    *unaff_A6 = candidate_69bf4;
  }
  bVar4 = *(char *)((int)unaff_A6 + 0x7e) + 1;
  if (0x23 < bVar4) {
    bVar4 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x7e) = bVar4;
  uVar6 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x1f);
    uVar6 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar6 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_69b74;
    uVar6 = 0;
  }
  candidate_1945c();
  if ((bool)uVar6) {
    *unaff_A6 = 0x69c9a;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xfd;
  return;
}
