/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x001000.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0000109e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_00001070(void)

{
  short sVar1;
  char cVar2;
  int in_A1;
  short *psVar3;
  short *psVar4;

  if (DAT_0010fdae < 2) {
    return;
  }
  cVar2 = DAT_00d00047;
  if (DAT_00d00047 != '\0') {
    DAT_00320000 = 1;
    cVar2 = func_0x00c004c2();
    if (DAT_0010fd82 == '\0') {
      return;
    }
    in_A1 = 0x3a0000;
    if (_DAT_0010fe80 != 0x31737472) {
      psVar3 = (short *)0x10ee;
      psVar4 = (short *)0x10fc00;
      do {
        *psVar4 = *psVar3;
        sVar1 = *psVar3;
        psVar3 = psVar3 + 1;
        psVar4 = psVar4 + 1;
      } while (sVar1 != 0x4e75);
      (*(code *)0x10fc00)();
      return;
    }
  }
  *(char *)(in_A1 + 0x1d) = cVar2;
  DAT_00d00058 = DAT_00d00108;
  *(char *)(in_A1 + 0xd) = cVar2;
  return;
}
