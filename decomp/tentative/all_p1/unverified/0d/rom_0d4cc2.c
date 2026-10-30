/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d4cc2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d4cc2(void)

{
  int iVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  candidate_1ab98();
  candidate_1b1dc();
  candidate_260c8();
  iVar1 = candidate_d5090();
  unaff_A6[0xc] = (iVar1 << 0x10) >> 8;
  unaff_A6[0xd] = 0;
  *unaff_A6 = 0xd4d24;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_d4fca;
  }
  candidate_427a();
  sVar2 = (short)*(int *)((int)unaff_A6 + 0x9a);
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    sVar2 = candidate_49e8();
  }
  if (sVar2 != 0) {
    *unaff_A6 = candidate_d4fca;
  }
  uVar3 = false;
  candidate_409a();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_d4fca;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
