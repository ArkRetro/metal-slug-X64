/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2ad9; tentative_c. */

void z80_candidate_2ad9(undefined2 param_1,short param_2,short param_3)

{
  bool bVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  byte bVar6;
  undefined2 uVar7;
  byte *pbVar8;

  bVar2 = (byte)((ushort)param_1 >> 8);
  bVar6 = (byte)param_1 >> 4;
  if (bRamf8cb < bVar2) {
    uVar7 = CONCAT11(4,bVar6);
    bRamf8cb = bVar2;
    z80_candidate_8(4,CONCAT11(bVar2,bVar2),uVar7);
    z80_candidate_8(CONCAT11((char)((ushort)uVar7 >> 8) + '\x01',bVar2));
  }
  else {
    cVar3 = '\x05';
    bRamf8cb = bVar2;
    z80_candidate_8(4);
    z80_candidate_8(CONCAT11(cVar3 + -1,bVar6));
  }
  cVar3 = DAT_ram_f8c1;
  if (((DAT_ram_f8c1 != '\0') && (DAT_ram_f811 != -1)) &&
     (cVar3 = DAT_ram_f811, DAT_ram_f811 != '\0')) {
    bVar2 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar6 = *(byte *)CONCAT11(6,DAT_ram_f8c1);
    cVar3 = -CARRY1(bVar2,bVar6);
    if (CARRY1(bVar2,bVar6)) {
      cVar3 = *(char *)CONCAT11(7,bVar2 + bVar6);
    }
  }
  cVar5 = *(char *)(param_2 + 0x12);
  cVar4 = cVar3;
  if (((cVar3 != '\0') && (cVar5 != -1)) && (cVar4 = cVar5, cVar5 != '\0')) {
    bVar2 = *(byte *)CONCAT11(6,cVar5);
    bVar6 = *(byte *)CONCAT11(6,cVar3);
    cVar4 = -CARRY1(bVar2,bVar6);
    if (CARRY1(bVar2,bVar6)) {
      cVar4 = *(char *)CONCAT11(7,bVar2 + bVar6);
    }
  }
  cVar3 = -1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    cVar3 = DAT_ram_f805;
  }
  cVar5 = cVar3;
  if (((cVar3 != '\0') && (cVar4 != -1)) && (cVar5 = cVar4, cVar4 != '\0')) {
    bVar2 = *(byte *)CONCAT11(6,cVar4);
    bVar6 = *(byte *)CONCAT11(6,cVar3);
    cVar5 = -CARRY1(bVar2,bVar6);
    if (CARRY1(bVar2,bVar6)) {
      cVar5 = *(char *)CONCAT11(7,bVar2 + bVar6);
    }
  }
  bVar1 = cVar5 != cRamf8c6;
  cRamf8c6 = cVar5;
  if (bVar1) {
    z80_candidate_8(4,CONCAT11(10,*(undefined1 *)CONCAT11(10,cVar5)));
  }
  bVar6 = 1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar6 = DAT_ram_f809;
  }
  DAT_ram_f8bc = (DAT_ram_f8bc + bVar6) - CARRY1(DAT_ram_f8bc,bVar6);
  if (DAT_ram_f8bb == '\0') {
    if (((*(byte *)(param_2 + 9) & 0x10) != 0) || ((bRamf8b9 & 0x10) == 0)) goto LAB_ram_2bd2;
    bRamf8b9 = bRamf8b9 & 0xef;
  }
  else {
    DAT_ram_f8bb = DAT_ram_f8bb - bVar6;
    if (DAT_ram_f8bb != '\0') goto LAB_ram_2bd2;
    if ((*(byte *)(param_2 + 9) & 0x10) != 0) {
      *(byte *)(param_3 + 3) = *(byte *)(param_3 + 3) | 0x10;
      goto LAB_ram_2bd2;
    }
  }
  z80_candidate_21ca(0xf8cc);
LAB_ram_2bd2:
  cVar3 = cRamf904;
  cRamf904 = DAT_ram_f903;
  pbVar8 = &DAT_ram_f905;
  if (cVar3 != DAT_ram_f903) {
    z80_candidate_8(4);
  }
  bVar2 = *pbVar8;
  bVar6 = pbVar8[1];
  pbVar8[1] = bVar2;
  if (bVar6 == bVar2) {
    return;
  }
  z80_candidate_8(4,CONCAT11(7,(~bVar2 & 7) * '\b' | bVar2));
  return;
}
