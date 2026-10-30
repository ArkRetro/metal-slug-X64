/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2daf; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_2daf(byte param_1)

{
  byte bVar1;
  char cVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  char cVar6;
  ushort uVar7;
  undefined2 uVar8;
  undefined1 uVar9;
  short sVar10;
  short sVar11;
  char *pcVar12;
  undefined *puVar13;
  short sVar14;

  cVar6 = DAT_ram_faa8;
  DAT_ram_faa8 = DAT_ram_faa7;
  if (cVar6 != DAT_ram_faa7) {
    param_1 = 4;
    z80_candidate_8();
  }
  puVar13 = &DAT_ram_f907;
  sVar14 = _DAT_ram_f907;
  if ((*(byte *)(_DAT_ram_f907 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  if ((char)DAT_ram_f940 < '\0') {
    uRamf943 = 0;
    param_1 = 4;
    z80_candidate_8(0xb500);
    DAT_ram_f917 = 0xff;
  }
  _DAT_ram_f80f =
       CONCAT11(DAT_ram_f918 + (char)((ushort)_DAT_ram_f910 >> 8),(char)_DAT_ram_f910) +
       *(short *)(sVar14 + 0x10);
  bVar1 = z80_candidate_22cf(0xf954);
  uVar7 = (ushort)param_1;
  _DAT_ram_f80f = ((ushort)bVar1 * 2 - uVar7) + _DAT_ram_f80f;
  sVar10 = -0x69e;
  z80_candidate_243f();
  cVar6 = (char)uVar7;
  sVar10 = sVar10 + _DAT_ram_f80f;
  if (_DAT_ram_f915 != sVar10) {
    bVar1 = (byte)((ushort)sVar10 >> 8);
    sVar11 = sVar10;
    if (sVar10 < 0) {
      bVar1 = 0;
      sVar11 = 0;
    }
    sVar11 = CONCAT11(*(undefined1 *)CONCAT11(4,(char)sVar11),
                      *(undefined1 *)CONCAT11(5,(char)sVar11));
    if (7 < bVar1) {
      if (bVar1 == 8) {
        bVar1 = 7;
        sVar11 = sVar11 * 2;
      }
      else {
        sVar11 = -1;
      }
    }
    cVar6 = '\x04';
    _DAT_ram_f915 = sVar10;
    z80_candidate_8(CONCAT11(0xa5,bVar1 << 3 | (byte)((ushort)sVar11 >> 8)));
    z80_candidate_8(CONCAT11(0xa1,(char)sVar11));
  }
  cVar2 = z80_candidate_22cf(0xf946);
  bVar1 = (cVar2 - cVar6) - 1;
  if ((char)DAT_ram_f940 < '\0') {
    bVar1 = 0;
  }
  bVar4 = DAT_ram_f912;
  if (((DAT_ram_f912 != 0) && (bVar1 != 0xff)) && (bVar4 = bVar1, bVar1 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar1);
    bVar5 = *(byte *)CONCAT11(6,DAT_ram_f912);
    bVar4 = -CARRY1(bVar3,bVar5);
    if (CARRY1(bVar3,bVar5)) {
      bVar4 = *(byte *)CONCAT11(7,bVar3 + bVar5);
    }
  }
  bVar5 = *(byte *)(sVar14 + 0x12);
  bVar3 = bVar4;
  if (((bVar4 != 0) && (bVar5 != 0xff)) && (bVar3 = bVar5, bVar5 != 0)) {
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar3 = -CARRY1(bVar5,bVar4);
    if (CARRY1(bVar5,bVar4)) {
      bVar3 = *(byte *)CONCAT11(7,bVar5 + bVar4);
    }
  }
  bVar4 = 0xff;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar4 = DAT_ram_f805;
  }
  bVar5 = bVar4;
  if (((bVar4 != 0) && (bVar3 != 0xff)) && (bVar5 = bVar3, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar5 = -CARRY1(bVar3,bVar4);
    if (CARRY1(bVar3,bVar4)) {
      bVar5 = *(byte *)CONCAT11(7,bVar3 + bVar4);
    }
  }
  if (bVar5 != DAT_ram_f917) {
    bVar1 = *(byte *)CONCAT11(10,bVar5 | 0x80);
    bVar4 = puVar13[0x39] & 7;
    if (3 < bVar4) {
      if (4 < bVar4) {
        if ((6 < bVar4) &&
           (bVar4 = (char)(DAT_ram_f923 + bVar1) >> 7 | DAT_ram_f923 + bVar1, bVar4 != bRamf91c)) {
          DAT_ram_f922 = DAT_ram_f922 | 2;
          bRamf91c = bVar4;
        }
        bVar4 = (char)(DAT_ram_f935 + bVar1) >> 7 | DAT_ram_f935 + bVar1;
        if (bVar4 != bRamf92e) {
          DAT_ram_f934 = DAT_ram_f934 | 2;
          bRamf92e = bVar4;
        }
      }
      bVar4 = (char)(DAT_ram_f92c + bVar1) >> 7 | DAT_ram_f92c + bVar1;
      if (bVar4 != bRamf925) {
        DAT_ram_f92b = DAT_ram_f92b | 2;
        bRamf925 = bVar4;
      }
    }
    bVar4 = (char)(DAT_ram_f93e + bVar1) >> 7 | DAT_ram_f93e + bVar1;
    if (bVar4 != bRamf937) {
      DAT_ram_f93d = DAT_ram_f93d | 2;
      bRamf937 = bVar4;
    }
  }
  uVar8 = CONCAT11(bVar1,4);
  DAT_ram_f917 = bVar5;
  if (DAT_ram_f922 != 0) {
    z80_candidate_2d53(uVar8,0xf91b);
    DAT_ram_f922 = 0;
  }
  uVar9 = (undefined1)((ushort)uVar8 >> 8);
  if (DAT_ram_f92b != 0) {
    z80_candidate_2d53(0xf924);
    DAT_ram_f92b = 0;
  }
  if (DAT_ram_f934 != 0) {
    z80_candidate_2d53(0xf92d);
    DAT_ram_f934 = 0;
  }
  if (DAT_ram_f93d != 0) {
    z80_candidate_2d53(0xf936);
    DAT_ram_f93d = 0;
  }
  DAT_ram_f940 = DAT_ram_f940 & 0x7f;
  sVar10 = -0x6bf;
  if (DAT_ram_f940 != bRamf941) {
    bRamf941 = DAT_ram_f940;
    z80_candidate_8(CONCAT11(0xb1,DAT_ram_f940));
  }
  cVar6 = *(char *)(sVar10 + 1);
  pcVar12 = (char *)(sVar10 + 2);
  if (cVar6 != *pcVar12) {
    *pcVar12 = cVar6;
    uVar8 = CONCAT11(uVar9,4);
    z80_candidate_8(uVar8,CONCAT11(0xb5,cVar6));
    uVar9 = (undefined1)((ushort)uVar8 >> 8);
  }
  cVar6 = pcVar12[1];
  if (cVar6 != pcVar12[2]) {
    pcVar12[2] = cVar6;
    z80_candidate_8(CONCAT11(uVar9,4),CONCAT11(0x28,cVar6));
  }
  bVar1 = 1;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar1 = DAT_ram_f809;
  }
  DAT_ram_f90d = (DAT_ram_f90d + bVar1) - CARRY1(DAT_ram_f90d,bVar1);
  if (DAT_ram_f90c == '\0') {
    if (((*(byte *)(sVar14 + 9) & 0x10) == 0) && ((bRamf90a & 0x10) != 0)) {
      bRamf90a = bRamf90a & 0xef;
LAB_ram_2fe8:
      DAT_ram_f944 = DAT_ram_f919;
    }
  }
  else {
    DAT_ram_f90c = DAT_ram_f90c - bVar1;
    if (DAT_ram_f90c == '\0') {
      if ((*(byte *)(sVar14 + 9) & 0x10) == 0) goto LAB_ram_2fe8;
      puVar13[3] = puVar13[3] | 0x10;
    }
  }
  puVar13 = &DAT_ram_f96f;
  sVar14 = _DAT_ram_f96f;
  if ((*(byte *)(_DAT_ram_f96f + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  if ((char)DAT_ram_f9a8 < '\0') {
    uRamf9ab = 0;
    bVar1 = 4;
    z80_candidate_8(0xb600);
    DAT_ram_f97f = 0xff;
  }
  _DAT_ram_f80f =
       CONCAT11(DAT_ram_f980 + (char)((ushort)_DAT_ram_f978 >> 8),(char)_DAT_ram_f978) +
       *(short *)(sVar14 + 0x10);
  bVar4 = z80_candidate_22cf(0xf9bc);
  uVar7 = (ushort)bVar1;
  _DAT_ram_f80f = ((ushort)bVar4 * 2 - uVar7) + _DAT_ram_f80f;
  sVar10 = -0x636;
  z80_candidate_243f();
  cVar6 = (char)uVar7;
  sVar10 = sVar10 + _DAT_ram_f80f;
  if (_DAT_ram_f97d != sVar10) {
    bVar1 = (byte)((ushort)sVar10 >> 8);
    sVar11 = sVar10;
    if (sVar10 < 0) {
      bVar1 = 0;
      sVar11 = 0;
    }
    sVar11 = CONCAT11(*(undefined1 *)CONCAT11(4,(char)sVar11),
                      *(undefined1 *)CONCAT11(5,(char)sVar11));
    if (7 < bVar1) {
      if (bVar1 == 8) {
        bVar1 = 7;
        sVar11 = sVar11 * 2;
      }
      else {
        sVar11 = -1;
      }
    }
    cVar6 = '\x04';
    _DAT_ram_f97d = sVar10;
    z80_candidate_8(CONCAT11(0xa6,bVar1 << 3 | (byte)((ushort)sVar11 >> 8)));
    z80_candidate_8(CONCAT11(0xa2,(char)sVar11));
  }
  cVar2 = z80_candidate_22cf(0xf9ae);
  bVar1 = (cVar2 - cVar6) - 1;
  if ((char)DAT_ram_f9a8 < '\0') {
    bVar1 = 0;
  }
  bVar4 = DAT_ram_f97a;
  if (((DAT_ram_f97a != 0) && (bVar1 != 0xff)) && (bVar4 = bVar1, bVar1 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar1);
    bVar5 = *(byte *)CONCAT11(6,DAT_ram_f97a);
    bVar4 = -CARRY1(bVar3,bVar5);
    if (CARRY1(bVar3,bVar5)) {
      bVar4 = *(byte *)CONCAT11(7,bVar3 + bVar5);
    }
  }
  bVar5 = *(byte *)(sVar14 + 0x12);
  bVar3 = bVar4;
  if (((bVar4 != 0) && (bVar5 != 0xff)) && (bVar3 = bVar5, bVar5 != 0)) {
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar3 = -CARRY1(bVar5,bVar4);
    if (CARRY1(bVar5,bVar4)) {
      bVar3 = *(byte *)CONCAT11(7,bVar5 + bVar4);
    }
  }
  bVar4 = 0xff;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar4 = DAT_ram_f805;
  }
  bVar5 = bVar4;
  if (((bVar4 != 0) && (bVar3 != 0xff)) && (bVar5 = bVar3, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar5 = -CARRY1(bVar3,bVar4);
    if (CARRY1(bVar3,bVar4)) {
      bVar5 = *(byte *)CONCAT11(7,bVar3 + bVar4);
    }
  }
  if (bVar5 != DAT_ram_f97f) {
    bVar1 = *(byte *)CONCAT11(10,bVar5 | 0x80);
    bVar4 = puVar13[0x39] & 7;
    if (3 < bVar4) {
      if (4 < bVar4) {
        if ((6 < bVar4) &&
           (bVar4 = (char)(DAT_ram_f98b + bVar1) >> 7 | DAT_ram_f98b + bVar1, bVar4 != bRamf984)) {
          DAT_ram_f98a = DAT_ram_f98a | 2;
          bRamf984 = bVar4;
        }
        bVar4 = (char)(DAT_ram_f99d + bVar1) >> 7 | DAT_ram_f99d + bVar1;
        if (bVar4 != bRamf996) {
          DAT_ram_f99c = DAT_ram_f99c | 2;
          bRamf996 = bVar4;
        }
      }
      bVar4 = (char)(DAT_ram_f994 + bVar1) >> 7 | DAT_ram_f994 + bVar1;
      if (bVar4 != bRamf98d) {
        DAT_ram_f993 = DAT_ram_f993 | 2;
        bRamf98d = bVar4;
      }
    }
    bVar4 = (char)(DAT_ram_f9a6 + bVar1) >> 7 | DAT_ram_f9a6 + bVar1;
    if (bVar4 != bRamf99f) {
      DAT_ram_f9a5 = DAT_ram_f9a5 | 2;
      bRamf99f = bVar4;
    }
  }
  uVar8 = CONCAT11(bVar1,4);
  DAT_ram_f97f = bVar5;
  if (DAT_ram_f98a != 0) {
    z80_candidate_2d53(uVar8,0xf983);
    DAT_ram_f98a = 0;
  }
  uVar9 = (undefined1)((ushort)uVar8 >> 8);
  if (DAT_ram_f993 != 0) {
    z80_candidate_2d53(0xf98c);
    DAT_ram_f993 = 0;
  }
  if (DAT_ram_f99c != 0) {
    z80_candidate_2d53(0xf995);
    DAT_ram_f99c = 0;
  }
  if (DAT_ram_f9a5 != 0) {
    z80_candidate_2d53(0xf99e);
    DAT_ram_f9a5 = 0;
  }
  DAT_ram_f9a8 = DAT_ram_f9a8 & 0x7f;
  sVar10 = -0x657;
  if (DAT_ram_f9a8 != bRamf9a9) {
    bRamf9a9 = DAT_ram_f9a8;
    z80_candidate_8(CONCAT11(0xb2,DAT_ram_f9a8));
  }
  cVar6 = *(char *)(sVar10 + 1);
  pcVar12 = (char *)(sVar10 + 2);
  if (cVar6 != *pcVar12) {
    *pcVar12 = cVar6;
    uVar8 = CONCAT11(uVar9,4);
    z80_candidate_8(uVar8,CONCAT11(0xb6,cVar6));
    uVar9 = (undefined1)((ushort)uVar8 >> 8);
  }
  cVar6 = pcVar12[1];
  if (cVar6 != pcVar12[2]) {
    pcVar12[2] = cVar6;
    z80_candidate_8(CONCAT11(uVar9,4),CONCAT11(0x28,cVar6));
  }
  bVar1 = 1;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar1 = DAT_ram_f809;
  }
  DAT_ram_f975 = (DAT_ram_f975 + bVar1) - CARRY1(DAT_ram_f975,bVar1);
  if (DAT_ram_f974 == '\0') {
    if (((*(byte *)(sVar14 + 9) & 0x10) == 0) && ((bRamf972 & 0x10) != 0)) {
      bRamf972 = bRamf972 & 0xef;
LAB_ram_3217:
      DAT_ram_f9ac = DAT_ram_f981;
    }
  }
  else {
    DAT_ram_f974 = DAT_ram_f974 - bVar1;
    if (DAT_ram_f974 == '\0') {
      if ((*(byte *)(sVar14 + 9) & 0x10) == 0) goto LAB_ram_3217;
      puVar13[3] = puVar13[3] | 0x10;
    }
  }
  puVar13 = &DAT_ram_f9d7;
  sVar14 = _DAT_ram_f9d7;
  if ((*(byte *)(_DAT_ram_f9d7 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  if ((char)DAT_ram_fa10 < '\0') {
    uRamfa13 = 0;
    bVar1 = 6;
    z80_candidate_8(0xb500);
    DAT_ram_f9e7 = 0xff;
  }
  _DAT_ram_f80f =
       CONCAT11(DAT_ram_f9e8 + (char)((ushort)_DAT_ram_f9e0 >> 8),(char)_DAT_ram_f9e0) +
       *(short *)(sVar14 + 0x10);
  bVar4 = z80_candidate_22cf(0xfa24);
  uVar7 = (ushort)bVar1;
  _DAT_ram_f80f = ((ushort)bVar4 * 2 - uVar7) + _DAT_ram_f80f;
  sVar10 = -0x5ce;
  z80_candidate_243f();
  cVar6 = (char)uVar7;
  sVar10 = sVar10 + _DAT_ram_f80f;
  if (_DAT_ram_f9e5 != sVar10) {
    bVar1 = (byte)((ushort)sVar10 >> 8);
    sVar11 = sVar10;
    if (sVar10 < 0) {
      bVar1 = 0;
      sVar11 = 0;
    }
    sVar11 = CONCAT11(*(undefined1 *)CONCAT11(4,(char)sVar11),
                      *(undefined1 *)CONCAT11(5,(char)sVar11));
    if (7 < bVar1) {
      if (bVar1 == 8) {
        bVar1 = 7;
        sVar11 = sVar11 * 2;
      }
      else {
        sVar11 = -1;
      }
    }
    cVar6 = '\x06';
    _DAT_ram_f9e5 = sVar10;
    z80_candidate_8(CONCAT11(0xa5,bVar1 << 3 | (byte)((ushort)sVar11 >> 8)));
    z80_candidate_8(CONCAT11(0xa1,(char)sVar11));
  }
  cVar2 = z80_candidate_22cf(0xfa16);
  bVar1 = (cVar2 - cVar6) - 1;
  if ((char)DAT_ram_fa10 < '\0') {
    bVar1 = 0;
  }
  bVar4 = DAT_ram_f9e2;
  if (((DAT_ram_f9e2 != 0) && (bVar1 != 0xff)) && (bVar4 = bVar1, bVar1 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar1);
    bVar5 = *(byte *)CONCAT11(6,DAT_ram_f9e2);
    bVar4 = -CARRY1(bVar3,bVar5);
    if (CARRY1(bVar3,bVar5)) {
      bVar4 = *(byte *)CONCAT11(7,bVar3 + bVar5);
    }
  }
  bVar5 = *(byte *)(sVar14 + 0x12);
  bVar3 = bVar4;
  if (((bVar4 != 0) && (bVar5 != 0xff)) && (bVar3 = bVar5, bVar5 != 0)) {
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar3 = -CARRY1(bVar5,bVar4);
    if (CARRY1(bVar5,bVar4)) {
      bVar3 = *(byte *)CONCAT11(7,bVar5 + bVar4);
    }
  }
  bVar4 = 0xff;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar4 = DAT_ram_f805;
  }
  bVar5 = bVar4;
  if (((bVar4 != 0) && (bVar3 != 0xff)) && (bVar5 = bVar3, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar5 = -CARRY1(bVar3,bVar4);
    if (CARRY1(bVar3,bVar4)) {
      bVar5 = *(byte *)CONCAT11(7,bVar3 + bVar4);
    }
  }
  if (bVar5 != DAT_ram_f9e7) {
    bVar1 = *(byte *)CONCAT11(10,bVar5 | 0x80);
    bVar4 = puVar13[0x39] & 7;
    if (3 < bVar4) {
      if (4 < bVar4) {
        if ((6 < bVar4) &&
           (bVar4 = (char)(DAT_ram_f9f3 + bVar1) >> 7 | DAT_ram_f9f3 + bVar1, bVar4 != bRamf9ec)) {
          DAT_ram_f9f2 = DAT_ram_f9f2 | 2;
          bRamf9ec = bVar4;
        }
        bVar4 = (char)(DAT_ram_fa05 + bVar1) >> 7 | DAT_ram_fa05 + bVar1;
        if (bVar4 != bRamf9fe) {
          DAT_ram_fa04 = DAT_ram_fa04 | 2;
          bRamf9fe = bVar4;
        }
      }
      bVar4 = (char)(DAT_ram_f9fc + bVar1) >> 7 | DAT_ram_f9fc + bVar1;
      if (bVar4 != bRamf9f5) {
        DAT_ram_f9fb = DAT_ram_f9fb | 2;
        bRamf9f5 = bVar4;
      }
    }
    bVar4 = (char)(DAT_ram_fa0e + bVar1) >> 7 | DAT_ram_fa0e + bVar1;
    if (bVar4 != bRamfa07) {
      DAT_ram_fa0d = DAT_ram_fa0d | 2;
      bRamfa07 = bVar4;
    }
  }
  uVar8 = CONCAT11(bVar1,6);
  DAT_ram_f9e7 = bVar5;
  if (DAT_ram_f9f2 != 0) {
    z80_candidate_2d53(uVar8,0xf9eb);
    DAT_ram_f9f2 = 0;
  }
  uVar9 = (undefined1)((ushort)uVar8 >> 8);
  if (DAT_ram_f9fb != 0) {
    z80_candidate_2d53(0xf9f4);
    DAT_ram_f9fb = 0;
  }
  if (DAT_ram_fa04 != 0) {
    z80_candidate_2d53(0xf9fd);
    DAT_ram_fa04 = 0;
  }
  if (DAT_ram_fa0d != 0) {
    z80_candidate_2d53(0xfa06);
    DAT_ram_fa0d = 0;
  }
  DAT_ram_fa10 = DAT_ram_fa10 & 0x7f;
  sVar10 = -0x5ef;
  if (DAT_ram_fa10 != bRamfa11) {
    bRamfa11 = DAT_ram_fa10;
    z80_candidate_8(CONCAT11(0xb1,DAT_ram_fa10));
  }
  cVar6 = *(char *)(sVar10 + 1);
  pcVar12 = (char *)(sVar10 + 2);
  if (cVar6 != *pcVar12) {
    *pcVar12 = cVar6;
    uVar8 = CONCAT11(uVar9,6);
    z80_candidate_8(uVar8,CONCAT11(0xb5,cVar6));
    uVar9 = (undefined1)((ushort)uVar8 >> 8);
  }
  cVar6 = pcVar12[1];
  if (cVar6 != pcVar12[2]) {
    pcVar12[2] = cVar6;
    z80_candidate_8(CONCAT11(uVar9,4),CONCAT11(0x28,cVar6));
  }
  bVar1 = 1;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar1 = DAT_ram_f809;
  }
  DAT_ram_f9dd = (DAT_ram_f9dd + bVar1) - CARRY1(DAT_ram_f9dd,bVar1);
  if (DAT_ram_f9dc == '\0') {
    if (((*(byte *)(sVar14 + 9) & 0x10) != 0) || ((bRamf9da & 0x10) == 0)) goto LAB_ram_344c;
    bRamf9da = bRamf9da & 0xef;
  }
  else {
    DAT_ram_f9dc = DAT_ram_f9dc - bVar1;
    if (DAT_ram_f9dc != '\0') goto LAB_ram_344c;
    if ((*(byte *)(sVar14 + 9) & 0x10) != 0) {
      puVar13[3] = puVar13[3] | 0x10;
      goto LAB_ram_344c;
    }
  }
  DAT_ram_fa14 = DAT_ram_f9e9;
LAB_ram_344c:
  puVar13 = &DAT_ram_fa3f;
  sVar14 = _DAT_ram_fa3f;
  if ((*(byte *)(_DAT_ram_fa3f + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  if ((char)DAT_ram_fa78 < '\0') {
    uRamfa7b = 0;
    bVar1 = 6;
    z80_candidate_8(0xb600);
    DAT_ram_fa4f = 0xff;
  }
  _DAT_ram_f80f =
       CONCAT11(DAT_ram_fa50 + (char)((ushort)_DAT_ram_fa48 >> 8),(char)_DAT_ram_fa48) +
       *(short *)(sVar14 + 0x10);
  bVar4 = z80_candidate_22cf(0xfa8c);
  uVar7 = (ushort)bVar1;
  _DAT_ram_f80f = ((ushort)bVar4 * 2 - uVar7) + _DAT_ram_f80f;
  sVar10 = -0x566;
  z80_candidate_243f();
  cVar6 = (char)uVar7;
  sVar10 = sVar10 + _DAT_ram_f80f;
  if (_DAT_ram_fa4d != sVar10) {
    bVar1 = (byte)((ushort)sVar10 >> 8);
    sVar11 = sVar10;
    if (sVar10 < 0) {
      bVar1 = 0;
      sVar11 = 0;
    }
    sVar11 = CONCAT11(*(undefined1 *)CONCAT11(4,(char)sVar11),
                      *(undefined1 *)CONCAT11(5,(char)sVar11));
    if (7 < bVar1) {
      if (bVar1 == 8) {
        bVar1 = 7;
        sVar11 = sVar11 * 2;
      }
      else {
        sVar11 = -1;
      }
    }
    cVar6 = '\x06';
    _DAT_ram_fa4d = sVar10;
    z80_candidate_8(CONCAT11(0xa6,bVar1 << 3 | (byte)((ushort)sVar11 >> 8)));
    z80_candidate_8(CONCAT11(0xa2,(char)sVar11));
  }
  cVar2 = z80_candidate_22cf(0xfa7e);
  bVar1 = (cVar2 - cVar6) - 1;
  if ((char)DAT_ram_fa78 < '\0') {
    bVar1 = 0;
  }
  bVar4 = DAT_ram_fa4a;
  if (((DAT_ram_fa4a != 0) && (bVar1 != 0xff)) && (bVar4 = bVar1, bVar1 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar1);
    bVar5 = *(byte *)CONCAT11(6,DAT_ram_fa4a);
    bVar4 = -CARRY1(bVar3,bVar5);
    if (CARRY1(bVar3,bVar5)) {
      bVar4 = *(byte *)CONCAT11(7,bVar3 + bVar5);
    }
  }
  bVar5 = *(byte *)(sVar14 + 0x12);
  bVar3 = bVar4;
  if (((bVar4 != 0) && (bVar5 != 0xff)) && (bVar3 = bVar5, bVar5 != 0)) {
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar3 = -CARRY1(bVar5,bVar4);
    if (CARRY1(bVar5,bVar4)) {
      bVar3 = *(byte *)CONCAT11(7,bVar5 + bVar4);
    }
  }
  bVar4 = 0xff;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar4 = DAT_ram_f805;
  }
  bVar5 = bVar4;
  if (((bVar4 != 0) && (bVar3 != 0xff)) && (bVar5 = bVar3, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar5 = -CARRY1(bVar3,bVar4);
    if (CARRY1(bVar3,bVar4)) {
      bVar5 = *(byte *)CONCAT11(7,bVar3 + bVar4);
    }
  }
  if (bVar5 != DAT_ram_fa4f) {
    bVar1 = *(byte *)CONCAT11(10,bVar5 | 0x80);
    bVar4 = puVar13[0x39] & 7;
    if (3 < bVar4) {
      if (4 < bVar4) {
        if ((6 < bVar4) &&
           (bVar4 = (char)(DAT_ram_fa5b + bVar1) >> 7 | DAT_ram_fa5b + bVar1, bVar4 != bRamfa54)) {
          DAT_ram_fa5a = DAT_ram_fa5a | 2;
          bRamfa54 = bVar4;
        }
        bVar4 = (char)(DAT_ram_fa6d + bVar1) >> 7 | DAT_ram_fa6d + bVar1;
        if (bVar4 != bRamfa66) {
          DAT_ram_fa6c = DAT_ram_fa6c | 2;
          bRamfa66 = bVar4;
        }
      }
      bVar4 = (char)(DAT_ram_fa64 + bVar1) >> 7 | DAT_ram_fa64 + bVar1;
      if (bVar4 != bRamfa5d) {
        DAT_ram_fa63 = DAT_ram_fa63 | 2;
        bRamfa5d = bVar4;
      }
    }
    bVar4 = (char)(DAT_ram_fa76 + bVar1) >> 7 | DAT_ram_fa76 + bVar1;
    if (bVar4 != bRamfa6f) {
      DAT_ram_fa75 = DAT_ram_fa75 | 2;
      bRamfa6f = bVar4;
    }
  }
  uVar8 = CONCAT11(bVar1,6);
  DAT_ram_fa4f = bVar5;
  if (DAT_ram_fa5a != 0) {
    z80_candidate_2d53(uVar8,0xfa53);
    DAT_ram_fa5a = 0;
  }
  uVar9 = (undefined1)((ushort)uVar8 >> 8);
  if (DAT_ram_fa63 != 0) {
    z80_candidate_2d53(0xfa5c);
    DAT_ram_fa63 = 0;
  }
  if (DAT_ram_fa6c != 0) {
    z80_candidate_2d53(0xfa65);
    DAT_ram_fa6c = 0;
  }
  if (DAT_ram_fa75 != 0) {
    z80_candidate_2d53(0xfa6e);
    DAT_ram_fa75 = 0;
  }
  DAT_ram_fa78 = DAT_ram_fa78 & 0x7f;
  sVar10 = -0x587;
  if (DAT_ram_fa78 != bRamfa79) {
    bRamfa79 = DAT_ram_fa78;
    z80_candidate_8(CONCAT11(0xb2,DAT_ram_fa78));
  }
  cVar6 = *(char *)(sVar10 + 1);
  pcVar12 = (char *)(sVar10 + 2);
  if (cVar6 != *pcVar12) {
    *pcVar12 = cVar6;
    uVar8 = CONCAT11(uVar9,6);
    z80_candidate_8(uVar8,CONCAT11(0xb6,cVar6));
    uVar9 = (undefined1)((ushort)uVar8 >> 8);
  }
  cVar6 = pcVar12[1];
  if (cVar6 != pcVar12[2]) {
    pcVar12[2] = cVar6;
    z80_candidate_8(CONCAT11(uVar9,4),CONCAT11(0x28,cVar6));
  }
  bVar1 = 1;
  if ((*(byte *)(sVar14 + 9) & 2) != 0) {
    bVar1 = DAT_ram_f809;
  }
  DAT_ram_fa45 = (DAT_ram_fa45 + bVar1) - CARRY1(DAT_ram_fa45,bVar1);
  if (DAT_ram_fa44 == '\0') {
    if ((*(byte *)(sVar14 + 9) & 0x10) != 0) {
      return;
    }
    if ((bRamfa42 & 0x10) == 0) {
      return;
    }
    bRamfa42 = bRamfa42 & 0xef;
    cVar6 = DAT_ram_fa44;
  }
  else {
    DAT_ram_fa44 = DAT_ram_fa44 - bVar1;
    if (DAT_ram_fa44 != '\0') {
      return;
    }
    cVar6 = '\0';
    if ((*(byte *)(sVar14 + 9) & 0x10) != 0) {
      puVar13[3] = puVar13[3] | 0x10;
      return;
    }
  }
  DAT_ram_fa44 = cVar6;
  DAT_ram_fa7c = DAT_ram_fa51;
  return;
}
