/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030284.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int candidate_30284(void)

{
  int iVar1;
  undefined2 in_D0w;
  int iVar2;
  int iVar3;
  char unaff_D6b;
  char cVar4;
  char unaff_D7b;

  iVar2 = CONCAT22(in_D0w,0x800);
  for (; unaff_D7b != '\0'; unaff_D7b = unaff_D7b + -1) {
    iVar1 = CONCAT31((int3)((uint)iVar2 >> 8),0x20);
    for (cVar4 = unaff_D6b; iVar3 = iVar1, cVar4 != '\0'; cVar4 = cVar4 + -1) {
      iVar1 = iVar3 + 0x200000;
      _DAT_003c0000 = iVar3;
    }
    iVar2 = iVar2 + 0x10000;
  }
  return iVar2;
}
