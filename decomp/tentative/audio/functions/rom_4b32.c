/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4b32; tentative_c. */

void z80_candidate_4b32(short param_1)

{
  byte in_F;

  z80_candidate_4b09();
  if ((in_F & 1) == 0) {
    return;
  }
  z80_candidate_4b2a();
  if ((in_F & 1) != 0) {
    z80_candidate_379d();
    return;
  }
  *(byte *)(param_1 + 0x12) = -(1 < *(byte *)(param_1 + 0x12)) & *(byte *)(param_1 + 0x12) - 2;
  return;
}
