/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4a2a; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_4a2a(undefined2 param_1,byte *param_2,short param_3,short param_4)

{
  undefined1 uVar1;
  byte in_F;
  byte bVar2;
  byte bVar3;
  char cVar4;
  undefined1 uVar5;
  undefined2 uVar6;
  byte *pbVar7;
  undefined2 uVar8;
  byte *pbVar9;
  byte *pbVar10;
  short sVar11;
  undefined1 *puVar12;
  byte *pbVar13;
  short sVar14;

  z80_candidate_4b84();
  DAT_ram_fbff = (undefined1)((ushort)param_1 >> 8);
  in_F = in_F & 0xad;
  bVar2 = in_F;
  _DAT_ram_fbfd = param_2;
  if ((*(byte *)(param_3 + 9) & 1) != 0) {
    bVar3 = z80_candidate_4b09();
    uVar5 = (undefined1)param_1;
    if ((bool)(in_F & 1)) {
      bVar2 = *_DAT_ram_fbfd < *(byte *)(param_4 + 2);
      if (*_DAT_ram_fbfd == *(byte *)(param_4 + 2)) goto LAB_ram_4a5b;
    }
    else {
      bVar2 = in_F & 0xad;
      if (((*(byte *)(param_3 + 9) & 2) == 0) &&
         (bVar2 = in_F & 0xad, (*(byte *)(param_3 + 9) & 0x20) != 0)) {
        return bVar3;
      }
    }
  }
  uVar5 = (undefined1)param_1;
  bVar3 = z80_candidate_4422();
  if ((bVar2 & 1) == 0) {
    return bVar3;
  }
LAB_ram_4a5b:
  *(byte *)(param_3 + 9) = *(byte *)(param_3 + 9) | 0x20;
  *(undefined1 *)(param_4 + 4) = *(undefined1 *)(param_3 + 2);
  *(undefined1 *)(param_4 + 8) = DAT_ram_fbf9;
  *(undefined1 *)(param_4 + 0xb) = DAT_ram_fbfc;
  *(undefined1 *)(param_4 + 7) = DAT_ram_fbfa;
  *(undefined1 *)(param_4 + 6) = 0;
  cVar4 = *(char *)(param_4 + 5);
  *(char *)(param_4 + 5) = DAT_ram_fbfb + (DAT_ram_fbfb == '\0');
  *(undefined1 *)(param_4 + 3) = *(undefined1 *)(param_3 + 9);
  if (cVar4 == '\0') {
    uVar1 = *(undefined1 *)(param_3 + 0xb);
    if (*(short *)(param_4 + 0xc) == *(short *)(param_3 + 10)) {
                    /* WARNING: Switch is manually overridden */
      switch(*(undefined1 *)(param_4 + 2)) {
      case 0:
        z80_candidate_21b9(param_4 + 0x16);
        z80_candidate_22a5(param_4 + 0x24);
        z80_candidate_22a5(param_4 + 0x32);
        bVar2 = z80_candidate_2412(CONCAT11(cVar4,*(undefined1 *)(param_3 + 0xe)),param_4 + 0x40,
                                   param_4);
        return bVar2;
      case 1:
        bVar2 = *(byte *)(param_4 + 0x12);
        uVar5 = 0;
        if (bVar2 != *(byte *)(param_4 + 0x3e)) {
          *(byte *)(param_4 + 0x3e) = bVar2;
          uVar5 = 0;
          z80_candidate_8(4);
        }
        break;
      case 2:
        *(undefined1 *)(param_4 + 0x1a) = 2;
        bVar2 = z80_candidate_21b9(param_4 + 0x1b);
        return bVar2;
      case 3:
        DAT_ram_fbbe = 4;
        pbVar13 = _DAT_ram_fbb1;
        cVar4 = DAT_ram_fbb0;
        if (DAT_ram_ff13 != DAT_ram_fbb0) {
          DAT_ram_ff13 = DAT_ram_fbb0;
          cVar4 = (((DAT_ram_fbb0 + '\x01') * '\x02' + '\x01') * '\x02' + '\x01') * '\x02';
        }
        for (; (DAT_ram_fba7 != *pbVar13 && (*pbVar13 <= DAT_ram_fba7)); pbVar13 = pbVar13 + 6) {
        }
        DAT_ram_fbb5 = pbVar13[1];
        pbVar13 = pbVar13 + 2;
        z80_candidate_57be(0x4aeb);
        DAT_ram_fbb3 = *pbVar13;
        DAT_ram_fbb4 = pbVar13[1];
        DAT_ram_fbc0 = pbVar13[2];
        DAT_ram_fbbf = pbVar13[3];
        _DAT_ram_fbb6 = *(undefined2 *)(pbVar13 + 4);
        _DAT_ram_fbb8 = *(undefined2 *)(pbVar13 + 6);
        _DAT_ram_fbba = *(undefined2 *)(pbVar13 + 8);
        _DAT_ram_fbbc = *(undefined2 *)(pbVar13 + 10);
        z80_candidate_21b9(0xfbc2);
        z80_candidate_22a5(0xfbd0);
        z80_candidate_22a5(0xfbde);
        bVar2 = z80_candidate_2412(CONCAT11(cVar4,*(undefined1 *)(param_3 + 0xe)),param_4 + 0x4d,
                                   param_4);
        return bVar2;
      }
      *(byte *)(param_4 + 0x3d) = *(byte *)(param_4 + 0x13) & 0xf0 | bVar2;
      if ((*(byte *)(param_4 + 0x39) & 0x40) != 0) {
        bVar2 = DAT_ram_faa7 & 7;
        uVar8 = CONCAT11(uVar5,4);
        DAT_ram_faa7 = DAT_ram_faa7 & 0xf | 8;
        z80_candidate_8(uVar8,CONCAT11(0x22,bVar2));
        uVar5 = (undefined1)((ushort)uVar8 >> 8);
        DAT_ram_faa8 = 0xff;
      }
      z80_candidate_22a5(param_4 + 0x3f);
      z80_candidate_22a5(param_4 + 0x4d);
      bVar2 = z80_candidate_2412(CONCAT11(uVar5,*(undefined1 *)(param_3 + 0xe)),param_4 + 0x5b,
                                 param_4);
      return bVar2;
    }
    *(undefined1 *)(param_4 + 0xc) = *(undefined1 *)(param_3 + 10);
    *(undefined1 *)(param_4 + 0xd) = uVar1;
    pbVar13 = _DAT_ram_fbfd;
    uVar8 = CONCAT11(DAT_ram_fbff,uVar5);
                    /* WARNING: Switch is manually overridden */
    switch(*(undefined1 *)(param_4 + 2)) {
    case 0:
      bVar2 = _DAT_ram_fbfd[1];
      *(byte *)(param_4 + 0x11) = bVar2;
      *(char *)(param_4 + 0x12) = -CARRY1(bVar2,bVar2);
      bVar2 = pbVar13[2];
      *(byte *)(param_4 + 0x13) = bVar2;
      bVar2 = -(bVar2 == 0) ^ 0x38;
      DAT_ram_f905 = (bVar2 ^ DAT_ram_f905 | *(byte *)(param_4 + 0x14)) ^ bVar2;
      pbVar13 = pbVar13 + 3;
      uVar6 = 0x16;
      sVar14 = param_4 + 0x16;
      z80_candidate_214c(uVar8,pbVar13,sVar14,param_4,0x4acf);
      sVar14 = sVar14 + 0xe;
      z80_candidate_2259(uVar6,pbVar13,sVar14,param_3);
      pbVar13 = pbVar13 + 6;
      sVar14 = sVar14 + 0xe;
      z80_candidate_2259(pbVar13,sVar14);
      bVar2 = z80_candidate_23df(pbVar13 + 6,sVar14 + 0xe);
      return bVar2;
    case 1:
      pbVar9 = _DAT_ram_fbfd + 6;
      pbVar10 = (byte *)(param_4 + 0x14);
      cVar4 = '\x04';
      break;
    case 2:
      *(undefined1 *)(param_4 + 0x11) = DAT_ram_fbff;
      pbVar13 = pbVar13 + 1;
      bVar2 = z80_candidate_214c(param_4 + 0x1b,0x4acf);
      *(byte **)(param_4 + 0x12) = pbVar13 + 6;
      return bVar2;
    case 3:
      *(undefined1 *)(param_4 + 0x11) = DAT_ram_fbff;
      pbVar13 = pbVar13 + 1;
      uVar8 = 0x23;
      sVar14 = param_4 + 0x23;
      z80_candidate_214c(sVar14,0x4acf);
      sVar14 = sVar14 + 0xe;
      z80_candidate_2259(uVar8,sVar14,param_3);
      pbVar13 = pbVar13 + 6;
      sVar14 = sVar14 + 0xe;
      z80_candidate_2259(sVar14);
      pbVar13 = pbVar13 + 6;
      bVar2 = z80_candidate_23df(sVar14 + 0xe);
      *(byte **)(param_4 + 0x12) = pbVar13 + 4;
      return bVar2;
    }
    do {
      sVar14 = 7;
      do {
        pbVar7 = pbVar10;
        *pbVar7 = *pbVar9;
        pbVar9 = pbVar9 + 1;
        sVar14 = sVar14 + -1;
        pbVar10 = pbVar7 + 1;
      } while (sVar14 != 0);
      pbVar7[1] = 0x7f;
      pbVar10 = pbVar7 + 3;
      cVar4 = cVar4 + -1;
    } while (cVar4 != '\0');
    *(undefined1 *)(param_4 + 0x1c) = *(undefined1 *)(param_4 + 0x15);
    *(undefined1 *)(param_4 + 0x25) = *(undefined1 *)(param_4 + 0x1e);
    *(undefined1 *)(param_4 + 0x2e) = *(undefined1 *)(param_4 + 0x27);
    *(undefined1 *)(param_4 + 0x37) = *(undefined1 *)(param_4 + 0x30);
    *(byte *)(param_4 + 0x13) = pbVar13[1] & 0xf0;
    *(byte *)(param_4 + 0x39) = pbVar13[2] | 0x80;
    if ((pbVar13[3] & 8) == 0) {
      *(byte *)(param_4 + 0x38) = pbVar13[4] & 0xc0;
    }
    else {
      DAT_ram_faa7 = pbVar13[3];
      *(byte *)(param_4 + 0x38) = pbVar13[4];
    }
    *(undefined1 *)(param_4 + 0x3c) = 0;
    *(byte *)(param_4 + 0x11) = pbVar13[5];
    pbVar13 = pbVar13 + 0x22;
    sVar14 = param_4 + 0x3f;
    z80_candidate_2259(pbVar13,sVar14,param_4,param_3);
    pbVar13 = pbVar13 + 6;
    sVar14 = sVar14 + 0xe;
    z80_candidate_2259(pbVar13,sVar14);
    bVar2 = z80_candidate_23df(pbVar13 + 6,sVar14 + 0xe);
    return bVar2;
  }
  bVar2 = *(char *)(param_4 + 2) * '\x02';
                    /* WARNING: Switch is manually overridden */
  switch(*(char *)(param_4 + 2)) {
  case '\0':
    uVar5 = *(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 8) | 0x80);
    *(undefined1 *)(param_4 + 9) = *(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 8));
    *(undefined1 *)(param_4 + 10) = uVar5;
    return bVar2;
  case '\x02':
    if (DAT_ram_ff13 != *(char *)(param_4 + 0x11)) {
      DAT_ram_ff13 = *(char *)(param_4 + 0x11);
    }
    sVar14 = *(short *)(param_4 + 0x12);
    bVar2 = *(byte *)(sVar14 + 5 + (ushort)(byte)(*(char *)(param_4 + 8) - *(char *)(sVar14 + 4)));
    sVar11 = (ushort)bVar2 * 4 + (ushort)bVar2;
    z80_candidate_57be(0x4b07);
    puVar12 = (undefined1 *)(sVar14 + sVar11);
    *(undefined1 *)(param_4 + 0x15) = *puVar12;
    *(undefined1 *)(param_4 + 0x16) = puVar12[1];
    *(undefined1 *)(param_4 + 0x17) = puVar12[2];
    *(undefined1 *)(param_4 + 0x18) = puVar12[3];
    *(undefined1 *)(param_4 + 0x19) = puVar12[4];
    bVar2 = *(byte *)(param_4 + 0x15) & 3;
    if (bVar2 == 0) {
      bVar2 = *(byte *)(param_3 + 0xf);
      *(byte *)(param_4 + 0x15) = bVar2;
    }
    return bVar2;
  case '\x03':
    *(short *)(param_4 + 9) =
         CONCAT11(*(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 8) | 0x80),
                  *(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 8))) -
         CONCAT11(*(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 0x16) | 0x80),
                  *(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 0x16)));
    _DAT_ram_fbad = 0xffff;
    return bVar2;
  }
  uVar5 = *(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 8) | 0x80);
  *(undefined1 *)(param_4 + 9) = *(undefined1 *)CONCAT11(1,*(byte *)(param_4 + 8));
  *(undefined1 *)(param_4 + 10) = uVar5;
  bVar2 = *(byte *)(param_4 + 0x38);
  if ((bVar2 & 0xc0) == 0) {
    bVar2 = (byte)(*(byte *)(param_3 + 0xf) << 7) >> 1 | ((*(byte *)(param_3 + 0xf) & 3) >> 1) << 7
            | bVar2;
    *(byte *)(param_4 + 0x3b) = bVar2;
    return bVar2;
  }
  *(byte *)(param_4 + 0x3b) = bVar2;
  return bVar2 & 0xc0;
}
