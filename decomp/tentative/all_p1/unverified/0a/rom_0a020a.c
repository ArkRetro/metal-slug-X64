/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a020a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a020a(void)

{
  uint uVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9ea8();
  candidate_4256();
  unaff_A6[0xd] = uRam000a0b5e;
  *(undefined4 *)((int)unaff_A6 + 0x76) = uRam000a0b62;
  iVar2 = iRam000a0b66;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iRam000a0b66;
  }
  unaff_A6[0xc] = iVar2;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *unaff_A6 = 0xa01d0;
  uVar1 = unaff_A6[0xd];
  uVar3 = CARRY4(*(uint *)((int)unaff_A6 + 0x76),uVar1);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x76) + uVar1;
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_41ac;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
