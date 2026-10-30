/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ab80.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ab80(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  undefined8 uVar3;

  candidate_9f88();
  candidate_9ea8();
  candidate_260c8();
  uVar3 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar3;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
  candidate_4256();
  *unaff_A6 = 0x7abe8;
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7ac76;
  }
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_1c71e;
  }
  if ((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_1c706;
  }
  *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) + 1;
  uVar2 = *(ushort *)((int)unaff_A6 + 0x92) < 0xf;
  if (0xe < (short)*(ushort *)((int)unaff_A6 + 0x92)) {
    *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
    candidate_40bc();
    uVar2 = 0;
    candidate_1acf2();
  }
  candidate_7b9a4();
  if ((bool)uVar2) {
    *unaff_A6 = 0x7aae8;
  }
  return;
}
