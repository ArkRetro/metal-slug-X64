/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4388; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_4388(short param_1,short param_2)

{
  byte *pbVar1;

  DAT_ram_fbbe = 4;
  pbVar1 = _DAT_ram_fbb1;
  if (DAT_ram_ff13 != DAT_ram_fbb0) {
    DAT_ram_ff13 = DAT_ram_fbb0;
  }
  for (; (DAT_ram_fba7 != *pbVar1 && (*pbVar1 <= DAT_ram_fba7)); pbVar1 = pbVar1 + 6) {
  }
  DAT_ram_fbb5 = pbVar1[1];
  pbVar1 = pbVar1 + 2;
  z80_candidate_57be();
  DAT_ram_fbb3 = *pbVar1;
  DAT_ram_fbb4 = pbVar1[1];
  DAT_ram_fbc0 = pbVar1[2];
  DAT_ram_fbbf = pbVar1[3];
  _DAT_ram_fbb6 = *(undefined2 *)(pbVar1 + 4);
  _DAT_ram_fbb8 = *(undefined2 *)(pbVar1 + 6);
  _DAT_ram_fbba = *(undefined2 *)(pbVar1 + 8);
  _DAT_ram_fbbc = *(undefined2 *)(pbVar1 + 10);
  z80_candidate_21b9(0xfbc2);
  z80_candidate_22a5(0xfbd0);
  z80_candidate_22a5(0xfbde);
  z80_candidate_2412(*(undefined1 *)(param_1 + 0xe),param_2 + 0x4d,param_2);
  return;
}
