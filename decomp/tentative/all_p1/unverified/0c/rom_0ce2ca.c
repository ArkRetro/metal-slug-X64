/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce2ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ce2ca(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  unaff_A6[0xc] = 0xfffe0000;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x1e;
  candidate_9ea8();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffc0;
  *unaff_A6 = 0xce328;
  unaff_A6[0xd] = ((int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_ce3ba;
  }
  if ((_DAT_0010000e & 2) == 0) {
    uVar2 = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    uVar2 = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  *(undefined1 *)(unaff_A6 + 5) = uVar2;
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_ce1da;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_ce1da;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
