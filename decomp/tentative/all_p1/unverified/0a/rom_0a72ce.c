/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a72ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a72ce(void)

{
  uint uVar1;
  short sVar2;
  byte bVar3;
  uint uVar4;
  int iVar5;
  undefined2 uVar6;
  undefined4 *unaff_A6;
  bool bVar7;

  unaff_A6[0x1d] = unaff_A6[0xd];
  func_0x000143e4();
  candidate_49e8();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    if (*(char *)((int)unaff_A6 + 0x7b) == '\0') {
      iVar5 = (int)unaff_A6[0xc] >> 3;
    }
    else {
      iVar5 = (int)unaff_A6[0xc] >> 4;
    }
    unaff_A6[0xc] = unaff_A6[0xc] - iVar5;
  }
  else {
    unaff_A6[0xd] = unaff_A6[0x1d];
    iVar5 = 0xc0000;
    uVar6 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      uVar6 = 0x80;
      iVar5 = -0xc0000;
    }
    unaff_A6[0xc] = (iVar5 - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
    *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar6;
  }
  bVar7 = false;
  if (*(char *)((int)unaff_A6 + 0x7b) != '\0') {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) goto LAB_000a737c;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
    }
  }
  iVar5 = unaff_A6[0xc];
  if (-1 < iVar5) {
    iVar5 = -iVar5;
  }
  uVar4 = (iVar5 + 0xc0000) - unaff_A6[0xd] >> 4;
  uVar1 = unaff_A6[0xd];
  bVar7 = CARRY4(uVar4,uVar1);
  unaff_A6[0xd] = uVar4 + uVar1;
LAB_000a737c:
  candidate_19504();
  if (bVar7) {
    *unaff_A6 = candidate_a73b4;
  }
  sVar2 = *(short *)((int)unaff_A6 + 0x72);
  *(short *)((int)unaff_A6 + 0x72) = sVar2 + -1;
  if (sVar2 == 0) {
    *unaff_A6 = candidate_a73b4;
  }
  candidate_427a();
  bVar3 = *(byte *)(unaff_A6 + 0x1e);
  *(byte *)(unaff_A6 + 0x1e) = bVar3 + 0x60;
  if (bVar3 < 0xa0) {
    candidate_a748e();
  }
  return;
}
