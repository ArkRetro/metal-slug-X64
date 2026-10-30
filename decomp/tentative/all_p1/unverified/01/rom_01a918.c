/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a918.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1a918(void)

{
  short sVar1;
  int iVar2;

  iVar2 = -0x7ffe0001;
  sVar1 = 0x17b;
  do {
    _DAT_003c0000 = iVar2;
    iVar2 = iVar2 + 0x10000;
    DAT_00300001 = (char)sVar1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != -1);
  return;
}
