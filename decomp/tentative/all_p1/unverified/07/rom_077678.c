/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077678.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77678(void)

{
  char cVar1;
  undefined2 extraout_D0u;
  undefined2 uVar2;
  short sVar3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int iVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_1ad6e();
  uVar2 = (undefined2)(CONCAT22(extraout_D0u,*(undefined2 *)(unaff_A6[3] + 0x4e)) / 3);
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x1e) = uVar2;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0xffff;
  uVar2 = candidate_78d10();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  sVar3 = candidate_78d10();
  if (sVar3 != *(short *)((int)unaff_A6 + 0x86)) {
    *(short *)((int)unaff_A6 + 0x86) = sVar3;
    candidate_4256();
    candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x66) = 0xe0ed;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0xe0ed;
    candidate_1acf2();
    bVar5 = false;
    candidate_40bc();
    iVar4 = extraout_A0_00;
    if (bVar5) {
      candidate_1b220();
      iVar4 = extraout_A0_01;
    }
    *(undefined2 *)(iVar4 + 0x66) = 0xe0ed;
    *(undefined2 *)(iVar4 + 0x1e) = 0xe0ed;
    func_0x0001da76();
  }
  *unaff_A6 = 0x7777e;
  uVar6 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar6) {
    *unaff_A6 = 0x776e4;
  }
  cVar1 = *(char *)(unaff_A6[3] + 0x9b);
  if (cVar1 == '\x01') {
    *unaff_A6 = 0x77a84;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0;
  if (!bVar5) {
    *unaff_A6 = candidate_77a54;
  }
  bVar5 = bVar5 && cVar1 == '\0';
  candidate_78d7a();
  if (bVar5) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
