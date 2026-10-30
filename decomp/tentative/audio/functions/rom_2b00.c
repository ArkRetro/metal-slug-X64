/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2b00; tentative_c. */

void z80_candidate_2b00(undefined2 param_1,short param_2,short param_3)

{
  bool bVar1;
  undefined2 in_AF;
  undefined1 uVar2;
  byte bVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  byte bVar7;
  undefined2 uVar8;
  byte *pbVar9;

  uVar2 = (undefined1)((ushort)in_AF >> 8);
  bVar7 = (byte)param_1;
  if (bRamf8cb < bVar7) {
    uVar8 = CONCAT11(4,uVar2);
    bRamf8cb = bVar7;
    z80_candidate_8(4,param_1,uVar8);
    z80_candidate_8(CONCAT11((char)((ushort)uVar8 >> 8) + '\x01',(char)param_1));
  }
  else {
    cVar4 = '\x05';
    bRamf8cb = bVar7;
    z80_candidate_8(4);
    z80_candidate_8(CONCAT11(cVar4 + -1,uVar2));
  }
  cVar4 = DAT_ram_f8c1;
  if (((DAT_ram_f8c1 != '\0') && (DAT_ram_f811 != -1)) &&
     (cVar4 = DAT_ram_f811, DAT_ram_f811 != '\0')) {
    bVar3 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar7 = *(byte *)CONCAT11(6,DAT_ram_f8c1);
    cVar4 = -CARRY1(bVar3,bVar7);
    if (CARRY1(bVar3,bVar7)) {
      cVar4 = *(char *)CONCAT11(7,bVar3 + bVar7);
    }
  }
  cVar6 = *(char *)(param_2 + 0x12);
  cVar5 = cVar4;
  if (((cVar4 != '\0') && (cVar6 != -1)) && (cVar5 = cVar6, cVar6 != '\0')) {
    bVar3 = *(byte *)CONCAT11(6,cVar6);
    bVar7 = *(byte *)CONCAT11(6,cVar4);
    cVar5 = -CARRY1(bVar3,bVar7);
    if (CARRY1(bVar3,bVar7)) {
      cVar5 = *(char *)CONCAT11(7,bVar3 + bVar7);
    }
  }
  cVar4 = -1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    cVar4 = DAT_ram_f805;
  }
  cVar6 = cVar4;
  if (((cVar4 != '\0') && (cVar5 != -1)) && (cVar6 = cVar5, cVar5 != '\0')) {
    bVar3 = *(byte *)CONCAT11(6,cVar5);
    bVar7 = *(byte *)CONCAT11(6,cVar4);
    cVar6 = -CARRY1(bVar3,bVar7);
    if (CARRY1(bVar3,bVar7)) {
      cVar6 = *(char *)CONCAT11(7,bVar3 + bVar7);
    }
  }
  bVar1 = cVar6 != cRamf8c6;
  cRamf8c6 = cVar6;
  if (bVar1) {
    z80_candidate_8(4,CONCAT11(10,*(undefined1 *)CONCAT11(10,cVar6)));
  }
  bVar7 = 1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar7 = DAT_ram_f809;
  }
  DAT_ram_f8bc = (DAT_ram_f8bc + bVar7) - CARRY1(DAT_ram_f8bc,bVar7);
  if (DAT_ram_f8bb == '\0') {
    if (((*(byte *)(param_2 + 9) & 0x10) != 0) || ((bRamf8b9 & 0x10) == 0)) goto LAB_ram_2bd2;
    bRamf8b9 = bRamf8b9 & 0xef;
  }
  else {
    DAT_ram_f8bb = DAT_ram_f8bb - bVar7;
    if (DAT_ram_f8bb != '\0') goto LAB_ram_2bd2;
    if ((*(byte *)(param_2 + 9) & 0x10) != 0) {
      *(byte *)(param_3 + 3) = *(byte *)(param_3 + 3) | 0x10;
      goto LAB_ram_2bd2;
    }
  }
  z80_candidate_21ca(0xf8cc);
LAB_ram_2bd2:
  cVar4 = cRamf904;
  cRamf904 = DAT_ram_f903;
  pbVar9 = &DAT_ram_f905;
  if (cVar4 != DAT_ram_f903) {
    z80_candidate_8(4);
  }
  bVar3 = *pbVar9;
  bVar7 = pbVar9[1];
  pbVar9[1] = bVar3;
  if (bVar7 == bVar3) {
    return;
  }
  z80_candidate_8(4,CONCAT11(7,(~bVar3 & 7) * '\b' | bVar3));
  return;
}
