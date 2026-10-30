/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4363; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 z80_candidate_4363(undefined1 param_1,short param_2)

{
  *(short *)(param_2 + 9) =
       CONCAT11(*(undefined1 *)CONCAT11(1,*(byte *)(param_2 + 8) | 0x80),
                *(undefined1 *)CONCAT11(1,*(byte *)(param_2 + 8))) -
       CONCAT11(*(undefined1 *)CONCAT11(1,*(byte *)(param_2 + 0x16) | 0x80),
                *(undefined1 *)CONCAT11(1,*(byte *)(param_2 + 0x16)));
  _DAT_ram_fbad = 0xffff;
  return param_1;
}
