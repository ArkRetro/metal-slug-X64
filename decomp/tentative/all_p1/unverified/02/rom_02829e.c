/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02829e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2829e(void)

{
  uint uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(byte *)((int)unaff_A6 + 0x7d) = *(byte *)((int)unaff_A6 + 0x7d) ^ 0x80;
  sVar2 = *(short *)((int)unaff_A6 + 0x62);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    sVar2 = -sVar2;
  }
  if (sVar2 < 1) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  *(int *)((int)unaff_A6 + 0x72) = -(*(int *)((int)unaff_A6 + 0x62) >> 5);
  candidate_12192();
  *unaff_A6 = 0x28300;
  candidate_146a8();
  if (*(int *)((int)unaff_A6 + 0x72) == 0) {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar3 = false;
  }
  else if (*(int *)((int)unaff_A6 + 0x62) == 0) {
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
    uVar3 = false;
  }
  else {
    uVar1 = *(uint *)((int)unaff_A6 + 0x62);
    uVar3 = CARRY4(*(uint *)((int)unaff_A6 + 0x72),uVar1);
    *(uint *)((int)unaff_A6 + 0x62) = *(uint *)((int)unaff_A6 + 0x72) + uVar1;
  }
  candidate_28d04();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = 0x28118;
    uVar3 = 0;
  }
  candidate_28da8();
  candidate_4c24();
  if ((bool)uVar3) {
    candidate_1b206();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_2849a;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_284f2;
    bVar4 = false;
    candidate_1b206();
    if (bVar4) {
      *unaff_A6 = candidate_2858c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
