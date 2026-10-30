/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x38c7; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_38c7(undefined2 param_1)

{
  char cVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  byte bVar5;
  undefined2 uVar6;
  undefined1 uVar8;
  undefined2 uVar7;
  ushort uVar9;
  undefined2 uVar10;
  undefined *puVar11;
  short sVar12;

  uVar8 = (undefined1)((ushort)param_1 >> 8);
  puVar11 = &DAT_ram_faa9;
  sVar12 = _DAT_ram_faa9;
  if ((*(byte *)(_DAT_ram_faa9 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  uVar7 = _DAT_ram_fac1;
  if (DAT_ram_fac3 != '\0') {
    if (DAT_ram_fac3 == '\x02') {
      DAT_ram_fac3 = '\x01';
      uVar6 = CONCAT11((char)((ushort)_DAT_ram_fac1 >> 8),6);
      uVar10 = _DAT_ram_fabf;
      z80_candidate_8(uVar6,_DAT_ram_fabf,0x8c0);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      DAT_ram_fab9 = '\0';
      z80_candidate_8(0x81);
      uVar9 = 0x1c01;
      uVar6 = CONCAT11(uVar8,4);
      z80_candidate_8(uVar6,0x1c01);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar9 & 0xff00);
      uVar6 = CONCAT11(uVar8,6);
      z80_candidate_8(uVar6,CONCAT11(0x10,(char)uVar10));
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar7,CONCAT11(0x18,(char)((ushort)uVar10 >> 8)));
      z80_candidate_8(CONCAT11(0x20,(char)uVar7));
      z80_candidate_8(CONCAT11(0x28,(char)((ushort)uVar7 >> 8)));
      z80_candidate_8(1);
    }
    else if ((DAT_ram_f80a & 1) != 0) {
      DAT_ram_fac3 = '\0';
      DAT_ram_fab4 = '\0';
      goto LAB_ram_39af;
    }
    cVar1 = z80_candidate_21de(0xfac4);
    cVar3 = DAT_ram_fab4;
    if (((DAT_ram_fab4 != '\0') && (cVar1 != -1)) && (cVar3 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,DAT_ram_fab4);
      cVar3 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar3 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar1 = *(char *)(sVar12 + 0x12);
    cVar4 = cVar3;
    if (((cVar3 != '\0') && (cVar1 != -1)) && (cVar4 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      cVar4 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar4 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar3 = -1;
    if ((*(byte *)(sVar12 + 9) & 2) != 0) {
      cVar3 = DAT_ram_f805;
    }
    DAT_ram_fab9 = cVar3;
    if (((cVar3 != '\0') && (cVar4 != -1)) && (DAT_ram_fab9 = cVar4, cVar4 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar4);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      DAT_ram_fab9 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        DAT_ram_fab9 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    bVar5 = *(byte *)CONCAT11(0xb,DAT_ram_fab9);
    if (bVar5 != 0) {
      bVar5 = (byte)((DAT_ram_fabe & 3) << 7) >> 1 | ((DAT_ram_fabe & 3) >> 1) << 7 | bVar5 - 1;
    }
    uVar7 = CONCAT11(uVar8,6);
    z80_candidate_8(uVar7,CONCAT11(8,bVar5));
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
LAB_ram_39af:
  bVar5 = 1;
  if ((*(byte *)(sVar12 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_faaf = (DAT_ram_faaf + bVar5) - CARRY1(DAT_ram_faaf,bVar5);
  if (DAT_ram_faae == '\0') {
    if (((*(byte *)(sVar12 + 9) & 0x10) == 0) && ((bRamfaac & 0x10) != 0)) {
      bRamfaac = bRamfaac & 0xef;
LAB_ram_39eb:
      z80_candidate_21ca(0xfac4);
    }
  }
  else {
    DAT_ram_faae = DAT_ram_faae - bVar5;
    if (DAT_ram_faae == '\0') {
      if ((*(byte *)(sVar12 + 9) & 0x10) == 0) goto LAB_ram_39eb;
      puVar11[3] = puVar11[3] | 0x10;
    }
  }
  puVar11 = &DAT_ram_fad2;
  sVar12 = _DAT_ram_fad2;
  if ((*(byte *)(_DAT_ram_fad2 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  uVar7 = _DAT_ram_faea;
  if (DAT_ram_faec != '\0') {
    if (DAT_ram_faec == '\x02') {
      DAT_ram_faec = '\x01';
      uVar6 = CONCAT11((char)((ushort)_DAT_ram_faea >> 8),6);
      uVar10 = _DAT_ram_fae8;
      z80_candidate_8(uVar6,_DAT_ram_fae8,0x9c0);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      DAT_ram_fae2 = '\0';
      z80_candidate_8(0x82);
      uVar9 = 0x1c02;
      uVar6 = CONCAT11(uVar8,4);
      z80_candidate_8(uVar6,0x1c02);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar9 & 0xff00);
      uVar6 = CONCAT11(uVar8,6);
      z80_candidate_8(uVar6,CONCAT11(0x11,(char)uVar10));
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar7,CONCAT11(0x19,(char)((ushort)uVar10 >> 8)));
      z80_candidate_8(CONCAT11(0x21,(char)uVar7));
      z80_candidate_8(CONCAT11(0x29,(char)((ushort)uVar7 >> 8)));
      z80_candidate_8(2);
    }
    else if ((DAT_ram_f80a & 2) != 0) {
      DAT_ram_faec = '\0';
      DAT_ram_fadd = '\0';
      goto LAB_ram_3ad9;
    }
    cVar1 = z80_candidate_21de(0xfaed);
    cVar3 = DAT_ram_fadd;
    if (((DAT_ram_fadd != '\0') && (cVar1 != -1)) && (cVar3 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,DAT_ram_fadd);
      cVar3 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar3 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar1 = *(char *)(sVar12 + 0x12);
    cVar4 = cVar3;
    if (((cVar3 != '\0') && (cVar1 != -1)) && (cVar4 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      cVar4 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar4 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar3 = -1;
    if ((*(byte *)(sVar12 + 9) & 2) != 0) {
      cVar3 = DAT_ram_f805;
    }
    DAT_ram_fae2 = cVar3;
    if (((cVar3 != '\0') && (cVar4 != -1)) && (DAT_ram_fae2 = cVar4, cVar4 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar4);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      DAT_ram_fae2 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        DAT_ram_fae2 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    bVar5 = *(byte *)CONCAT11(0xb,DAT_ram_fae2);
    if (bVar5 != 0) {
      bVar5 = (byte)((DAT_ram_fae7 & 3) << 7) >> 1 | ((DAT_ram_fae7 & 3) >> 1) << 7 | bVar5 - 1;
    }
    uVar7 = CONCAT11(uVar8,6);
    z80_candidate_8(uVar7,CONCAT11(9,bVar5));
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
LAB_ram_3ad9:
  bVar5 = 1;
  if ((*(byte *)(sVar12 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_fad8 = (DAT_ram_fad8 + bVar5) - CARRY1(DAT_ram_fad8,bVar5);
  if (DAT_ram_fad7 == '\0') {
    if (((*(byte *)(sVar12 + 9) & 0x10) == 0) && ((bRamfad5 & 0x10) != 0)) {
      bRamfad5 = bRamfad5 & 0xef;
LAB_ram_3b15:
      z80_candidate_21ca(0xfaed);
    }
  }
  else {
    DAT_ram_fad7 = DAT_ram_fad7 - bVar5;
    if (DAT_ram_fad7 == '\0') {
      if ((*(byte *)(sVar12 + 9) & 0x10) == 0) goto LAB_ram_3b15;
      puVar11[3] = puVar11[3] | 0x10;
    }
  }
  puVar11 = &DAT_ram_fafb;
  sVar12 = _DAT_ram_fafb;
  if ((*(byte *)(_DAT_ram_fafb + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  uVar7 = _DAT_ram_fb13;
  if (DAT_ram_fb15 != '\0') {
    if (DAT_ram_fb15 == '\x02') {
      DAT_ram_fb15 = '\x01';
      uVar6 = CONCAT11((char)((ushort)_DAT_ram_fb13 >> 8),6);
      uVar10 = _DAT_ram_fb11;
      z80_candidate_8(uVar6,_DAT_ram_fb11,0xac0);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      DAT_ram_fb0b = '\0';
      z80_candidate_8(0x84);
      uVar9 = 0x1c04;
      uVar6 = CONCAT11(uVar8,4);
      z80_candidate_8(uVar6,0x1c04);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar9 & 0xff00);
      uVar6 = CONCAT11(uVar8,6);
      z80_candidate_8(uVar6,CONCAT11(0x12,(char)uVar10));
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar7,CONCAT11(0x1a,(char)((ushort)uVar10 >> 8)));
      z80_candidate_8(CONCAT11(0x22,(char)uVar7));
      z80_candidate_8(CONCAT11(0x2a,(char)((ushort)uVar7 >> 8)));
      z80_candidate_8(4);
    }
    else if ((DAT_ram_f80a & 4) != 0) {
      DAT_ram_fb15 = '\0';
      DAT_ram_fb06 = '\0';
      goto LAB_ram_3c03;
    }
    cVar1 = z80_candidate_21de(0xfb16);
    cVar3 = DAT_ram_fb06;
    if (((DAT_ram_fb06 != '\0') && (cVar1 != -1)) && (cVar3 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,DAT_ram_fb06);
      cVar3 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar3 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar1 = *(char *)(sVar12 + 0x12);
    cVar4 = cVar3;
    if (((cVar3 != '\0') && (cVar1 != -1)) && (cVar4 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      cVar4 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar4 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar3 = -1;
    if ((*(byte *)(sVar12 + 9) & 2) != 0) {
      cVar3 = DAT_ram_f805;
    }
    DAT_ram_fb0b = cVar3;
    if (((cVar3 != '\0') && (cVar4 != -1)) && (DAT_ram_fb0b = cVar4, cVar4 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar4);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      DAT_ram_fb0b = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        DAT_ram_fb0b = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    bVar5 = *(byte *)CONCAT11(0xb,DAT_ram_fb0b);
    if (bVar5 != 0) {
      bVar5 = (byte)((DAT_ram_fb10 & 3) << 7) >> 1 | ((DAT_ram_fb10 & 3) >> 1) << 7 | bVar5 - 1;
    }
    uVar7 = CONCAT11(uVar8,6);
    z80_candidate_8(uVar7,CONCAT11(10,bVar5));
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
LAB_ram_3c03:
  bVar5 = 1;
  if ((*(byte *)(sVar12 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_fb01 = (DAT_ram_fb01 + bVar5) - CARRY1(DAT_ram_fb01,bVar5);
  if (DAT_ram_fb00 == '\0') {
    if (((*(byte *)(sVar12 + 9) & 0x10) == 0) && ((bRamfafe & 0x10) != 0)) {
      bRamfafe = bRamfafe & 0xef;
LAB_ram_3c3f:
      z80_candidate_21ca(0xfb16);
    }
  }
  else {
    DAT_ram_fb00 = DAT_ram_fb00 - bVar5;
    if (DAT_ram_fb00 == '\0') {
      if ((*(byte *)(sVar12 + 9) & 0x10) == 0) goto LAB_ram_3c3f;
      puVar11[3] = puVar11[3] | 0x10;
    }
  }
  puVar11 = &DAT_ram_fb24;
  sVar12 = _DAT_ram_fb24;
  if ((*(byte *)(_DAT_ram_fb24 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  uVar7 = _DAT_ram_fb3c;
  if (DAT_ram_fb3e != '\0') {
    if (DAT_ram_fb3e == '\x02') {
      DAT_ram_fb3e = '\x01';
      uVar6 = CONCAT11((char)((ushort)_DAT_ram_fb3c >> 8),6);
      uVar10 = _DAT_ram_fb3a;
      z80_candidate_8(uVar6,_DAT_ram_fb3a,0xbc0);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      DAT_ram_fb34 = '\0';
      z80_candidate_8(0x88);
      uVar9 = 0x1c08;
      uVar6 = CONCAT11(uVar8,4);
      z80_candidate_8(uVar6,0x1c08);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar9 & 0xff00);
      uVar6 = CONCAT11(uVar8,6);
      z80_candidate_8(uVar6,CONCAT11(0x13,(char)uVar10));
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar7,CONCAT11(0x1b,(char)((ushort)uVar10 >> 8)));
      z80_candidate_8(CONCAT11(0x23,(char)uVar7));
      z80_candidate_8(CONCAT11(0x2b,(char)((ushort)uVar7 >> 8)));
      z80_candidate_8(8);
    }
    else if ((DAT_ram_f80a & 8) != 0) {
      DAT_ram_fb3e = '\0';
      DAT_ram_fb2f = '\0';
      goto LAB_ram_3d2d;
    }
    cVar1 = z80_candidate_21de(0xfb3f);
    cVar3 = DAT_ram_fb2f;
    if (((DAT_ram_fb2f != '\0') && (cVar1 != -1)) && (cVar3 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,DAT_ram_fb2f);
      cVar3 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar3 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar1 = *(char *)(sVar12 + 0x12);
    cVar4 = cVar3;
    if (((cVar3 != '\0') && (cVar1 != -1)) && (cVar4 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      cVar4 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar4 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar3 = -1;
    if ((*(byte *)(sVar12 + 9) & 2) != 0) {
      cVar3 = DAT_ram_f805;
    }
    DAT_ram_fb34 = cVar3;
    if (((cVar3 != '\0') && (cVar4 != -1)) && (DAT_ram_fb34 = cVar4, cVar4 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar4);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      DAT_ram_fb34 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        DAT_ram_fb34 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    bVar5 = *(byte *)CONCAT11(0xb,DAT_ram_fb34);
    if (bVar5 != 0) {
      bVar5 = (byte)((DAT_ram_fb39 & 3) << 7) >> 1 | ((DAT_ram_fb39 & 3) >> 1) << 7 | bVar5 - 1;
    }
    uVar7 = CONCAT11(uVar8,6);
    z80_candidate_8(uVar7,CONCAT11(0xb,bVar5));
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
LAB_ram_3d2d:
  bVar5 = 1;
  if ((*(byte *)(sVar12 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_fb2a = (DAT_ram_fb2a + bVar5) - CARRY1(DAT_ram_fb2a,bVar5);
  if (DAT_ram_fb29 == '\0') {
    if (((*(byte *)(sVar12 + 9) & 0x10) == 0) && ((bRamfb27 & 0x10) != 0)) {
      bRamfb27 = bRamfb27 & 0xef;
LAB_ram_3d69:
      z80_candidate_21ca(0xfb3f);
    }
  }
  else {
    DAT_ram_fb29 = DAT_ram_fb29 - bVar5;
    if (DAT_ram_fb29 == '\0') {
      if ((*(byte *)(sVar12 + 9) & 0x10) == 0) goto LAB_ram_3d69;
      puVar11[3] = puVar11[3] | 0x10;
    }
  }
  puVar11 = &DAT_ram_fb4d;
  sVar12 = _DAT_ram_fb4d;
  if ((*(byte *)(_DAT_ram_fb4d + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  uVar7 = _DAT_ram_fb65;
  if (DAT_ram_fb67 != '\0') {
    if (DAT_ram_fb67 == '\x02') {
      DAT_ram_fb67 = '\x01';
      uVar6 = CONCAT11((char)((ushort)_DAT_ram_fb65 >> 8),6);
      uVar10 = _DAT_ram_fb63;
      z80_candidate_8(uVar6,_DAT_ram_fb63,0xcc0);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      DAT_ram_fb5d = '\0';
      z80_candidate_8(0x90);
      uVar9 = 0x1c10;
      uVar6 = CONCAT11(uVar8,4);
      z80_candidate_8(uVar6,0x1c10);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar9 & 0xff00);
      uVar6 = CONCAT11(uVar8,6);
      z80_candidate_8(uVar6,CONCAT11(0x14,(char)uVar10));
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar7,CONCAT11(0x1c,(char)((ushort)uVar10 >> 8)));
      z80_candidate_8(CONCAT11(0x24,(char)uVar7));
      z80_candidate_8(CONCAT11(0x2c,(char)((ushort)uVar7 >> 8)));
      z80_candidate_8(0x10);
    }
    else if ((DAT_ram_f80a & 0x10) != 0) {
      DAT_ram_fb67 = '\0';
      DAT_ram_fb58 = '\0';
      goto LAB_ram_3e57;
    }
    cVar1 = z80_candidate_21de(0xfb68);
    cVar3 = DAT_ram_fb58;
    if (((DAT_ram_fb58 != '\0') && (cVar1 != -1)) && (cVar3 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,DAT_ram_fb58);
      cVar3 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar3 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar1 = *(char *)(sVar12 + 0x12);
    cVar4 = cVar3;
    if (((cVar3 != '\0') && (cVar1 != -1)) && (cVar4 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      cVar4 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar4 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar3 = -1;
    if ((*(byte *)(sVar12 + 9) & 2) != 0) {
      cVar3 = DAT_ram_f805;
    }
    DAT_ram_fb5d = cVar3;
    if (((cVar3 != '\0') && (cVar4 != -1)) && (DAT_ram_fb5d = cVar4, cVar4 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar4);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      DAT_ram_fb5d = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        DAT_ram_fb5d = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    bVar5 = *(byte *)CONCAT11(0xb,DAT_ram_fb5d);
    if (bVar5 != 0) {
      bVar5 = (byte)((DAT_ram_fb62 & 3) << 7) >> 1 | ((DAT_ram_fb62 & 3) >> 1) << 7 | bVar5 - 1;
    }
    uVar7 = CONCAT11(uVar8,6);
    z80_candidate_8(uVar7,CONCAT11(0xc,bVar5));
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
LAB_ram_3e57:
  bVar5 = 1;
  if ((*(byte *)(sVar12 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_fb53 = (DAT_ram_fb53 + bVar5) - CARRY1(DAT_ram_fb53,bVar5);
  if (DAT_ram_fb52 == '\0') {
    if (((*(byte *)(sVar12 + 9) & 0x10) == 0) && ((bRamfb50 & 0x10) != 0)) {
      bRamfb50 = bRamfb50 & 0xef;
LAB_ram_3e93:
      z80_candidate_21ca(0xfb68);
    }
  }
  else {
    DAT_ram_fb52 = DAT_ram_fb52 - bVar5;
    if (DAT_ram_fb52 == '\0') {
      if ((*(byte *)(sVar12 + 9) & 0x10) == 0) goto LAB_ram_3e93;
      puVar11[3] = puVar11[3] | 0x10;
    }
  }
  puVar11 = &DAT_ram_fb76;
  sVar12 = _DAT_ram_fb76;
  if ((*(byte *)(_DAT_ram_fb76 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  uVar7 = _DAT_ram_fb8e;
  if (DAT_ram_fb90 != '\0') {
    if (DAT_ram_fb90 == '\x02') {
      DAT_ram_fb90 = '\x01';
      uVar6 = CONCAT11((char)((ushort)_DAT_ram_fb8e >> 8),6);
      uVar10 = _DAT_ram_fb8c;
      z80_candidate_8(uVar6,_DAT_ram_fb8c,0xdc0);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      DAT_ram_fb86 = '\0';
      z80_candidate_8(0xa0);
      uVar9 = 0x1c20;
      uVar6 = CONCAT11(uVar8,4);
      z80_candidate_8(uVar6,0x1c20);
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar9 & 0xff00);
      uVar6 = CONCAT11(uVar8,6);
      z80_candidate_8(uVar6,CONCAT11(0x15,(char)uVar10));
      uVar8 = (undefined1)((ushort)uVar6 >> 8);
      z80_candidate_8(uVar7,CONCAT11(0x1d,(char)((ushort)uVar10 >> 8)));
      z80_candidate_8(CONCAT11(0x25,(char)uVar7));
      z80_candidate_8(CONCAT11(0x2d,(char)((ushort)uVar7 >> 8)));
      z80_candidate_8(0x20);
    }
    else if ((DAT_ram_f80a & 0x20) != 0) {
      DAT_ram_fb90 = '\0';
      DAT_ram_fb81 = '\0';
      goto LAB_ram_3f81;
    }
    cVar1 = z80_candidate_21de(0xfb91);
    cVar3 = DAT_ram_fb81;
    if (((DAT_ram_fb81 != '\0') && (cVar1 != -1)) && (cVar3 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,DAT_ram_fb81);
      cVar3 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar3 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar1 = *(char *)(sVar12 + 0x12);
    cVar4 = cVar3;
    if (((cVar3 != '\0') && (cVar1 != -1)) && (cVar4 = cVar1, cVar1 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar1);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      cVar4 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        cVar4 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    cVar3 = -1;
    if ((*(byte *)(sVar12 + 9) & 2) != 0) {
      cVar3 = DAT_ram_f805;
    }
    DAT_ram_fb86 = cVar3;
    if (((cVar3 != '\0') && (cVar4 != -1)) && (DAT_ram_fb86 = cVar4, cVar4 != '\0')) {
      bVar2 = *(byte *)CONCAT11(6,cVar4);
      bVar5 = *(byte *)CONCAT11(6,cVar3);
      DAT_ram_fb86 = -CARRY1(bVar2,bVar5);
      if (CARRY1(bVar2,bVar5)) {
        DAT_ram_fb86 = *(char *)CONCAT11(7,bVar2 + bVar5);
      }
    }
    bVar5 = *(byte *)CONCAT11(0xb,DAT_ram_fb86);
    if (bVar5 != 0) {
      bVar5 = (byte)((DAT_ram_fb8b & 3) << 7) >> 1 | ((DAT_ram_fb8b & 3) >> 1) << 7 | bVar5 - 1;
    }
    z80_candidate_8(CONCAT11(uVar8,6),CONCAT11(0xd,bVar5));
  }
LAB_ram_3f81:
  bVar5 = 1;
  if ((*(byte *)(sVar12 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_fb7c = (DAT_ram_fb7c + bVar5) - CARRY1(DAT_ram_fb7c,bVar5);
  if (DAT_ram_fb7b == '\0') {
    if ((*(byte *)(sVar12 + 9) & 0x10) != 0) {
      return;
    }
    if ((bRamfb79 & 0x10) == 0) {
      return;
    }
    bRamfb79 = bRamfb79 & 0xef;
    cVar3 = DAT_ram_fb7b;
  }
  else {
    DAT_ram_fb7b = DAT_ram_fb7b - bVar5;
    if (DAT_ram_fb7b != '\0') {
      return;
    }
    cVar3 = '\0';
    if ((*(byte *)(sVar12 + 9) & 0x10) != 0) {
      puVar11[3] = puVar11[3] | 0x10;
      return;
    }
  }
  DAT_ram_fb7b = cVar3;
  z80_candidate_21ca(0xfb91);
  return;
}
