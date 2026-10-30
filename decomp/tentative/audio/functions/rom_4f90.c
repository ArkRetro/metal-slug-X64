/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4f90; tentative_c. */

char z80_candidate_4f90(char param_1,undefined1 param_2)

{
  byte bVar1;
  byte bVar2;
  char cVar3;

  bVar2 = *(byte *)CONCAT11(6,param_1 + -0x80);
  bVar1 = bVar2 + *(byte *)CONCAT11(6,param_2);
  if (CARRY1(bVar2,*(byte *)CONCAT11(6,param_2))) {
    cVar3 = bVar1 + 0x40;
    if (0xbf < bVar1) {
      cVar3 = -1;
    }
  }
  else {
    cVar3 = bVar1 + 0x40;
    if (bVar1 < 0xc0) {
      cVar3 = '\0';
    }
  }
  return *(char *)CONCAT11(7,cVar3) + -0x80;
}
