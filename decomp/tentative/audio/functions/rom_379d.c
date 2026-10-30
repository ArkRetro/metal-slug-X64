/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x379d; tentative_c. */

void z80_candidate_379d(byte param_1,short param_2)

{
  char cVar1;
  byte bVar2;
  char cVar3;

  *(undefined1 *)(param_2 + 0xc) = 0xff;
  *(undefined1 *)(param_2 + 0xd) = 0xff;
  param_2 = param_2 + 0x14;
  cVar3 = '\x04';
  do {
    bVar2 = param_1 & 1;
    param_1 = param_1 >> 1;
    if ((bVar2 != 0) && (*(byte *)(param_2 + 8) < 0x7f)) {
      cVar1 = *(byte *)(param_2 + 8) + 1;
      *(char *)(param_2 + 8) = cVar1;
      *(char *)(param_2 + 1) = cVar1;
      *(byte *)(param_2 + 7) = *(byte *)(param_2 + 7) | 2;
    }
    param_2 = param_2 + 9;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  DAT_ram_fa4f = 0xff;
  return;
}
