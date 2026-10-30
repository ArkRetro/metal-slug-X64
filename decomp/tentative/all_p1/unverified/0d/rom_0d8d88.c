/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8d88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8d88(void)

{
  int iVar1;
  byte *pbVar2;
  byte bVar3;
  undefined2 uVar4;
  short sVar5;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar6;

  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x80;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  uVar4 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar4;
  sVar5 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x2a) = *(undefined2 *)((short)(sVar5 << 2) + 0x2cc2b0);
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)((short)(sVar5 << 2) + 0x2cc2b2);
  candidate_1b04a();
  *(undefined4 *)((int)unaff_A6 + 0x9e) = extraout_A0;
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  candidate_4256();
  *unaff_A6 = 0xd8e08;
  *(short *)(unaff_A6 + 0x23) = *(short *)(unaff_A6 + 0x23) + 1;
  if ((*(short *)(unaff_A6 + 0x23) < 0xb5) &&
     (*(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1,
     *(short *)((int)unaff_A6 + 0x76) < 1)) {
    *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x1e);
    uVar4 = candidate_1b12e();
    *(undefined2 *)(unaff_A6 + 0x1f) = uVar4;
    candidate_d923a();
    candidate_d939a();
  }
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d888c;
  }
  *(ushort *)((int)unaff_A6 + 0xaa) = *(ushort *)((int)unaff_A6 + 0x6e) >> 3;
  candidate_427a();
  sVar5 = candidate_49e8();
  if (sVar5 != 0) {
    *unaff_A6 = candidate_d888c;
  }
  bVar6 = false;
  candidate_4c24();
  if (bVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d888c;
  }
  iVar1 = *(int *)(unaff_A6[3] + 0xc);
  pbVar2 = (byte *)(iVar1 + 0x9a);
  bVar3 = *pbVar2;
  *pbVar2 = bVar3 + 1;
  bVar6 = (*(byte *)(iVar1 + 0x5f) & 0x80) == 0;
  if (!bVar6) {
    *unaff_A6 = candidate_d888c;
  }
  bVar6 = bVar6 && 0xfe < bVar3;
  candidate_19504();
  if (bVar6) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
