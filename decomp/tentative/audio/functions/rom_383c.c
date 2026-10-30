/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x383c; tentative_c. */

void z80_candidate_383c(byte param_1,short param_2)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;

  *(undefined1 *)(param_2 + 0xc) = 0xff;
  *(undefined1 *)(param_2 + 0xd) = 0xff;
  pcVar3 = (char *)(param_2 + 0x14);
  cVar2 = '\x04';
  do {
    bVar1 = param_1 & 1;
    param_1 = param_1 >> 1;
    if ((bVar1 != 0) && (*pcVar3 != '\0')) {
      *pcVar3 = *pcVar3 + -1;
      pcVar3[7] = pcVar3[7] | 1;
    }
    pcVar3 = pcVar3 + 9;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}
