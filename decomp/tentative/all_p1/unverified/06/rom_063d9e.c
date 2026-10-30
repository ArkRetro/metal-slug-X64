/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x063d9e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_63d9e(void)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int extraout_D1;
  undefined4 *unaff_A6;
  undefined8 uVar4;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_4256();
  unaff_A6[0xc] = *(int *)(unaff_A6[3] + 0x30) >> 1;
  unaff_A6[0xd] = *(int *)(unaff_A6[3] + 0x34) >> 1;
  uVar4 = candidate_2e7c();
  iVar1 = (int)((ulonglong)uVar4 >> 0x20);
  iVar3 = (int)uVar4;
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    iVar1 = -iVar1;
  }
  if ((*(byte *)(unaff_A6[3] + 0x15) & 2) != 0) {
    iVar3 = -iVar3;
  }
  *(int *)((int)unaff_A6 + 0x6e) = iVar1;
  *(int *)((int)unaff_A6 + 0x72) = iVar3;
  sVar2 = candidate_29c6();
  if (sVar2 < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  unaff_A6[0xc] = ((int)sVar2 >> 2) + unaff_A6[0xc];
  candidate_29c6();
  unaff_A6[0xd] = extraout_D1 + unaff_A6[0xd];
  candidate_1abc8();
  *unaff_A6 = candidate_63e48;
  return;
}
