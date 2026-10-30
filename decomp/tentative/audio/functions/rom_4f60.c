/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4f60; tentative_c. */

void z80_candidate_4f60(undefined1 param_1,undefined2 param_2)

{
  char cVar1;

  DAT_ram_fdf2 = (char)((ushort)param_2 >> 8);
  DAT_ram_fdf1 = param_1;
  if (DAT_ram_fdf2 == '@') {
    DAT_ram_fdf0 = 0;
    return;
  }
  cVar1 = z80_candidate_4f90(DAT_ram_fdeb,DAT_ram_fdf2);
  DAT_ram_fdf0 = cVar1 - DAT_ram_fdeb;
  return;
}
