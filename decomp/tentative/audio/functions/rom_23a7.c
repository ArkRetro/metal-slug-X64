/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x23a7; tentative_c. */

void z80_candidate_23a7(byte param_1,short param_2)

{
  byte bVar1;
  char cVar2;

  bVar1 = -CARRY1(param_1,param_1);
  *(byte *)(param_2 + 6) = (bVar1 ^ *(byte *)(param_2 + 6)) & 0xbf ^ bVar1;
  bVar1 = (param_1 ^ bVar1) + CARRY1(param_1,param_1);
  if (bVar1 == 0x80) {
    bVar1 = 0x7f;
  }
  cVar2 = *(char *)CONCAT11(1,bVar1 | 0x80);
  *(byte *)(param_2 + 7) =
       (byte)(*(byte *)CONCAT11(1,bVar1) >> 1 | cVar2 << 7) >> 1 | (cVar2 >> 1) << 7;
  *(char *)(param_2 + 8) = cVar2 >> 2;
  return;
}
