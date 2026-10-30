/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f058.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f058(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  short *psVar5;
  undefined4 *unaff_A6;
  bool bVar6;

  psVar5 = (short *)((int)(short)((*(byte *)((int)unaff_A6 + 0x8a) & 0xf) * 6) +
                    *(int *)((short)((*(byte *)(unaff_A6 + 0x22) & 7) << 2) + 0x8fd5c));
  sVar4 = 0x20;
  sVar3 = *psVar5;
  if (sVar3 < 0) {
    sVar4 = -0x20;
  }
  if ((*(byte *)(unaff_A6 + 0x25) & 1) != 0) {
    sVar3 = sVar3 - sVar4;
  }
  sVar4 = psVar5[1];
  sVar1 = psVar5[2];
  unaff_A6[0xc] = (int)sVar3 << 8;
  *(int *)((int)unaff_A6 + 0x76) = (int)sVar4 << 8;
  unaff_A6[0xd] = (int)sVar1 << 8;
  if ((*(byte *)((int)unaff_A6 + 0x8a) & 0xf) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  }
  else if (7 < (*(byte *)((int)unaff_A6 + 0x8a) & 0xf)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  }
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  *unaff_A6 = 0x8f114;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_8f1e0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_8f1e0;
  }
  uVar2 = candidate_2eb4();
  *(char *)((int)unaff_A6 + 0x8a) = (char)(uVar2 >> 3);
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = candidate_8efde;
  }
  bVar6 = false;
  candidate_19504();
  if (bVar6) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
