/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2136; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_2136(void)

{
  DAT_ram_f808 = 1;
  if (DAT_ram_f812 == 0) {
    _DAT_ram_f806 = 0xffff;
    return DAT_ram_f812;
  }
  _DAT_ram_f806 = (ushort)*(byte *)CONCAT11(7,DAT_ram_f812) * 0x10;
  return ~*(byte *)CONCAT11(6,DAT_ram_f812);
}
