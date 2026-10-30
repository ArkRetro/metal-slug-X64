/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce3ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ce3ba(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar2 = candidate_29c6();
  unaff_A6[0xc] = (int)((uint)(ushort)-((uVar2 & 0x700) - 0x280) << 0x10) >> 8;
  uVar2 = candidate_29c6();
  unaff_A6[0xd] = (int)((uint)(ushort)((uVar2 & 0x700) + 0x400) << 0x10) >> 8;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffc0;
  *unaff_A6 = 0xce3f8;
  unaff_A6[0xd] = ((int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_ce1da;
  }
  if ((_DAT_0010000e & 2) == 0) {
    uVar3 = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    uVar3 = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  *(undefined1 *)(unaff_A6 + 5) = uVar3;
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_ce1da;
  }
  uVar3 = false;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_ce1da;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
