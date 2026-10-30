/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03c6ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_3c6ea(void)

{
  int iVar1;
  int iVar2;
  short sVar4;
  int iVar3;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;
  bool bVar5;

  func_0x000040ac();
  if ((bool)in_CF) {
    bVar5 = false;
    iVar2 = candidate_13cf8();
    *(char *)(unaff_A6 + 0x70) = -bVar5;
    if (bVar5) {
      iVar2 = *(int *)(extraout_A0 + 0x14) + iVar2;
      if (*(int *)(unaff_A6 + 0x38) != 0) {
        iVar2 = iVar2 - *(int *)(*(int *)(unaff_A6 + 0x38) + 0x14);
      }
      *(int *)(unaff_A6 + 0x28) = iVar2;
    }
  }
  candidate_1abc8();
  sVar4 = 0xff;
  iVar2 = *(int *)(unaff_A6 + 0xc);
  iVar1 = *(int *)(unaff_A6 + 0x40) - *(int *)(iVar2 + 0x40);
  iVar3 = iVar1 + -0x80000;
  if ((iVar1 + 0x20000U < 0xa0000) && (*(char *)(iVar2 + 0x6d) != '@')) {
    sVar4 = (short)((uint)(iVar3 * 0x10) >> 0x10);
    iVar3 = CONCAT22((short)(iVar3 * 0x10),sVar4 + 0x100);
  }
  *(char *)(unaff_A6 + 0x12) = (char)sVar4;
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(iVar2 + 0x1e);
  return iVar3;
}
