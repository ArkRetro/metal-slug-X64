/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024f0e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24f0e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined8 uVar3;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_1abc8();
  candidate_260c8();
  uVar3 = candidate_3b332();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar3;
  unaff_A6[0x20] = 0xffffe000;
  *(undefined2 *)(unaff_A6 + 0x26) = 0;
  candidate_25160();
  candidate_4256();
  *unaff_A6 = 0x24f80;
  candidate_25192();
  unaff_A6[0xd] = unaff_A6[0x20] + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_2503a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_24fd6;
  }
  bVar2 = false;
  candidate_24acc();
  if (bVar2) {
    *unaff_A6 = candidate_2503a;
  }
  candidate_248d8();
  return;
}
