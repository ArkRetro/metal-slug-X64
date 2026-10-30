/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0777ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_777ce(void)

{
  char cVar1;
  undefined2 extraout_D0u;
  undefined2 uVar2;
  short sVar3;
  int extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

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
    candidate_1da40();
    *(undefined2 *)(extraout_A0_00 + 0x66) = 0xe0ed;
    *(undefined2 *)(extraout_A0_00 + 0x1e) = 0xe0ed;
  }
  *unaff_A6 = 0x778ae;
  uVar5 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar5) {
    *unaff_A6 = 0x7783a;
  }
  cVar1 = *(char *)(unaff_A6[3] + 0x9b);
  if (cVar1 == '\x01') {
    *unaff_A6 = 0x77ad4;
  }
  bVar4 = (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0;
  if (!bVar4) {
    *unaff_A6 = candidate_77aa4;
  }
  bVar4 = bVar4 && cVar1 == '\0';
  candidate_78d7a();
  if (bVar4) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
