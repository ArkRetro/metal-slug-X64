/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x3740; tentative_c. */

void z80_candidate_3740(short param_1,short param_2)

{
  byte bVar1;

  bVar1 = *(byte *)(param_2 + 0x12);
  if (bVar1 != *(byte *)(param_2 + 0x3e)) {
    *(byte *)(param_2 + 0x3e) = bVar1;
    z80_candidate_8(4);
  }
  *(byte *)(param_2 + 0x3d) = *(byte *)(param_2 + 0x13) & 0xf0 | bVar1;
  if ((*(byte *)(param_2 + 0x39) & 0x40) != 0) {
    bVar1 = DAT_ram_faa7 & 7;
    DAT_ram_faa7 = DAT_ram_faa7 & 0xf | 8;
    z80_candidate_8(4,CONCAT11(0x22,bVar1));
    DAT_ram_faa8 = 0xff;
  }
  z80_candidate_22a5(param_2 + 0x3f);
  z80_candidate_22a5(param_2 + 0x4d);
  z80_candidate_2412(*(undefined1 *)(param_1 + 0xe),param_2 + 0x5b,param_2);
  return;
}
