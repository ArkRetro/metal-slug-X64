/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01209c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1209c(void)

{
  ushort uVar1;
  byte bVar2;
  byte in_D0b;

  uVar1 = _DAT_0010bfea;
  bVar2 = DAT_0010bfec;
  if ((0x1f < in_D0b) || (bVar2 = in_D0b, in_D0b != DAT_0010bfec)) {
    DAT_0010bfec = bVar2;
    if (in_D0b == 3) {
      DAT_0010bfed = 0xff;
      return;
    }
    *(byte *)((short)_DAT_0010bfea + 0x10bf68) = in_D0b;
    uVar1 = uVar1 + 1 & 0x7f;
    if (uVar1 == _DAT_0010bfe8) {
      DAT_0010bfec = 0xff;
      return;
    }
  }
  _DAT_0010bfea = uVar1;
  return;
}
