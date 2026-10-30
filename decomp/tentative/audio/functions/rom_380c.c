/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x380c; tentative_c. */

void z80_candidate_380c(byte param_1,short param_2)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;

  *(undefined1 *)(param_2 + 0xc) = 0xff;
  *(undefined1 *)(param_2 + 0xd) = 0xff;
  pbVar3 = (byte *)(param_2 + 0x14);
  cVar2 = '\x04';
  do {
    bVar1 = param_1 & 1;
    param_1 = param_1 >> 1;
    if ((bVar1 != 0) && (*pbVar3 < 0xf)) {
      *pbVar3 = *pbVar3 + 1;
      pbVar3[7] = pbVar3[7] | 1;
    }
    pbVar3 = pbVar3 + 9;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}
