/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x40b4; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_40b4(byte param_1)

{
  bool bVar1;
  byte bVar2;
  char cVar3;
  byte bVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  short sVar8;
  ushort uVar9;
  undefined1 uVar11;
  undefined2 uVar10;
  short sVar12;
  ushort uVar13;
  ushort uVar14;
  undefined *puVar15;
  short sVar16;

  puVar15 = &DAT_ram_fb9f;
  sVar16 = _DAT_ram_fb9f;
  if ((*(byte *)(_DAT_ram_fb9f + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  if (DAT_ram_fbbe == '\0') goto LAB_ram_42d5;
  if (DAT_ram_fbbe == '\x04') {
    sVar12 = _DAT_ram_fbb6;
    if ((char)((ushort)_DAT_ram_fbb8 >> 8) == '\0' && (char)_DAT_ram_fbb8 == '\0') {
LAB_ram_4101:
      if ((char)((ushort)_DAT_ram_fbba >> 8) == '\0' && (char)_DAT_ram_fbba == '\0') {
LAB_ram_413b:
        if ((char)((ushort)_DAT_ram_fbbc >> 8) == '\0' && (char)_DAT_ram_fbbc == '\0')
        goto LAB_ram_4193;
        DAT_ram_fbbe = '\x01';
        sVar8 = _DAT_ram_fbbc;
      }
      else {
        DAT_ram_fbbe = '\x02';
        sVar8 = _DAT_ram_fbba + -1;
      }
    }
    else {
      sVar8 = _DAT_ram_fbb8 + -1;
      DAT_ram_fbbe = '\x03';
    }
    z80_candidate_8(4,sVar12,0x1001);
    z80_candidate_8(0x1100);
    z80_candidate_8(0x1b00);
    DAT_ram_fbaf = '\0';
    DAT_ram_fbc1 = 0;
    uVar10 = CONCAT11(0x12,(char)sVar12);
    z80_candidate_8(uVar10);
    uVar10 = CONCAT11((char)((ushort)uVar10 >> 8) + '\x01',(char)((ushort)sVar12 >> 8));
    z80_candidate_8(uVar10);
    sVar12 = sVar12 + sVar8;
    uVar10 = CONCAT11((char)((ushort)uVar10 >> 8) + '\x01',(char)sVar12);
    param_1 = 4;
    z80_candidate_8(uVar10);
    z80_candidate_8(CONCAT11((char)((ushort)uVar10 >> 8) + '\x01',(char)((ushort)sVar12 >> 8)));
    z80_candidate_8(0x1c80);
    DAT_ram_f80a = DAT_ram_f80a & 0x7f;
    z80_candidate_8(0x1c00);
    uVar11 = 0x10;
    z80_candidate_8(0x1000);
    uVar10 = CONCAT11(uVar11,0x80);
    if (DAT_ram_fbbe == '\x02') {
      uVar10 = CONCAT11(uVar11,0x90);
    }
    z80_candidate_8(uVar10);
  }
  else if ((DAT_ram_f80a & 0x80) != 0) {
    if (DAT_ram_fbbe == '\x01') {
LAB_ram_4193:
      z80_candidate_8(4,0x1b00);
      DAT_ram_fbbe = '\0';
      DAT_ram_fba4 = '\0';
      goto LAB_ram_42d5;
    }
    if (DAT_ram_fbbe != '\x02') {
      sVar12 = _DAT_ram_fbb6 + _DAT_ram_fbb8;
      goto LAB_ram_4101;
    }
    if (DAT_ram_fbbf != -1) {
      if (DAT_ram_fbbf == '\0') {
        sVar12 = _DAT_ram_fbb6 + _DAT_ram_fbb8 + _DAT_ram_fbba;
        goto LAB_ram_413b;
      }
      DAT_ram_fbbf = DAT_ram_fbbf + -1;
      param_1 = 4;
      uVar9 = 0x1c80;
      z80_candidate_8(0x1c80);
      z80_candidate_8(uVar9 & 0xff00);
    }
  }
  bVar2 = DAT_ram_fbc0;
  if (DAT_ram_fbc0 == 0) {
    bVar2 = *(byte *)(sVar16 + 0xf);
  }
  bVar2 = (byte)(bVar2 >> 1 | bVar2 << 7) >> 1 | (bVar2 >> 1) << 7;
  if (bVar2 != DAT_ram_fbc1) {
    param_1 = 4;
    DAT_ram_fbc1 = bVar2;
    z80_candidate_8(CONCAT11(0x11,bVar2));
  }
  _DAT_ram_f80f = *(short *)(sVar16 + 0x10) + _DAT_ram_fba8;
  bVar2 = z80_candidate_22cf(0xfbde);
  _DAT_ram_f80f = ((ushort)bVar2 * 2 - (ushort)param_1) + _DAT_ram_f80f;
  sVar12 = -0x414;
  z80_candidate_243f();
  uVar9 = _DAT_ram_fbb3;
  sVar12 = sVar12 + _DAT_ram_f80f;
  if (_DAT_ram_fbad != sVar12) {
    uVar13 = CONCAT11(9,(char)sVar12);
    cVar6 = (char)((ushort)sVar12 >> 8);
    _DAT_ram_fbad = sVar12;
    z80_candidate_1f6d();
    uVar14 = uVar13 + uVar9;
    if (CARRY2(uVar13,uVar9)) {
      uVar14 = CONCAT11((byte)((uVar14 >> 1) >> 8) | CARRY2(uVar13,uVar9) << 7,(char)(uVar14 >> 1));
      cVar6 = cVar6 + '\x01';
    }
    if (cVar6 != '\0') {
      cVar5 = cVar6;
      if (cVar6 < '\0') {
        do {
          bVar2 = (byte)(uVar14 >> 8);
          uVar14 = CONCAT11(bVar2 >> 1,(byte)uVar14 >> 1 | bVar2 << 7);
          cVar5 = cVar5 + '\x01';
        } while (cVar5 != '\0');
      }
      else {
        do {
          bVar1 = CARRY2(uVar14,uVar14);
          uVar14 = uVar14 * 2;
          if (bVar1) {
            uVar14 = -(ushort)bVar1;
          }
          cVar5 = cVar5 + -1;
        } while (cVar5 != '\0');
      }
    }
    uVar10 = CONCAT11(0x19,(char)uVar14);
    sVar12 = CONCAT11(cVar6,4);
    z80_candidate_8(uVar10);
    z80_candidate_8(CONCAT11((char)((ushort)uVar10 >> 8) + '\x01',(char)(uVar14 >> 8)));
  }
  cVar7 = (char)sVar12;
  DAT_ram_f811 = z80_candidate_21de(0xfbc2);
  cVar3 = z80_candidate_22cf(0xfbd0);
  cVar6 = (cVar3 - cVar7) + -1;
  cVar5 = DAT_ram_f811;
  if (((DAT_ram_f811 != '\0') && ((char)(cVar3 - cVar7) != '\0')) && (cVar5 = cVar6, cVar6 != '\0'))
  {
    bVar4 = *(byte *)CONCAT11(6,cVar6);
    bVar2 = *(byte *)CONCAT11(6,DAT_ram_f811);
    cVar5 = -CARRY1(bVar4,bVar2);
    if (CARRY1(bVar4,bVar2)) {
      cVar5 = *(char *)CONCAT11(7,bVar4 + bVar2);
    }
  }
  cVar6 = DAT_ram_fbaa;
  if (((DAT_ram_fbaa != '\0') && (cVar5 != -1)) && (cVar6 = cVar5, cVar5 != '\0')) {
    bVar4 = *(byte *)CONCAT11(6,cVar5);
    bVar2 = *(byte *)CONCAT11(6,DAT_ram_fbaa);
    cVar6 = -CARRY1(bVar4,bVar2);
    if (CARRY1(bVar4,bVar2)) {
      cVar6 = *(char *)CONCAT11(7,bVar4 + bVar2);
    }
  }
  cVar5 = *(char *)(sVar16 + 0x12);
  cVar3 = cVar6;
  if (((cVar6 != '\0') && (cVar5 != -1)) && (cVar3 = cVar5, cVar5 != '\0')) {
    bVar4 = *(byte *)CONCAT11(6,cVar5);
    bVar2 = *(byte *)CONCAT11(6,cVar6);
    cVar3 = -CARRY1(bVar4,bVar2);
    if (CARRY1(bVar4,bVar2)) {
      cVar3 = *(char *)CONCAT11(7,bVar4 + bVar2);
    }
  }
  cVar6 = -1;
  if ((*(byte *)(sVar16 + 9) & 2) != 0) {
    cVar6 = DAT_ram_f805;
  }
  cVar5 = cVar6;
  if (((cVar6 != '\0') && (cVar3 != -1)) && (cVar5 = cVar3, cVar3 != '\0')) {
    bVar4 = *(byte *)CONCAT11(6,cVar3);
    bVar2 = *(byte *)CONCAT11(6,cVar6);
    cVar5 = -CARRY1(bVar4,bVar2);
    if (CARRY1(bVar4,bVar2)) {
      cVar5 = *(char *)CONCAT11(7,bVar4 + bVar2);
    }
  }
  bVar1 = cVar5 != DAT_ram_fbaf;
  DAT_ram_fbaf = cVar5;
  if (bVar1) {
    z80_candidate_8(4,CONCAT11(0x1b,cVar5 * '\x02' + (0x7f < (byte)(cVar5 * '\x02'))));
  }
LAB_ram_42d5:
  bVar2 = 1;
  if ((*(byte *)(sVar16 + 9) & 2) != 0) {
    bVar2 = DAT_ram_f809;
  }
  bRamfba5 = (bRamfba5 + bVar2) - CARRY1(bRamfba5,bVar2);
  if (DAT_ram_fba4 == '\0') {
    if ((*(byte *)(sVar16 + 9) & 0x10) != 0) {
      return;
    }
    if ((bRamfba2 & 0x10) == 0) {
      return;
    }
    bRamfba2 = bRamfba2 & 0xef;
    cVar6 = DAT_ram_fba4;
  }
  else {
    DAT_ram_fba4 = DAT_ram_fba4 - bVar2;
    if (DAT_ram_fba4 != '\0') {
      return;
    }
    cVar6 = '\0';
    if ((*(byte *)(sVar16 + 9) & 0x10) != 0) {
      puVar15[3] = puVar15[3] | 0x10;
      return;
    }
  }
  DAT_ram_fba4 = cVar6;
  z80_candidate_21ca(0xfbc2);
  return;
}
