/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d1da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d1da(void)

{
  uint uVar1;
  uint uVar2;
  undefined2 uVar3;
  short sVar4;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  candidate_4c1c();
  sVar4 = *(short *)((int)unaff_A6 + 0x86) << 3;
  unaff_A6[0xc] = *(undefined4 *)(sVar4 + 0x4d1b2);
  unaff_A6[0xd] = 0;
  uVar2 = *(uint *)(sVar4 + 0x4d1b6);
  uVar1 = unaff_A6[10];
  bVar5 = CARRY4(uVar2,uVar1);
  unaff_A6[10] = uVar2 + uVar1;
  candidate_4dd86();
  if (bVar5) {
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  }
  else {
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  }
  unaff_A6[0x14] = 0xe0f3e;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  if (*(short *)((int)unaff_A6 + 0x86) == 0) {
    candidate_120f8();
  }
  *unaff_A6 = candidate_4d284;
  return;
}
