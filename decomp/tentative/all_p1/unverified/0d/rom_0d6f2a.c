/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6f2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6f2a(void)

{
  byte bVar1;
  short sVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  if (*(short *)(unaff_A6 + 0xf) <= *(short *)(unaff_A6 + 0x1d)) {
    *(undefined2 *)(unaff_A6 + 0x22) = 0xffe0;
    *unaff_A6 = 0xd6f42;
  }
  unaff_A6[0xd] = ((int)((uint)*(ushort *)(unaff_A6 + 0x22) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d6c66;
  }
  uVar3 = candidate_2eb4();
  *(ushort *)((int)unaff_A6 + 0x82) = uVar3 >> 3 & 0x1f;
  candidate_427a();
  sVar2 = (short)*(int *)((int)unaff_A6 + 0x96);
  if (*(int *)((int)unaff_A6 + 0x96) != 0) {
    sVar2 = candidate_49e8();
  }
  if (sVar2 != 0) {
    *unaff_A6 = candidate_d6a72;
  }
  bVar4 = false;
  candidate_4c24();
  if (bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0x7a);
  if (bVar1 == 5) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar4 = bVar1 < 5;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
