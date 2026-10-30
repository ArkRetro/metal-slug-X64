/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002976.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2976(void)

{
  ushort *puVar1;
  ushort in_D0w;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;

  if (in_D0w != 0) {
    in_D0w = _DAT_0010f006 + 1;
    _DAT_0010f006 = in_D0w;
  }
  sVar5 = 0x36;
  uVar3 = in_D0w;
  uVar4 = in_D0w;
  puVar1 = (ushort *)0x100016;
  do {
    puRam00100084 = puVar1;
    *puRam00100084 = in_D0w;
    uVar2 = in_D0w * 2;
    if (CARRY2(in_D0w,in_D0w)) {
      uVar2 = ~uVar2;
    }
    uVar3 = uVar2 ^ uVar3 + 0x22b7;
    uVar3 = uVar3 << 1 | uVar3 >> 0xf;
    uVar4 = uVar3 ^ uVar4 + 0x4555;
    uVar4 = uVar4 << 1 | uVar4 >> 0xf;
    in_D0w = (uVar4 ^ uVar2) << 1 | (uVar4 ^ uVar2) >> 0xf;
    sVar5 = sVar5 + -1;
    puVar1 = puRam00100084 + 1;
  } while (sVar5 != -1);
  return;
}
