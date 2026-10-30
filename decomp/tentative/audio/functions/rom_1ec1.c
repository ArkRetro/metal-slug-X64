/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x1ec1; tentative_c. */

byte z80_candidate_1ec1(void)

{
  byte bVar1;

  if (DAT_ram_ffee != DAT_ram_ffef) {
    bVar1 = DAT_ram_ffee & 0xf;
    DAT_ram_ffee = bVar1 + 1 & 0xf;
    return *(byte *)(bVar1 - 0x10);
  }
  return DAT_ram_ffee;
}
