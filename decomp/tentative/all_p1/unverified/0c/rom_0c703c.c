/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c703c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c703c(void)

{
  undefined1 in_D0b;
  short sVar1;
  int iVar2;
  int in_A0;

  if (-1 < *(short *)(in_A0 + 0x2c)) {
    sVar1 = 0x3ff;
    iVar2 = (uint)(ushort)((*(short *)(in_A0 + 0x2c) + 1) * 0x40) << 0x10;
    do {
      _DAT_003c0000 = iVar2 + 0x10000;
      iVar2 = iVar2 + 0x20000;
      sVar1 = sVar1 + -1;
      DAT_00300001 = in_D0b;
    } while (sVar1 != -1);
  }
  return;
}
