/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04e5d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_4e5d0(void)

{
  ushort extraout_D0u;
  uint in_D0;
  uint uVar1;
  uint in_D1;
  undefined2 extraout_D1w;
  byte bVar2;

  bVar2 = 0;
  if ((int)in_D0 < 0) {
    in_D0 = -in_D0;
    bVar2 = 0xff;
  }
  if ((int)in_D1 < 0) {
    in_D1 = -in_D1;
    bVar2 = ~bVar2;
  }
  if ((in_D0 < 0x10000) && (in_D1 < 0x10000)) {
    uVar1 = in_D0;
    if ((in_D1 != 0x10000) && (uVar1 = in_D1, in_D0 != 0x10000)) {
      uVar1 = (in_D0 & 0xffff) * (in_D1 & 0xffff) >> 0x10;
    }
    if (bVar2 != 0) {
      uVar1 = -uVar1;
    }
    return uVar1;
  }
  candidate_4e5be();
  return CONCAT22(extraout_D0u,extraout_D1w) << 0x10 | (uint)extraout_D0u;
}
