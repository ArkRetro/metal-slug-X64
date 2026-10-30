/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07bf38.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7bf38(void)

{
  short sVar2;
  undefined4 uVar1;
  byte bVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  sVar2 = (*(ushort *)(unaff_A6 + 0x1f) & 3) * 0xc;
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(sVar2 + 0xe45b4);
  unaff_A6[0xd] = *(undefined4 *)(sVar2 + 0xe45b8);
  uVar1 = candidate_7c150();
  unaff_A6[0xc] = uVar1;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *unaff_A6 = 0x7bf98;
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7bf1e;
  }
  bVar3 = candidate_2eb4();
  if (bVar3 < 0x60) {
    bVar3 = 0x60;
  }
  if (0xbf < bVar3) {
    bVar3 = 0xc0;
  }
  *(byte *)((int)unaff_A6 + 0x81) = (byte)(bVar3 + 0xa0) >> 2;
  candidate_427a();
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_7bf1e;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_7be86;
  }
  return;
}
