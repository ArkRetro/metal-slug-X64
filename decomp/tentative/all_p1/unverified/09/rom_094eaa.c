/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x094eaa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_94eaa(void)

{
  int iVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined8 uVar3;

  candidate_120f8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  candidate_4256();
  candidate_9ea8();
  uVar3 = candidate_2e7c();
  iVar1 = *(int *)(unaff_A6[3] + 0x30);
  iVar2 = *(short *)((int)unaff_A6 + 0x6e) * 0x20;
  unaff_A6[0xc] = iVar2 + iVar1 + (int)((ulonglong)uVar3 >> 0x20);
  unaff_A6[0xd] = iVar2 + ((int)uVar3 - iVar1);
  *unaff_A6 = candidate_94f1c;
  return;
}
