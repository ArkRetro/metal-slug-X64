/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x22cf; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_22cf(char *param_1,undefined2 param_2)

{
  bool bVar1;
  byte bVar2;
  byte bVar3;
  char cVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  char in_R;

  bVar7 = param_1[1];
  bVar6 = param_1[2];
  cVar4 = (char)((ushort)param_2 >> 8);
  bVar8 = param_1[3];
  if (*param_1 == '\0') {
    if (bVar6 == 0) {
      return 0;
    }
    bVar7 = cVar4 + bVar7;
    if (bVar7 == 0) {
      return 0;
    }
  }
  else {
    if (bVar7 == 0) {
      return 0;
    }
    bVar5 = cVar4 * '\x02';
    bVar3 = bVar5;
    if (((bVar5 != 0) && (bVar8 != 0xff)) && (bVar3 = bVar8, bVar8 != 0)) {
      bVar2 = *(byte *)CONCAT11(6,bVar8);
      bVar8 = *(byte *)CONCAT11(6,bVar5);
      bVar3 = -CARRY1(bVar2,bVar8);
      if (CARRY1(bVar2,bVar8)) {
        bVar3 = *(byte *)CONCAT11(7,bVar2 + bVar8);
      }
    }
    bVar1 = CARRY1(bVar3,bVar6);
    bVar6 = bVar3 + bVar6;
    if (bVar6 == 0) {
      return 0;
    }
    if (bVar1) {
      bVar6 = 0xff;
    }
  }
  cVar4 = param_1[4];
  if (cVar4 != '\0') {
    param_1[4] = cVar4 + -1;
    return 0;
  }
  bVar2 = param_1[5];
  bVar8 = bVar2 + bVar7;
  bVar3 = bVar8 + 1;
  param_1[5] = bVar3;
  bVar5 = param_1[6] + (CARRY1(bVar2,bVar7) || 0xfe < bVar8);
  param_1[6] = bVar5;
  cVar4 = param_1[7];
  if (cVar4 != '\x01') {
    if (cVar4 == '\x02') {
      bVar3 = ~bVar3;
    }
    else if (cVar4 == '\x03') {
      bVar3 = -!(bool)(bVar5 & 1);
    }
    else if (cVar4 == '\x04') {
      if (CARRY1(bVar2,bVar7) || 0xfe < bVar8) {
        bVar8 = (byte)((ushort)_DAT_ram_f81a >> 8);
        bVar7 = in_R + (char)_DAT_ram_f81a ^ bVar8;
        _DAT_ram_f81a = CONCAT11(bVar7,bVar8);
        param_1[8] = bVar7;
      }
      bVar3 = param_1[8];
    }
    else {
      bVar3 = -(bVar5 & 1) ^ bVar3;
    }
  }
  bVar2 = param_1[9];
  bVar8 = param_1[10];
  param_1[10] = bVar2 + bVar8;
  bVar5 = param_1[0xc];
  bVar7 = param_1[0xb] + bVar5;
  bVar7 = -(CARRY1(param_1[0xb],bVar5) || CARRY1(bVar7,CARRY1(bVar2,bVar8))) |
          bVar7 + CARRY1(bVar2,bVar8);
  param_1[0xc] = bVar7;
  bVar8 = bVar7;
  if (((bVar7 != 0) && (bVar6 != 0xff)) && (bVar8 = bVar6, bVar6 != 0)) {
    bVar5 = *(byte *)CONCAT11(6,bVar6);
    bVar6 = *(byte *)CONCAT11(6,bVar7);
    bVar8 = -CARRY1(bVar5,bVar6);
    if (CARRY1(bVar5,bVar6)) {
      bVar8 = *(byte *)CONCAT11(7,bVar5 + bVar6);
    }
  }
  bVar6 = bVar8;
  if (((bVar8 != 0) && (bVar3 != 0xff)) && (bVar6 = bVar3, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar7 = *(byte *)CONCAT11(6,bVar8);
    bVar6 = -CARRY1(bVar3,bVar7);
    if (CARRY1(bVar3,bVar7)) {
      bVar6 = *(byte *)CONCAT11(7,bVar3 + bVar7);
    }
  }
  return bVar6;
}
