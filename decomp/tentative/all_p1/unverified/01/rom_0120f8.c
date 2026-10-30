/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0120f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_120f8(void)

{
  byte bVar1;
  ushort in_D0w;
  byte bVar2;
  ushort uVar3;

  if ((0x1fff < in_D0w) || (bVar2 = (byte)in_D0w, bVar2 < 0x20)) {
    return;
  }
  uVar3 = in_D0w & 0xff00;
  if (uVar3 == 0x100) {
    return;
  }
  if (uVar3 != 0x300) {
    if (uVar3 == 0x200) {
      return;
    }
    if (DAT_00100088 == '\x01') {
      if (uVar3 == 0) {
        return;
      }
      if (((DAT_0010fd82 != '\0') && (DAT_0010fe80 == '\0')) && (DAT_00d00046 != '\0')) {
        return;
      }
      if (DAT_0010fd8d == '\0') {
        return;
      }
    }
    if ((DAT_0010bff0 != '\0') && (0xfff < in_D0w)) {
      return;
    }
    candidate_1209c();
    uVar3 = _DAT_0010bfea;
    bVar1 = DAT_0010bfec;
    if ((0x1f < bVar2) || (bVar1 = bVar2, bVar2 != DAT_0010bfec)) {
      DAT_0010bfec = bVar1;
      if (bVar2 == 3) {
        DAT_0010bfed = 0xff;
        return;
      }
      *(byte *)((short)_DAT_0010bfea + 0x10bf68) = bVar2;
      uVar3 = uVar3 + 1 & 0x7f;
      if (uVar3 == _DAT_0010bfe8) {
        DAT_0010bfec = 0xff;
        return;
      }
    }
    _DAT_0010bfea = uVar3;
    return;
  }
  return;
}
