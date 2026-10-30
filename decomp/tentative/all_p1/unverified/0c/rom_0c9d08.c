/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c9d08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c9d08(void)

{
  int iVar1;
  undefined2 uVar3;
  int iVar2;
  ushort uVar4;
  short sVar5;
  ushort uVar6;
  ushort *puVar7;
  int unaff_A6;

  puVar7 = (ushort *)&DAT_0010c5fc;
  if (*(short *)(unaff_A6 + 0x6e) != 0) {
    puVar7 = (ushort *)&DAT_0010c600;
  }
  uVar6 = *puVar7;
  if (0xd < uVar6) {
    uVar6 = 0xe;
  }
  if ((char)uVar6 == *(char *)(unaff_A6 + 0x8b)) {
    return;
  }
  *(char *)(unaff_A6 + 0x8b) = (char)uVar6;
  sVar5 = 0xe - uVar6;
  if (*(short *)(unaff_A6 + 0x6e) == 0) {
    uVar3 = 0x707c;
    uVar4 = 0x20;
  }
  else {
    uVar3 = 0x749c;
    uVar4 = 0xffe0;
  }
  iVar1 = CONCAT22(uVar3,0x310);
  for (; iVar2 = iVar1, uVar6 != 0; uVar6 = uVar6 - 1) {
    iVar1 = (uint)uVar4 * 0x10000 + iVar2;
    _DAT_003c0000 = iVar2;
  }
  iVar1 = CONCAT22((short)((uint)iVar2 >> 0x10),800);
  for (; iVar2 = iVar1, sVar5 != 0; sVar5 = sVar5 + -1) {
    iVar1 = (uint)uVar4 * 0x10000 + iVar2;
    _DAT_003c0000 = iVar2;
  }
  return;
}
