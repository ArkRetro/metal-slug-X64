/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x0008; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_8(byte param_1,undefined2 param_2)

{
  undefined1 uVar1;

  do {
  } while (cIo0004 < '\0');
  uVar1 = (undefined1)((ushort)param_2 >> 8);
  _DAT_ram_f80b = CONCAT11(uVar1,param_1);
  *(undefined1 *)(ushort)param_1 = uVar1;
  *(char *)(ushort)(byte)(param_1 + 1) = (char)param_2;
  return;
}
