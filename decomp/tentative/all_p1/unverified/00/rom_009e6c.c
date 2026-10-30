/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009e6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_9e6c(void)

{
  undefined1 in_D0b;
  short sVar1;
  short sVar2;
  int iVar3;

  iVar3 = 0x1068ea;
  sVar1 = 0;
  sVar2 = 0xff;
  DAT_00300001 = in_D0b;
  do {
    if (*(short *)(iVar3 + 8) == -1) {
      *(undefined2 *)(iVar3 + 8) = 0;
      sVar1 = candidate_9e34();
    }
    iVar3 = iVar3 + 0x10;
    sVar1 = sVar1 + 0x10;
    sVar2 = sVar2 + -1;
  } while (sVar2 != -1);
  return sVar1;
}
