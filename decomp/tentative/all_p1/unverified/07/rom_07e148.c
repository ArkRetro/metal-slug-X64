/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07e148.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7e148(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined8 uVar3;

  candidate_260c8();
  uVar3 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar3;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x7e1a0;
  func_0x000143e4();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_7e136;
  }
  if (0x1c8 < (short)*(ushort *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_7e136;
  }
  bVar2 = *(ushort *)(unaff_A6 + 0x10) < 0x1c8;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
