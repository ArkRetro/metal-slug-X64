/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8cd8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8cd8(void)

{
  int iVar1;
  byte *pbVar2;
  byte bVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0xaa) = 0;
  *(undefined2 *)(unaff_A6 + 0x2a) = 0x200;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  candidate_4256();
  unaff_A6[0x28] = *(undefined4 *)((short)((*(ushort *)(unaff_A6 + 0x27) & 3) << 2) + 0xe9b30);
  *(undefined2 *)(unaff_A6 + 0x29) = 0;
  *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  *unaff_A6 = 0xd8d36;
  bVar4 = false;
  candidate_21288();
  if (bVar4) {
    *unaff_A6 = candidate_d8d88;
  }
  candidate_d95d2();
  candidate_d961a();
  candidate_427a();
  iVar1 = *(int *)(unaff_A6[3] + 0xc);
  pbVar2 = (byte *)(iVar1 + 0x9a);
  bVar3 = *pbVar2;
  *pbVar2 = bVar3 + 1;
  bVar4 = (*(byte *)(iVar1 + 0x5f) & 0x80) == 0;
  if (!bVar4) {
    *unaff_A6 = candidate_d888c;
  }
  bVar4 = bVar4 && 0xfe < bVar3;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
