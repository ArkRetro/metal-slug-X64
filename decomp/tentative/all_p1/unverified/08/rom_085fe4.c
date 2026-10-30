/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085fe4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_85fe4(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int unaff_A6;
  undefined8 uVar5;

  iVar1 = *(int *)(*(int *)(unaff_A6 + 0x6e) + 0x3c);
  iVar2 = *(int *)(unaff_A6 + 0x3c);
  iVar3 = *(int *)(*(int *)(unaff_A6 + 0x6e) + 0x40);
  iVar4 = *(int *)(unaff_A6 + 0x40);
  candidate_2f1a();
  uVar5 = candidate_2e7c();
  return CONCAT44(-((int)((ulonglong)uVar5 >> 0x20) * 0x100 - (iVar1 - iVar2)),
                  -((int)uVar5 * 0x100 - ((iVar3 + 0x100000) - iVar4)));
}
