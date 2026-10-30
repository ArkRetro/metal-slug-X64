/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4ccd; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_4ccd(void)

{
  bool bVar1;
  byte bVar2;
  char cVar3;
  byte bVar4;
  char cVar6;
  short sVar5;

  if ((bRamfc09 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfc2c & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfc4f & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfc72 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfc95 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfcb8 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfcdb & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfcfe & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfd21 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfd44 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfd67 & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfd8a & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfdad & 0x40) != 0) {
    z80_candidate_52de();
  }
  if ((bRamfdd0 & 0x40) != 0) {
    z80_candidate_52de();
  }
  cVar6 = (char)(_DAT_ram_f804 >> 8);
  if (DAT_ram_f808 == '\0') {
    if ((cVar6 != -1) &&
       ((bVar1 = CARRY2(_DAT_ram_f804,_DAT_ram_f806), _DAT_ram_f804 = _DAT_ram_f804 + _DAT_ram_f806,
        bVar1 || ((char)(_DAT_ram_f804 >> 8) != -1)))) {
      _DAT_ram_f804 = 0xffff;
    }
  }
  else if (((char)_DAT_ram_f804 != '\0' || cVar6 != '\0') &&
          (bVar1 = _DAT_ram_f804 < _DAT_ram_f806, _DAT_ram_f804 = _DAT_ram_f804 - _DAT_ram_f806,
          bVar1)) {
    _DAT_ram_f804 = 0;
  }
  DAT_ram_f809 = 0;
  cVar6 = DAT_ram_fdeb;
  if (DAT_ram_fdeb != DAT_ram_fdec) {
    if (DAT_ram_fded == '\0') {
      _DAT_ram_fdea = CONCAT11(DAT_ram_fdec,DAT_ram_fdea);
    }
    else {
      bVar4 = (DAT_ram_fdec >> 1) - ((char)DAT_ram_fdeb >> 1);
      if (bVar4 == 0) {
        DAT_ram_fded = '\0';
      }
      else {
        bVar2 = -CARRY1(bVar4,bVar4);
        bVar4 = *(byte *)CONCAT11(6,bVar2 ^ bVar4);
        cVar3 = bVar4 + *(byte *)CONCAT11(6,DAT_ram_fded);
        if (!CARRY1(bVar4,*(byte *)CONCAT11(6,DAT_ram_fded))) {
          cVar3 = '\0';
        }
        _DAT_ram_fdea = _DAT_ram_fdea + CONCAT11(bVar2,*(byte *)CONCAT11(7,cVar3) ^ bVar2) * 0x10;
      }
    }
  }
  if (DAT_ram_fdeb != cVar6) {
    z80_candidate_4f67();
  }
  if (DAT_ram_fdef != DAT_ram_fdf0) {
    if (DAT_ram_fdf1 == '\0') {
      _DAT_ram_fdee = CONCAT11(DAT_ram_fdf0,DAT_ram_fdee);
    }
    else {
      bVar4 = (DAT_ram_fdf0 >> 1) - ((char)DAT_ram_fdef >> 1);
      if (bVar4 == 0) {
        DAT_ram_fdf1 = '\0';
      }
      else {
        bVar2 = -CARRY1(bVar4,bVar4);
        bVar4 = *(byte *)CONCAT11(6,bVar2 ^ bVar4);
        cVar6 = bVar4 + *(byte *)CONCAT11(6,DAT_ram_fdf1);
        if (!CARRY1(bVar4,*(byte *)CONCAT11(6,DAT_ram_fdf1))) {
          cVar6 = '\0';
        }
        _DAT_ram_fdee = _DAT_ram_fdee + CONCAT11(bVar2,*(byte *)CONCAT11(7,cVar6) ^ bVar2) * 0x10;
      }
    }
  }
  bVar4 = DAT_ram_fdeb + DAT_ram_fdef;
  if (SCARRY1(DAT_ram_fdeb,DAT_ram_fdef)) {
    bVar4 = -CARRY1(DAT_ram_fdeb,DAT_ram_fdef) ^ 0x7f;
  }
  sVar5 = (byte)(bVar4 + 0x80) + 1;
  if (DAT_ram_f803 == '\0') {
    return;
  }
  if (DAT_ram_f803 != '\x01') {
    if (DAT_ram_f803 != '\x02') {
      _DAT_ram_fdf4 = _DAT_ram_fdf4 + sVar5;
    }
    _DAT_ram_fdf4 = _DAT_ram_fdf4 + sVar5;
  }
  _DAT_ram_fdf4 = _DAT_ram_fdf4 + sVar5;
  cVar6 = (char)(_DAT_ram_fdf4 >> 8);
  if (cVar6 != '\0') {
    _DAT_ram_fdf4 = CONCAT11(cVar6 + -1,DAT_ram_fdf4) & 0x3ff;
    DAT_ram_f809 = 1;
    bVar4 = bRamfc09 & 0xf7;
    bVar2 = bRamfc09 & 4;
    bRamfc09 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfc00);
    }
    bVar4 = bRamfc2c & 0xf7;
    bVar2 = bRamfc2c & 4;
    bRamfc2c = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfc23);
    }
    bVar4 = bRamfc4f & 0xf7;
    bVar2 = bRamfc4f & 4;
    bRamfc4f = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfc46);
    }
    bVar4 = bRamfc72 & 0xf7;
    bVar2 = bRamfc72 & 4;
    bRamfc72 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfc69);
    }
    bVar4 = bRamfc95 & 0xf7;
    bVar2 = bRamfc95 & 4;
    bRamfc95 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfc8c);
    }
    bVar4 = bRamfcb8 & 0xf7;
    bVar2 = bRamfcb8 & 4;
    bRamfcb8 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfcaf);
    }
    bVar4 = bRamfcdb & 0xf7;
    bVar2 = bRamfcdb & 4;
    bRamfcdb = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfcd2);
    }
    bVar4 = bRamfcfe & 0xf7;
    bVar2 = bRamfcfe & 4;
    bRamfcfe = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfcf5);
    }
    bVar4 = bRamfd21 & 0xf7;
    bVar2 = bRamfd21 & 4;
    bRamfd21 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfd18);
    }
    bVar4 = bRamfd44 & 0xf7;
    bVar2 = bRamfd44 & 4;
    bRamfd44 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfd3b);
    }
    bVar4 = bRamfd67 & 0xf7;
    bVar2 = bRamfd67 & 4;
    bRamfd67 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfd5e);
    }
    bVar4 = bRamfd8a & 0xf7;
    bVar2 = bRamfd8a & 4;
    bRamfd8a = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfd81);
    }
    bVar4 = bRamfdad & 0xf7;
    bVar2 = bRamfdad & 4;
    bRamfdad = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfda4);
    }
    bVar4 = bRamfdd0 & 0xf7;
    bVar2 = bRamfdd0 & 4;
    bRamfdd0 = bVar4;
    if (bVar2 != 0) {
      z80_candidate_531f(0xfdc7);
    }
    return;
  }
  return;
}
