/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x50cf; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_50cf(undefined2 param_1)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  char cVar4;
  short sVar5;

  sVar5 = -0x20a;
  cVar4 = '\b';
  bVar1 = 0;
  bVar3 = (byte)param_1;
  do {
    bVar2 = (byte)((ushort)param_1 >> 8);
    if ((*(byte *)(sVar5 + 9) & 4) == 0) {
LAB_ram_5127:
      bVar1 = 2;
      _DAT_ram_ff0e = sVar5;
      if (bVar2 == 0) {
        return 0;
      }
    }
    else {
      if (((bVar2 != 0) && (bVar2 == *(byte *)(sVar5 + 3))) &&
         (_DAT_ram_f816 != *(short *)(sVar5 + 7))) {
        if (*(byte *)(sVar5 + 2) < (byte)param_1) {
          return *(byte *)(sVar5 + 2);
        }
        func_0x5781();
        goto LAB_ram_5127;
      }
      bVar2 = bVar1;
      if ((bVar1 != 2) && (bVar2 = *(byte *)(sVar5 + 2), bVar3 <= bVar2)) {
        bVar3 = *(byte *)(sVar5 + 2);
        bVar1 = 1;
        _DAT_ram_ff0e = sVar5;
      }
    }
    sVar5 = sVar5 + 0x23;
    cVar4 = cVar4 + -1;
    if (cVar4 == '\0') {
      return bVar2;
    }
  } while( true );
}
