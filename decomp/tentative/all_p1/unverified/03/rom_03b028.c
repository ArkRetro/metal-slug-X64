/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03b028.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3b028(void)

{
  ushort uVar1;
  uint uVar2;
  int iVar3;
  short sVar4;

  iVar3 = 0xe07e8;
  uVar2 = 4;
  do {
    sVar4 = 6;
    do {
      iVar3 = iVar3 + 2;
      candidate_3cf60(sVar4,uVar2,iVar3);
      sVar4 = sVar4 + -1;
    } while (sVar4 != 0);
    uVar1 = (short)uVar2 - 1;
    uVar2 = (uint)uVar1;
  } while (uVar1 != 0);
  return;
}
