/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4f67; tentative_c. */

void z80_candidate_4f67(void)

{
  char cVar1;

  if (DAT_ram_fdf2 == '@') {
    DAT_ram_fdf0 = 0;
    return;
  }
  cVar1 = z80_candidate_4f90(DAT_ram_fdeb,DAT_ram_fdf2);
  DAT_ram_fdf0 = cVar1 - DAT_ram_fdeb;
  return;
}
