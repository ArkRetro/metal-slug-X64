/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x28d2; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_28d2(undefined2 param_1,short param_2,short param_3)

{
  bool bVar1;
  undefined2 in_AF;
  byte bVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  byte bVar6;
  byte bVar7;
  undefined2 uVar8;
  undefined1 uVar10;
  ushort uVar9;
  undefined2 uVar11;
  short sVar12;
  ushort uVar13;
  byte *pbVar14;
  undefined *puVar15;
  short sVar16;

  uVar10 = (undefined1)((ushort)in_AF >> 8);
  bVar6 = (byte)param_1;
  if (bRamf87e < bVar6) {
    uVar11 = CONCAT11(2,uVar10);
    uVar8 = CONCAT11(uVar10,4);
    bRamf87e = bVar6;
    z80_candidate_8(uVar8,param_1,uVar11);
    uVar10 = (undefined1)((ushort)uVar8 >> 8);
    z80_candidate_8(CONCAT11((char)((ushort)uVar11 >> 8) + '\x01',(char)param_1));
  }
  else {
    cVar3 = '\x03';
    uVar8 = CONCAT11(uVar10,4);
    bRamf87e = bVar6;
    z80_candidate_8(uVar8);
    uVar10 = (undefined1)((ushort)uVar8 >> 8);
    z80_candidate_8(CONCAT11(cVar3 + -1,uVar10));
  }
  cVar3 = DAT_ram_f874;
  if (((DAT_ram_f874 != '\0') && (DAT_ram_f811 != -1)) &&
     (cVar3 = DAT_ram_f811, DAT_ram_f811 != '\0')) {
    bVar2 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar6 = *(byte *)CONCAT11(6,DAT_ram_f874);
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
  bVar1 = cVar5 != cRamf879;
  cRamf879 = cVar5;
  if (bVar1) {
    uVar8 = CONCAT11(uVar10,4);
    z80_candidate_8(uVar8,CONCAT11(9,*(undefined1 *)CONCAT11(10,cVar5)));
    uVar10 = (undefined1)((ushort)uVar8 >> 8);
  }
  bVar6 = 1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar6 = DAT_ram_f809;
  }
  uVar9 = CONCAT11(uVar10,bVar6);
  DAT_ram_f86f = (DAT_ram_f86f + bVar6) - CARRY1(DAT_ram_f86f,bVar6);
  if (DAT_ram_f86e == '\0') {
    if (((*(byte *)(param_2 + 9) & 0x10) == 0) && ((bRamf86c & 0x10) != 0)) {
      bRamf86c = bRamf86c & 0xef;
LAB_ram_299e:
      z80_candidate_21ca(0xf87f);
    }
  }
  else {
    DAT_ram_f86e = DAT_ram_f86e - bVar6;
    if (DAT_ram_f86e == '\0') {
      if ((*(byte *)(param_2 + 9) & 0x10) == 0) goto LAB_ram_299e;
      *(byte *)(param_3 + 3) = *(byte *)(param_3 + 3) | 0x10;
    }
  }
  puVar15 = &DAT_ram_f8b6;
  sVar16 = _DAT_ram_f8b6;
  if ((*(byte *)(_DAT_ram_f8b6 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  DAT_ram_f811 = z80_candidate_21de(0xf8cc);
  if (DAT_ram_f811 == '\0') goto LAB_ram_2b20;
  cVar3 = z80_candidate_22cf(0xf8da);
  cVar3 = cVar3 - (char)uVar9;
  cVar5 = cVar3 + -1;
  cVar4 = DAT_ram_f811;
  if (((DAT_ram_f811 != '\0') && (cVar3 != '\0')) && (cVar4 = cVar5, cVar5 != '\0')) {
    bVar2 = *(byte *)CONCAT11(6,cVar5);
    bVar6 = *(byte *)CONCAT11(6,DAT_ram_f811);
    cVar4 = -CARRY1(bVar2,bVar6);
    if (CARRY1(bVar2,bVar6)) {
      cVar4 = *(char *)CONCAT11(7,bVar2 + bVar6);
    }
  }
  DAT_ram_f811 = cVar4;
  if ((cVar4 == '\0') || (DAT_ram_f8c1 == '\0')) goto LAB_ram_2b20;
  if (DAT_ram_f8c9 != '\0') {
    bVar6 = z80_candidate_22cf(0xf8e8);
    sVar12 = (ushort)bVar6 * 2 - (uVar9 & 0xff);
    cVar3 = (char)((ushort)sVar12 >> 8);
    bVar6 = (byte)((byte)((byte)sVar12 >> 1 | cVar3 << 7) >> 1 | (cVar3 >> 1) << 7) >> 1 |
            (cVar3 >> 2) << 7;
    uVar9 = (ushort)bVar6 << 8;
    DAT_ram_f903 = (bRamf8be & 0x1f) + bVar6;
    if ((char)DAT_ram_f903 < '\0') {
      DAT_ram_f903 = 0;
    }
    if (0x1f < DAT_ram_f903) {
      DAT_ram_f903 = 0x1f;
    }
    goto LAB_ram_2b20;
  }
  _DAT_ram_f80f = _DAT_ram_f8bf + _DAT_ram_f8c7 + *(short *)(sVar16 + 0x10);
  bVar6 = z80_candidate_22cf(0xf8e8);
  uVar9 = uVar9 & 0xff;
  _DAT_ram_f80f = ((ushort)bVar6 * 2 - uVar9) + _DAT_ram_f80f;
  sVar12 = -0x70a;
  z80_candidate_243f();
  sVar12 = sVar12 + _DAT_ram_f80f;
  if (_DAT_ram_f8c4 == sVar12) goto LAB_ram_2b20;
  bVar6 = (byte)((ushort)sVar12 >> 8);
  if (0xf < bVar6) {
    bVar6 = 0xf;
  }
  bVar7 = *(byte *)CONCAT11(2,(char)sVar12);
  bVar2 = *(byte *)CONCAT11(3,(char)sVar12);
  uVar13 = (ushort)bVar2;
                    /* WARNING: Switch is manually overridden */
  switch(bVar6 << 2) {
  case 0:
    goto z80_candidate_2b00;
  case 4:
    goto z80_candidate_2afd;
  case 8:
    goto z80_candidate_2afa;
  case 0xc:
    goto z80_candidate_2af7;
  case 0x10:
    break;
  case 0x14:
    bVar7 = bVar7 >> 1 | bVar2 << 7;
    bVar2 = bVar2 >> 1;
    break;
  case 0x18:
    uVar13 = uVar13 * 2 + (ushort)CARRY2(uVar13,uVar13);
    goto LAB_ram_2ae9;
  case 0x1c:
LAB_ram_2ae9:
    uVar13 = uVar13 * 2 + (ushort)CARRY2(uVar13,uVar13);
LAB_ram_2aec:
    bVar7 = (byte)uVar13;
    uVar10 = (undefined1)(uVar13 >> 8);
    uVar13 = CONCAT11(uVar10,uVar10);
    goto z80_candidate_2b00;
  case 0x20:
    goto LAB_ram_2aec;
  case 0x24:
    goto z80_candidate_2adf;
  case 0x28:
    goto z80_candidate_2add;
  case 0x2c:
    goto z80_candidate_2adb;
  case 0x30:
    bVar2 = bVar2 >> 1;
z80_candidate_2adb:
    bVar2 = bVar2 >> 1;
z80_candidate_2add:
    uVar13 = (ushort)(bVar2 >> 1);
z80_candidate_2adf:
    bVar7 = (byte)(uVar13 >> 1);
    uVar13 = 0;
    goto z80_candidate_2b00;
  case 0x34:
    uVar13 = uVar13 * 2;
    goto LAB_ram_2ad1;
  case 0x38:
LAB_ram_2ad1:
    uVar13 = uVar13 * 2;
LAB_ram_2ad2:
    bVar7 = (byte)(uVar13 * 2 >> 8);
    uVar13 = (ushort)bVar7 << 8;
    goto z80_candidate_2b00;
  case 0x3c:
    goto LAB_ram_2ad2;
  }
  bVar7 = bVar7 >> 1 | bVar2 << 7;
  bVar2 = bVar2 >> 1;
z80_candidate_2af7:
  bVar7 = bVar7 >> 1 | bVar2 << 7;
  bVar2 = bVar2 >> 1;
z80_candidate_2afa:
  bVar7 = bVar7 >> 1 | bVar2 << 7;
  bVar2 = bVar2 >> 1;
z80_candidate_2afd:
  uVar13 = (ushort)(bVar2 >> 1);
  bVar7 = bVar7 >> 1 | bVar2 << 7;
z80_candidate_2b00:
  bVar6 = (byte)uVar13;
  _DAT_ram_f8c4 = sVar12;
  if (bRamf8cb < bVar6) {
    uVar8 = CONCAT11(4,bVar7);
    uVar9 = CONCAT11(bVar7,4);
    bRamf8cb = bVar6;
    z80_candidate_8(uVar9,uVar13,uVar8);
    z80_candidate_8(CONCAT11((char)((ushort)uVar8 >> 8) + '\x01',(char)uVar13));
  }
  else {
    cVar3 = '\x05';
    uVar9 = CONCAT11(bVar7,4);
    bRamf8cb = bVar6;
    z80_candidate_8(uVar9);
    z80_candidate_8(CONCAT11(cVar3 + -1,(char)(uVar9 >> 8)));
  }
LAB_ram_2b20:
  uVar10 = (undefined1)(uVar9 >> 8);
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
  cVar5 = *(char *)(sVar16 + 0x12);
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
  if ((*(byte *)(sVar16 + 9) & 2) != 0) {
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
    uVar8 = CONCAT11(uVar10,4);
    z80_candidate_8(uVar8,CONCAT11(10,*(undefined1 *)CONCAT11(10,cVar5)));
    uVar10 = (undefined1)((ushort)uVar8 >> 8);
  }
  bVar6 = 1;
  if ((*(byte *)(sVar16 + 9) & 2) != 0) {
    bVar6 = DAT_ram_f809;
  }
  DAT_ram_f8bc = (DAT_ram_f8bc + bVar6) - CARRY1(DAT_ram_f8bc,bVar6);
  if (DAT_ram_f8bb == '\0') {
    if (((*(byte *)(sVar16 + 9) & 0x10) != 0) || ((bRamf8b9 & 0x10) == 0)) goto LAB_ram_2bd2;
    bRamf8b9 = bRamf8b9 & 0xef;
  }
  else {
    DAT_ram_f8bb = DAT_ram_f8bb - bVar6;
    if (DAT_ram_f8bb != '\0') goto LAB_ram_2bd2;
    if ((*(byte *)(sVar16 + 9) & 0x10) != 0) {
      puVar15[3] = puVar15[3] | 0x10;
      goto LAB_ram_2bd2;
    }
  }
  z80_candidate_21ca(0xf8cc);
LAB_ram_2bd2:
  bVar6 = bRamf904;
  bRamf904 = DAT_ram_f903;
  pbVar14 = &DAT_ram_f905;
  if (bVar6 != DAT_ram_f903) {
    uVar8 = CONCAT11(uVar10,4);
    z80_candidate_8(uVar8);
    uVar10 = (undefined1)((ushort)uVar8 >> 8);
  }
  bVar2 = *pbVar14;
  bVar6 = pbVar14[1];
  pbVar14[1] = bVar2;
  if (bVar6 != bVar2) {
    z80_candidate_8(CONCAT11(uVar10,4),CONCAT11(7,(~bVar2 & 7) * '\b' | bVar2));
    return;
  }
  return;
}
