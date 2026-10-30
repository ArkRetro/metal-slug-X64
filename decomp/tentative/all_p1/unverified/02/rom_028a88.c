/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028a88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_28a88(void)

{
  undefined4 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  uVar1 = 0x10000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar1 = 0xffff0000;
  }
  unaff_A6[0xc] = uVar1;
  unaff_A6[0xd] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_4256();
  *unaff_A6 = 0x28afa;
  func_0x000143e4();
  unaff_A6[0xc] = ((int)unaff_A6[0xc] >> 5) + unaff_A6[0xc];
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x6e) + unaff_A6[0xd];
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)
        ((int)unaff_A6 + (int)(short)(ushort)*(byte *)((short)(_DAT_0010000e & 3) + 0x2910e));
  candidate_427a();
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_28b8c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_28b8c;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_28b8c;
  }
  bVar3 = false;
  candidate_19504();
  if (bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
