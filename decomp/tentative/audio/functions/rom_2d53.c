/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2d53; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_2d53(byte param_1,byte param_2,short param_3,undefined2 param_4)

{
  byte bVar1;
  byte bVar2;
  char cVar3;

  cVar3 = (char)((ushort)param_4 >> 8);
  bVar1 = param_1 >> 1;
  if ((bool)(param_1 & 1)) {
    z80_candidate_8();
  }
  if (bVar1 == 0) {
    return bVar1;
  }
  cVar3 = cVar3 + '\x10';
  param_3 = param_3 + 1;
  bVar2 = bVar1 >> 1;
  if ((bool)(bVar1 & 1)) {
    z80_candidate_8();
  }
  if (bVar2 == 0) {
    return bVar2;
  }
  cVar3 = cVar3 + '\x10';
  param_3 = param_3 + 1;
  bVar1 = bVar2 >> 1;
  if ((bool)(bVar2 & 1)) {
    z80_candidate_8();
  }
  if (bVar1 == 0) {
    return bVar1;
  }
  cVar3 = cVar3 + '\x10';
  param_3 = param_3 + 1;
  bVar2 = bVar1 >> 1;
  if ((bool)(bVar1 & 1)) {
    z80_candidate_8();
  }
  if (bVar2 == 0) {
    return bVar2;
  }
  cVar3 = cVar3 + '\x10';
  param_3 = param_3 + 1;
  bVar1 = bVar2 >> 1;
  if ((bool)(bVar2 & 1)) {
    z80_candidate_8();
  }
  if (bVar1 == 0) {
    return bVar1;
  }
  cVar3 = cVar3 + '\x10';
  param_3 = param_3 + 1;
  bVar2 = bVar1 >> 1;
  if ((bool)(bVar1 & 1)) {
    z80_candidate_8();
  }
  if (bVar2 == 0) {
    return bVar2;
  }
  bVar1 = cVar3 + 0x10;
  if (!(bool)(bVar2 & 1)) {
    return bVar1;
  }
  do {
  } while (cIo0004 < '\0');
  _DAT_ram_f80b = CONCAT11(bVar1,param_2);
  *(byte *)(ushort)param_2 = bVar1;
  *(byte *)(ushort)(byte)(param_2 + 1) = *(byte *)(param_3 + 1);
  return *(byte *)(param_3 + 1);
}
