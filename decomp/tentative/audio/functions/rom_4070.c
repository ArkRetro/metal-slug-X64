/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4070; tentative_c. */

void z80_candidate_4070(void)

{
  undefined *puVar1;

  puVar1 = &DAT_ram_fb9f;
  z80_candidate_1f96();
  puVar1[2] = 3;
  DAT_ram_fbc0 = 0;
  DAT_ram_fbc1 = 0;
  DAT_ram_fbbe = 0;
  func_0x0030(puVar1 + 0x23);
  z80_candidate_224e(puVar1 + 0x3f);
  z80_candidate_224e(puVar1 + 0x31);
  z80_candidate_239f(puVar1 + 0x4d,puVar1);
  return;
}
