/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0196e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_196e8(void)

{
  short in_D0w;
  uint uVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  int unaff_A6;
  bool bVar4;

  *(undefined1 *)(unaff_A5 + 0x4323) = 0;
  candidate_1ab98();
  uVar1 = (uint)(ushort)(short)*(char *)(*(int *)(unaff_A5 + 0x431a) + 7);
  iVar2 = uVar1 * 0x10000;
  if (in_D0w != 0) {
    iVar2 = uVar1 * -0x10000;
  }
  *(int *)(unaff_A6 + 0x30) = iVar2;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  func_0x000143e4();
  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  sVar3 = candidate_1abc8();
  if (*(char *)(*(int *)(unaff_A5 + 0x431a) + 8) == '\x04') {
    sVar3 = candidate_c0d2c();
    bVar4 = CARRY2(*(ushort *)(unaff_A6 + 0x40),-sVar3);
    sVar3 = *(ushort *)(unaff_A6 + 0x40) + -sVar3;
    if ((-1 < sVar3) && (sVar3 = candidate_14742(), bVar4)) {
      return sVar3;
    }
  }
  return sVar3;
}
