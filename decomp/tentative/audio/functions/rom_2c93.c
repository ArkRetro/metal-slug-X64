/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2c93; tentative_c. */

void z80_candidate_2c93(void)

{
  char cVar1;

  cVar1 = '\x04';
  z80_candidate_8(0x2100);
  z80_candidate_8(0x2801);
  z80_candidate_8(0x2802);
  z80_candidate_8(0x2805);
  z80_candidate_8(0x2806);
  while( true ) {
    z80_candidate_8(0x417f);
    z80_candidate_8(0x427f);
    z80_candidate_8(0x457f);
    z80_candidate_8(0x467f);
    z80_candidate_8(0x497f);
    z80_candidate_8(0x4a7f);
    z80_candidate_8(0x4d7f);
    z80_candidate_8(0x4e7f);
    if (cVar1 != '\x04') break;
    cVar1 = '\x06';
  }
  DAT_ram_f919 = 1;
  z80_candidate_2d07();
  DAT_ram_f981 = 2;
  z80_candidate_2d07();
  DAT_ram_f9e9 = 5;
  z80_candidate_2d07();
  DAT_ram_fa51 = 6;
  z80_candidate_2d07();
  DAT_ram_faa7 = 0;
  DAT_ram_faa8 = 0;
  return;
}
