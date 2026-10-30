/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x1f6d; tentative_c. */

char z80_candidate_1f6d(char param_1,undefined2 param_2)

{
  char cVar1;
  byte *pbVar2;
  char cVar3;

  if (param_1 == '\0') {
    return param_1;
  }
  pbVar2 = (byte *)CONCAT11(6,(char)param_2);
  if (CARRY1(*(byte *)CONCAT11(6,param_1),*pbVar2)) {
    pbVar2 = (byte *)0x600;
  }
  cVar3 = (char)((ushort)pbVar2 >> 8);
  cVar1 = *(char *)CONCAT11(cVar3,param_1) + *(char *)CONCAT11(cVar3,(char)((ushort)param_2 >> 8));
  if (cVar3 != -1) {
    return cVar1;
  }
  return *(char *)CONCAT11(cVar3 + '\x01',cVar1);
}
