/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aab84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_aab84(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A5;
  undefined1 uVar3;

  uVar3 = false;
  uVar1 = func_0x000040ac();
  if ((((bool)uVar3) &&
      (uVar3 = uVar1 < *(ushort *)(unaff_A5 + 0x4608), uVar1 == *(ushort *)(unaff_A5 + 0x4608))) &&
     (uVar3 = 0, candidate_4624[unaff_A5] != (code)0x0)) {
    return 0;
  }
  sVar2 = func_0x000040ac();
  if ((((bool)uVar3) && (sVar2 == *(short *)(unaff_A5 + 0x462c))) &&
     (*(char *)(unaff_A5 + 0x4648) != '\0')) {
    return 1;
  }
  return 0xffffffff;
}
