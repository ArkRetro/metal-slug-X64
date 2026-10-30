/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x22a5; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_22a5(short param_1)

{
  byte bVar1;
  byte bVar2;
  char in_R;

  *(undefined1 *)(param_1 + 4) = *(undefined1 *)(param_1 + 0xd);
  *(undefined1 *)(param_1 + 5) = 0;
  *(undefined1 *)(param_1 + 6) = 0;
  bVar2 = (byte)((ushort)_DAT_ram_f81a >> 8);
  bVar1 = in_R + (char)_DAT_ram_f81a ^ bVar2;
  _DAT_ram_f81a = CONCAT11(bVar1,bVar2);
  *(byte *)(param_1 + 8) = bVar1;
  *(undefined1 *)(param_1 + 10) = 0;
  *(undefined1 *)(param_1 + 0xc) = 0;
  return;
}
