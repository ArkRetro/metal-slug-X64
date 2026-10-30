/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028bbc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28bbc(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_4256();
  candidate_9ea8();
  unaff_A6[0xd] = 0x99999;
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xffff0a3e;
  unaff_A6[0xc] = 0xcccc;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  *unaff_A6 = 0x28c04;
  func_0x000143e4();
  uVar1 = unaff_A6[0xd];
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x6e) + uVar1;
  bVar2 = (*(byte *)((int)unaff_A6 + 0x15) & 0x80) == 0;
  if (!bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  uVar3 = bVar2 && CARRY4(*(uint *)((int)unaff_A6 + 0x6e),uVar1);
  candidate_427a();
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
