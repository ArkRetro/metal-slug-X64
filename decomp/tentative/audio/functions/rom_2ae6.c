/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2ae6; tentative_c. */

void z80_candidate_2ae6(ushort param_1,short param_2,short param_3)

{
  bool bVar1;
  ushort uVar2;
  short sVar3;
  byte bVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  byte bVar8;
  undefined1 uVar9;
  undefined2 uVar10;
  undefined2 uVar11;
  byte *pbVar12;

  uVar2 = param_1 * 2 + (ushort)CARRY2(param_1,param_1);
  sVar3 = uVar2 * 2 + (ushort)CARRY2(uVar2,uVar2);
  uVar9 = (undefined1)sVar3;
  bVar8 = (byte)((ushort)sVar3 >> 8);
  uVar11 = CONCAT11(bVar8,bVar8);
  if (bRamf8cb < bVar8) {
    uVar10 = CONCAT11(4,uVar9);
    bRamf8cb = bVar8;
    z80_candidate_8(4,uVar11,uVar10);
    z80_candidate_8(CONCAT11((char)((ushort)uVar10 >> 8) + '\x01',(char)uVar11));
  }
  else {
    cVar5 = '\x05';
    bRamf8cb = bVar8;
    z80_candidate_8(4);
    z80_candidate_8(CONCAT11(cVar5 + -1,uVar9));
  }
  cVar5 = DAT_ram_f8c1;
  if (((DAT_ram_f8c1 != '\0') && (DAT_ram_f811 != -1)) &&
     (cVar5 = DAT_ram_f811, DAT_ram_f811 != '\0')) {
    bVar4 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar8 = *(byte *)CONCAT11(6,DAT_ram_f8c1);
    cVar5 = -CARRY1(bVar4,bVar8);
    if (CARRY1(bVar4,bVar8)) {
      cVar5 = *(char *)CONCAT11(7,bVar4 + bVar8);
    }
  }
  cVar7 = *(char *)(param_2 + 0x12);
  cVar6 = cVar5;
  if (((cVar5 != '\0') && (cVar7 != -1)) && (cVar6 = cVar7, cVar7 != '\0')) {
    bVar4 = *(byte *)CONCAT11(6,cVar7);
    bVar8 = *(byte *)CONCAT11(6,cVar5);
    cVar6 = -CARRY1(bVar4,bVar8);
    if (CARRY1(bVar4,bVar8)) {
      cVar6 = *(char *)CONCAT11(7,bVar4 + bVar8);
    }
  }
  cVar5 = -1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    cVar5 = DAT_ram_f805;
  }
  cVar7 = cVar5;
  if (((cVar5 != '\0') && (cVar6 != -1)) && (cVar7 = cVar6, cVar6 != '\0')) {
    bVar4 = *(byte *)CONCAT11(6,cVar6);
    bVar8 = *(byte *)CONCAT11(6,cVar5);
    cVar7 = -CARRY1(bVar4,bVar8);
    if (CARRY1(bVar4,bVar8)) {
      cVar7 = *(char *)CONCAT11(7,bVar4 + bVar8);
    }
  }
  bVar1 = cVar7 != cRamf8c6;
  cRamf8c6 = cVar7;
  if (bVar1) {
    z80_candidate_8(4,CONCAT11(10,*(undefined1 *)CONCAT11(10,cVar7)));
  }
  bVar8 = 1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar8 = DAT_ram_f809;
  }
  DAT_ram_f8bc = (DAT_ram_f8bc + bVar8) - CARRY1(DAT_ram_f8bc,bVar8);
  if (DAT_ram_f8bb == '\0') {
    if (((*(byte *)(param_2 + 9) & 0x10) != 0) || ((bRamf8b9 & 0x10) == 0)) goto LAB_ram_2bd2;
    bRamf8b9 = bRamf8b9 & 0xef;
  }
  else {
    DAT_ram_f8bb = DAT_ram_f8bb - bVar8;
    if (DAT_ram_f8bb != '\0') goto LAB_ram_2bd2;
    if ((*(byte *)(param_2 + 9) & 0x10) != 0) {
      *(byte *)(param_3 + 3) = *(byte *)(param_3 + 3) | 0x10;
      goto LAB_ram_2bd2;
    }
  }
  z80_candidate_21ca(0xf8cc);
LAB_ram_2bd2:
  cVar5 = cRamf904;
  cRamf904 = DAT_ram_f903;
  pbVar12 = &DAT_ram_f905;
  if (cVar5 != DAT_ram_f903) {
    z80_candidate_8(4);
  }
  bVar4 = *pbVar12;
  bVar8 = pbVar12[1];
  pbVar12[1] = bVar4;
  if (bVar8 == bVar4) {
    return;
  }
  z80_candidate_8(4,CONCAT11(7,(~bVar4 & 7) * '\b' | bVar4));
  return;
}
