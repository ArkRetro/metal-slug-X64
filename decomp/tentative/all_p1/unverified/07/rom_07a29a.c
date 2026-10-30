/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a29a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a29a(void)

{
  int iVar1;
  short sVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x7f0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x7f0;
  candidate_1b1dc();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 3;
  candidate_40bc();
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0;
  *unaff_A6 = 0x7a31a;
  candidate_1abc8();
  candidate_427a();
  iVar1 = unaff_A6[3];
  *(undefined2 *)((int)unaff_A6 + 0x82) = *(undefined2 *)(iVar1 + 0x82);
  if (*(short *)(iVar1 + 0x82) == 3) {
    *unaff_A6 = candidate_7a360;
  }
  sVar2 = *(short *)(iVar1 + 0x82);
  if (sVar2 == 1) {
    *(undefined2 *)(iVar1 + 0x82) = 0;
  }
  bVar4 = sVar2 == 0;
  candidate_7b996();
  if (bVar4) {
    *unaff_A6 = candidate_7aae2;
  }
  return;
}
