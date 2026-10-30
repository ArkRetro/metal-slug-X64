/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4b84; tentative_c. */

void z80_candidate_4b84(short param_1)

{
  char cVar1;
  undefined1 uVar2;
  short sVar3;
  short sVar4;

  sVar4 = (ushort)*(byte *)(param_1 + 10) * 4;
  cVar1 = *(byte *)(param_1 + 0xb) + 1;
  if (cVar1 == '\0') {
    sVar3 = 0x24ba;
    uVar2 = 0;
  }
  else {
    sVar4 = (ushort)*(byte *)(param_1 + 0xb) * 0x200 + sVar4;
    sVar3 = 0x80;
    uVar2 = z80_candidate_57be(cVar1,0x80);
  }
  z80_candidate_57be(uVar2,sVar3 + sVar4);
  return;
}
