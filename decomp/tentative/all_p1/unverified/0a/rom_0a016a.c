/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a016a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a016a(void)

{
  uint uVar1;
  short sVar3;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_9ea8();
  candidate_4256();
  sVar3 = (*(byte *)((int)unaff_A6 + 0x8e) & 3) * 0xc;
  unaff_A6[0xd] = *(undefined4 *)(sVar3 + 0xa0b2e);
  *(undefined4 *)((int)unaff_A6 + 0x76) = *(undefined4 *)(sVar3 + 0xa0b32);
  iVar2 = *(int *)(sVar3 + 0xa0b36);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  unaff_A6[0xc] = iVar2;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *unaff_A6 = 0xa01d0;
  uVar1 = unaff_A6[0xd];
  uVar4 = CARRY4(*(uint *)((int)unaff_A6 + 0x76),uVar1);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x76) + uVar1;
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_41ac;
    uVar4 = 0;
  }
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
