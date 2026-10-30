/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0164f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_164f0(void)

{
  ushort uVar1;
  short sVar2;
  short sVar3;
  uint uVar4;
  bool bVar5;

  uVar1 = 0x7000;
  sVar2 = 0x28;
  DAT_00300001 = 0;
  bVar5 = false;
  while (!bVar5) {
    uVar4 = (uint)uVar1 << 0x10;
    for (sVar3 = 0x20; sVar3 != 0; sVar3 = sVar3 + -1) {
      if (0x74ffffff < uVar4) {
        __m68k_trap(0xf);
      }
      uVar4 = uVar4 + 0x10000;
    }
    uVar1 = uVar1 + 0x20;
    sVar2 = sVar2 + -1;
    bVar5 = sVar2 == 0;
  }
  _DAT_003c0004 = 0;
  _DAT_003c0000 = 0x82000000;
  return uVar1;
}
