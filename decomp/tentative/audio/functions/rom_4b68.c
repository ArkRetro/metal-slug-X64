/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4b68; tentative_c. */

void z80_candidate_4b68(void)

{
  byte in_F;

  z80_candidate_4b09();
  if ((in_F & 1) == 0) {
    return;
  }
  z80_candidate_4b2a();
  if (!(bool)(in_F & 1)) {
    return;
  }
  z80_candidate_380c();
  return;
}
