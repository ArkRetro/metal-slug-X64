/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01228c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1228c(void)

{
  ushort uVar1;
  byte bVar2;
  byte bVar3;
  byte in_D0b;

  candidate_1209c();
  uVar1 = _DAT_0010bfea;
  bVar2 = -(0xdf < in_D0b) | in_D0b + 0x20;
  bVar3 = DAT_0010bfec;
  if ((0x1f < bVar2) || (bVar3 = bVar2, bVar2 != DAT_0010bfec)) {
    DAT_0010bfec = bVar3;
    if (bVar2 == 3) {
      DAT_0010bfed = 0xff;
      return;
    }
    *(byte *)((short)_DAT_0010bfea + 0x10bf68) = bVar2;
    uVar1 = uVar1 + 1 & 0x7f;
    if (uVar1 == _DAT_0010bfe8) {
      DAT_0010bfec = 0xff;
      return;
    }
  }
  _DAT_0010bfea = uVar1;
  return;
}
