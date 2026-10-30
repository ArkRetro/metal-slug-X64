/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x28c3; tentative_c_with_diagnostics. */

/* WARNING: Instruction at (ram,0x2b8c) overlaps instruction at (ram,0x2b8b)
    */
/* WARNING: Removing unreachable block (ram,0x2b04) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_28c3(byte param_1,short param_2,short param_3)

{
  bool bVar1;
  undefined2 in_AF;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  char cVar6;
  ushort uVar2;
  undefined2 uVar7;
  undefined1 uVar8;
  byte bVar9;
  undefined2 uVar10;
  short sVar11;
  byte *pbVar12;
  byte *pbVar13;
  byte *pbVar14;
  byte *pbVar15;
  undefined2 unaff_AF_;
  undefined *puVar16;
  short sVar17;

  bVar9 = (byte)((ushort)unaff_AF_ >> 8);
  bVar5 = param_1 >> 5;
  bVar3 = (byte)((byte)((byte)((byte)((byte)((ushort)in_AF >> 9) | param_1 << 7) >> 1 |
                              (param_1 >> 1) << 7) >> 1 | (param_1 >> 2) << 7) >> 1 |
                (param_1 >> 3) << 7) >> 1 | (param_1 >> 4) << 7;
  if (bRamf87e < bVar5) {
    uVar10 = CONCAT11(2,bVar3);
    uVar7 = CONCAT11(bVar3,4);
    bRamf87e = bVar5;
    z80_candidate_8(uVar7,bVar5,uVar10);
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
    z80_candidate_8(CONCAT11((char)((ushort)uVar10 >> 8) + '\x01',bVar5));
  }
  else {
    cVar6 = '\x03';
    uVar7 = CONCAT11(bVar3,4);
    bRamf87e = bVar5;
    z80_candidate_8(uVar7);
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
    z80_candidate_8(CONCAT11(cVar6 + -1,uVar8));
  }
  bVar5 = DAT_ram_f874;
  if (((DAT_ram_f874 != 0) && (DAT_ram_f811 != 0xff)) &&
     (bVar5 = DAT_ram_f811, bVar9 = DAT_ram_f874, DAT_ram_f811 != 0)) {
    bVar4 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar3 = *(byte *)CONCAT11(6,DAT_ram_f874);
    bVar5 = -CARRY1(bVar4,bVar3);
    if (CARRY1(bVar4,bVar3)) {
      bVar5 = *(byte *)CONCAT11(7,bVar4 + bVar3);
    }
  }
  bVar3 = *(byte *)(param_2 + 0x12);
  bVar4 = bVar5;
  if (((bVar5 != 0) && (bVar3 != 0xff)) && (bVar4 = bVar3, bVar9 = bVar5, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar4 = -CARRY1(bVar3,bVar5);
    if (CARRY1(bVar3,bVar5)) {
      bVar4 = *(byte *)CONCAT11(7,bVar3 + bVar5);
    }
  }
  bVar5 = 0xff;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f805;
  }
  bVar3 = bVar5;
  if (((bVar5 != 0) && (bVar4 != 0xff)) && (bVar3 = bVar4, bVar9 = bVar5, bVar4 != 0)) {
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar3 = -CARRY1(bVar4,bVar5);
    if (CARRY1(bVar4,bVar5)) {
      bVar3 = *(byte *)CONCAT11(7,bVar4 + bVar5);
    }
  }
  bVar1 = bVar3 != bRamf879;
  bRamf879 = bVar3;
  if (bVar1) {
    uVar7 = CONCAT11(uVar8,4);
    z80_candidate_8(uVar7,CONCAT11(9,*(undefined1 *)CONCAT11(10,bVar3)));
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
  bVar5 = 1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  pbVar15 = (byte *)CONCAT11(uVar8,bVar5);
  DAT_ram_f86f = (DAT_ram_f86f + bVar5) - CARRY1(DAT_ram_f86f,bVar5);
  if (DAT_ram_f86e == '\0') {
    if (((*(byte *)(param_2 + 9) & 0x10) == 0) && ((bRamf86c & 0x10) != 0)) {
      bRamf86c = bRamf86c & 0xef;
LAB_ram_299e:
      z80_candidate_21ca(0xf87f);
    }
  }
  else {
    DAT_ram_f86e = DAT_ram_f86e - bVar5;
    if (DAT_ram_f86e == '\0') {
      if ((*(byte *)(param_2 + 9) & 0x10) == 0) goto LAB_ram_299e;
      *(byte *)(param_3 + 3) = *(byte *)(param_3 + 3) | 0x10;
    }
  }
  puVar16 = &DAT_ram_f8b6;
  sVar17 = _DAT_ram_f8b6;
  if ((*(byte *)(_DAT_ram_f8b6 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  DAT_ram_f811 = z80_candidate_21de(0xf8cc);
  if (DAT_ram_f811 == 0) goto LAB_ram_2b20;
  cVar6 = z80_candidate_22cf(0xf8da);
  cVar6 = cVar6 - (char)pbVar15;
  bVar5 = cVar6 - 1;
  bVar3 = DAT_ram_f811;
  if (((DAT_ram_f811 != 0) && (cVar6 != '\0')) && (bVar3 = bVar5, bVar9 = DAT_ram_f811, bVar5 != 0))
  {
    bVar4 = *(byte *)CONCAT11(6,bVar5);
    bVar5 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar3 = -CARRY1(bVar4,bVar5);
    if (CARRY1(bVar4,bVar5)) {
      bVar3 = *(byte *)CONCAT11(7,bVar4 + bVar5);
    }
  }
  DAT_ram_f811 = bVar3;
  if ((DAT_ram_f811 == 0) || (DAT_ram_f8c1 == '\0')) goto LAB_ram_2b20;
  if (DAT_ram_f8c9 != '\0') {
    bVar5 = z80_candidate_22cf(0xf8e8);
    sVar11 = (ushort)bVar5 * 2 - ((ushort)pbVar15 & 0xff);
    cVar6 = (char)((ushort)sVar11 >> 8);
    bVar5 = (byte)((byte)((byte)sVar11 >> 1 | cVar6 << 7) >> 1 | (cVar6 >> 1) << 7) >> 1 |
            (cVar6 >> 2) << 7;
    pbVar15 = (byte *)((ushort)bVar5 << 8);
    DAT_ram_f903 = (bRamf8be & 0x1f) + bVar5;
    if ((char)DAT_ram_f903 < '\0') {
      DAT_ram_f903 = 0;
    }
    if (0x1f < DAT_ram_f903) {
      DAT_ram_f903 = 0x1f;
    }
    goto LAB_ram_2b20;
  }
  _DAT_ram_f80f = _DAT_ram_f8bf + _DAT_ram_f8c7 + *(short *)(sVar17 + 0x10);
  bVar5 = z80_candidate_22cf(0xf8e8);
  pbVar15 = (byte *)((ushort)pbVar15 & 0xff);
  _DAT_ram_f80f = ((ushort)bVar5 * 2 - (short)pbVar15) + _DAT_ram_f80f;
  sVar11 = -0x70a;
  z80_candidate_243f();
  pbVar12 = (byte *)(sVar11 + _DAT_ram_f80f);
  if (_DAT_ram_f8c4 == pbVar12) goto LAB_ram_2b20;
  bVar3 = (byte)((ushort)pbVar12 >> 8);
  bVar5 = bVar3;
  if (0xf < bVar3) {
    bVar5 = 0xf;
  }
  bVar5 = bVar5 << 2;
  pbVar15 = (byte *)(ushort)bVar5;
  bVar4 = *(byte *)CONCAT11(2,(char)pbVar12);
  pbVar14 = (byte *)(ushort)*(byte *)CONCAT11(3,(char)pbVar12);
  bVar1 = (byte *)0xd56f < pbVar15;
  uVar2 = CONCAT11(bVar4,bVar1);
  pbVar13 = pbVar15 + 0x2a90;
  _DAT_ram_f8c4 = pbVar12;
  switch(bVar5) {
  case 0:
    pbVar13 = pbVar14;
    break;
  case 4:
    goto z80_candidate_2afd;
  case 8:
    goto z80_candidate_2afa;
  case 0xc:
    goto z80_candidate_2af7;
  case 0x10:
    pbVar13 = pbVar14;
  case 100:
z80_candidate_2af4:
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),(byte)pbVar13 >> 1);
    uVar2 = (ushort)(byte)((byte)(uVar2 >> 9) | (byte)pbVar13 << 7) << 8;
z80_candidate_2af7:
    bVar1 = (bool)((byte)pbVar14 & 1);
    bVar4 = (byte)(uVar2 >> 8);
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
code_r0x2af9:
    uVar2 = (ushort)(byte)(bVar4 >> 1 | bVar1 << 7) << 8;
z80_candidate_2afa:
    uVar2 = CONCAT11((char)(uVar2 >> 8),(byte)pbVar14) & 0xff01;
    pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
code_r0x2afc:
    pbVar14 = pbVar13;
    uVar2 = (ushort)(byte)((byte)(uVar2 >> 9) | (char)uVar2 << 7) << 8;
z80_candidate_2afd:
    uVar2 = (ushort)(byte)((byte)(uVar2 >> 9) | (byte)pbVar14 << 7) << 8;
    pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
    break;
  case 0x14:
    goto z80_candidate_2af1;
  case 0x18:
    goto z80_candidate_2ae6;
  case 0x1c:
    goto LAB_ram_2ae9;
  case 0x20:
    pbVar13 = pbVar14;
    goto LAB_ram_2aec;
  case 0x24:
    goto z80_candidate_2adf;
  case 0x28:
    goto z80_candidate_2add;
  case 0x2c:
    goto z80_candidate_2adb;
  case 0x30:
    goto z80_candidate_2ad9;
  case 0x34:
    pbVar13 = pbVar14;
  case 0x40:
    pbVar14 = (byte *)((short)pbVar13 * 2);
LAB_ram_2ad1:
    pbVar14 = (byte *)((short)pbVar14 * 2);
LAB_ram_2ad2:
    pbVar13 = (byte *)((short)pbVar14 * 2);
    uVar2 = (ushort)pbVar13 & 0xff00;
code_r0x2ad4:
    pbVar13 = (byte *)((ushort)pbVar13 & 0xff00);
    break;
  case 0x38:
    goto LAB_ram_2ad1;
  case 0x3c:
    goto LAB_ram_2ad2;
  case 0x44:
    goto code_r0x2ad4;
  case 0x48:
    pbVar14 = pbVar15 + 0x2a8f;
z80_candidate_2ad9:
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
z80_candidate_2adb:
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
    goto z80_candidate_2add;
  case 0x4c:
    pbVar14 = pbVar13;
z80_candidate_2add:
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
z80_candidate_2adf:
    pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
code_r0x2ae1:
    uVar2 = (short)pbVar13 << 8;
    uVar8 = (undefined1)((ushort)pbVar13 >> 8);
    pbVar13 = (byte *)CONCAT11(uVar8,uVar8);
    break;
  case 0x50:
    goto code_r0x2ae1;
  case 0x54:
    pbVar14 = pbVar15 + 0x2a8f;
z80_candidate_2ae6:
    pbVar14 = (byte *)((short)pbVar14 * 2 + (ushort)CARRY2((ushort)pbVar14,(ushort)pbVar14));
    goto LAB_ram_2ae9;
  case 0x58:
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),bVar3);
LAB_ram_2ae9:
    pbVar13 = (byte *)((short)pbVar14 * 2 + (ushort)CARRY2((ushort)pbVar14,(ushort)pbVar14));
LAB_ram_2aec:
    uVar2 = (short)pbVar13 << 8;
    uVar8 = (undefined1)((ushort)pbVar13 >> 8);
    pbVar13 = (byte *)CONCAT11(uVar8,uVar8);
    break;
  case 0x5c:
    goto LAB_ram_2aec;
  case 0x60:
    pbVar14 = pbVar15 + 0x2a8f;
z80_candidate_2af1:
    uVar2 = (ushort)(byte)(bVar4 >> 1 | (byte)pbVar14 << 7) << 8;
    pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
    goto z80_candidate_2af4;
  case 0x68:
    bVar4 = bVar4 - 1;
    pbVar14 = pbVar13;
    goto code_r0x2af9;
  case 0x6c:
    goto code_r0x2afc;
  case 0x74:
    goto code_r0x2b05;
  case 0x78:
    goto code_r0x2b08;
  case 0x7c:
    goto code_r0x2b0c;
  case 0x80:
    pbVar15 = (byte *)CONCAT11(1,bVar5);
    goto code_r0x2b11;
  case 0x84:
    goto code_r0x2b14;
  case 0x88:
    goto LAB_ram_2b18;
  case 0x8c:
    goto code_r0x2b1c;
  case 0x90:
    goto LAB_ram_2b20;
  case 0x94:
    goto code_r0x2b24;
  case 0x98:
    goto code_r0x2b28;
  case 0x9c:
    *pbVar12 = bVar4;
    goto code_r0x2b2d;
  case 0xa0:
    goto code_r0x2b30;
  case 0xa4:
    goto code_r0x2b34;
  case 0xa8:
    goto code_r0x2b38;
  case 0xac:
    *pbVar15 = bVar4;
    goto code_r0x2b3d;
  case 0xb0:
    *pbVar12 = bVar4;
    goto LAB_ram_2b3f;
  case 0xb4:
    goto code_r0x2b45;
  case 0xb8:
    goto code_r0x2b48;
  case 0xbc:
    pbVar15 = (byte *)0x0;
    goto code_r0x2b4d;
  case 0xc0:
    goto code_r0x2b50;
  case 0xc4:
    goto code_r0x2b54;
  case 200:
    goto code_r0x2b58;
  case 0xcc:
    bVar5 = RST7();
    uVar2 = (ushort)bVar5 << 8;
    goto LAB_ram_2b5a;
  case 0xd0:
    pbVar15 = (byte *)0x0;
    goto LAB_ram_2b5a;
  case 0xd4:
    goto code_r0x2b64;
  case 0xd8:
    goto code_r0x2b68;
  case 0xdc:
    *pbVar12 = bVar4;
    goto code_r0x2b6d;
  case 0xe0:
    goto code_r0x2b70;
  case 0xe4:
    goto code_r0x2b74;
  case 0xe8:
    goto code_r0x2b78;
  case 0xec:
    *pbVar15 = bVar4;
    goto code_r0x2b7d;
  case 0xf0:
    uVar2 = (ushort)(byte)(bVar4 - 8) << 8;
    goto LAB_ram_2b7f;
  case 0xf4:
    goto code_r0x2b84;
  case 0xf8:
    goto code_r0x2b88;
  case 0xfc:
    uVar2 = (ushort)*pbVar15 << 8;
    goto code_r0x2b88;
  }
  pbVar12 = pbVar13;
  pbVar15 = (byte *)(uVar2 & 0xff00);
  pbVar13 = (byte *)0xf8cb;
code_r0x2b05:
  bVar5 = *pbVar13;
  *pbVar13 = (byte)pbVar12;
  uVar2 = CONCAT11(bVar5,bVar5 < (byte)pbVar12);
code_r0x2b08:
  pbVar13 = pbVar12;
  if ((uVar2 & 1) == 0) {
LAB_ram_2b18:
    pbVar12 = (byte *)0x500;
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),4);
code_r0x2b1c:
    cVar6 = (char)((ushort)pbVar12 >> 8);
    z80_candidate_8(pbVar15);
    z80_candidate_8(CONCAT11(cVar6 + -1,(char)((ushort)pbVar15 >> 8)));
  }
  else {
code_r0x2b0c:
    uVar8 = (undefined1)((ushort)pbVar15 >> 8);
    pbVar12 = (byte *)CONCAT11(4,uVar8);
    pbVar15 = (byte *)CONCAT11(uVar8,4);
code_r0x2b11:
    z80_candidate_8(pbVar15,pbVar13,pbVar12);
    pbVar12 = (byte *)CONCAT11((char)((ushort)pbVar12 >> 8) + '\x01',(char)pbVar13);
code_r0x2b14:
    z80_candidate_8(pbVar12);
  }
LAB_ram_2b20:
  pbVar13 = (byte *)(ushort)DAT_ram_f811;
code_r0x2b24:
  uVar2 = CONCAT11(DAT_ram_f8c1,(DAT_ram_f8c1 == '\0') << 6);
code_r0x2b28:
  if (!(bool)((byte)uVar2 >> 6)) {
    bVar5 = (char)pbVar13 + 1;
    pbVar13 = (byte *)(ushort)bVar5;
    uVar2 = uVar2 & 0xff00;
    if (bVar5 != 0) {
code_r0x2b2d:
      bVar9 = (byte)(uVar2 >> 8);
      bVar5 = (char)pbVar13 - 1;
      pbVar13 = (byte *)(ushort)bVar5;
      uVar2 = CONCAT11(bVar5,(bVar5 == 0) << 6);
code_r0x2b30:
      if (!(bool)((byte)uVar2 >> 6)) {
        pbVar13 = (byte *)CONCAT11(6,(char)pbVar13);
code_r0x2b34:
        uVar2 = (ushort)*pbVar13 << 8;
        pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),bVar9);
code_r0x2b38:
        bVar5 = (byte)(uVar2 >> 8);
        bVar1 = CARRY1(bVar5,*pbVar13);
        pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),bVar5 + *pbVar13);
        uVar2 = CONCAT11(-bVar1,bVar1);
        if (bVar1) {
code_r0x2b3d:
          uVar2 = (ushort)*(byte *)CONCAT11((char)((ushort)pbVar13 >> 8) + '\x01',(char)pbVar13) <<
                  8;
        }
      }
    }
  }
LAB_ram_2b3f:
  pbVar13 = (byte *)(ushort)*(byte *)(sVar17 + 0x12);
  bVar4 = (byte)(uVar2 >> 8);
  uVar2 = (ushort)bVar4 << 8;
  if (bVar4 != 0) {
code_r0x2b45:
    bVar5 = (char)pbVar13 + 1;
    pbVar13 = (byte *)(ushort)bVar5;
    uVar2 = (ushort)bVar4 << 8;
    if (bVar5 != 0) {
code_r0x2b48:
      bVar9 = (byte)(uVar2 >> 8);
      bVar5 = (char)pbVar13 - 1;
      pbVar13 = (byte *)(ushort)bVar5;
      uVar2 = (ushort)bVar5 << 8;
      if (bVar5 != 0) {
code_r0x2b4d:
        pbVar13 = (byte *)CONCAT11(6,(char)pbVar13);
        uVar2 = (ushort)*pbVar13 << 8;
code_r0x2b50:
        pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),bVar9);
        bVar5 = (byte)(uVar2 >> 8);
        uVar2 = CONCAT11(bVar5 + *pbVar13,CARRY1(bVar5,*pbVar13));
code_r0x2b54:
        pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),(char)(uVar2 >> 8));
        bVar1 = (uVar2 & 1) != 0;
        uVar2 = CONCAT11(-(char)uVar2,bVar1);
        if (bVar1) {
code_r0x2b58:
          uVar2 = (ushort)*(byte *)CONCAT11((char)((ushort)pbVar13 >> 8) + '\x01',(char)pbVar13) <<
                  8;
        }
      }
    }
  }
LAB_ram_2b5a:
  pbVar13 = (byte *)(uVar2 >> 8);
  bVar5 = 0xff;
  if ((*(byte *)(sVar17 + 9) & 2) != 0) {
code_r0x2b64:
    bVar5 = DAT_ram_f805;
  }
  uVar2 = CONCAT11(bVar5,(bVar5 == 0) << 6);
code_r0x2b68:
  if (!(bool)((byte)uVar2 >> 6)) {
    bVar5 = (char)pbVar13 + 1;
    pbVar13 = (byte *)(ushort)bVar5;
    uVar2 = uVar2 & 0xff00;
    if (bVar5 != 0) {
code_r0x2b6d:
      bVar9 = (byte)(uVar2 >> 8);
      bVar5 = (char)pbVar13 - 1;
      pbVar13 = (byte *)(ushort)bVar5;
      uVar2 = CONCAT11(bVar5,(bVar5 == 0) << 6);
code_r0x2b70:
      if (!(bool)((byte)uVar2 >> 6)) {
        pbVar13 = (byte *)CONCAT11(6,(char)pbVar13);
code_r0x2b74:
        uVar2 = (ushort)*pbVar13 << 8;
        pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),bVar9);
code_r0x2b78:
        bVar5 = (byte)(uVar2 >> 8);
        bVar1 = CARRY1(bVar5,*pbVar13);
        pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8),bVar5 + *pbVar13);
        uVar2 = CONCAT11(-bVar1,bVar1);
        if (bVar1) {
code_r0x2b7d:
          uVar2 = (ushort)*(byte *)CONCAT11((char)((ushort)pbVar13 >> 8) + '\x01',(char)pbVar13) <<
                  8;
        }
      }
    }
  }
LAB_ram_2b7f:
  cVar6 = (char)(uVar2 >> 8);
  uVar2 = CONCAT11(cVar6,(cVar6 == cRamf8c6) << 6);
  cRamf8c6 = cVar6;
code_r0x2b84:
  uVar8 = (undefined1)((ushort)pbVar15 >> 8);
  if (!(bool)((byte)uVar2 >> 6)) {
    pbVar13 = (byte *)(uVar2 >> 8);
code_r0x2b88:
    uVar7 = CONCAT11((char)((ushort)pbVar15 >> 8),4);
    z80_candidate_8((char)(uVar2 >> 8),uVar7,CONCAT11(10,*(undefined1 *)CONCAT11(10,(char)pbVar13)))
    ;
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
  bVar5 = 1;
  if ((*(byte *)(sVar17 + 9) & 2) != 0) {
    bVar5 = DAT_ram_f809;
  }
  DAT_ram_f8bc = (DAT_ram_f8bc + bVar5) - CARRY1(DAT_ram_f8bc,bVar5);
  if (DAT_ram_f8bb == '\0') {
    if (((*(byte *)(sVar17 + 9) & 0x10) != 0) || ((bRamf8b9 & 0x10) == 0)) goto LAB_ram_2bd2;
    bRamf8b9 = bRamf8b9 & 0xef;
  }
  else {
    DAT_ram_f8bb = DAT_ram_f8bb - bVar5;
    if (DAT_ram_f8bb != '\0') goto LAB_ram_2bd2;
    if ((*(byte *)(sVar17 + 9) & 0x10) != 0) {
      puVar16[3] = puVar16[3] | 0x10;
      goto LAB_ram_2bd2;
    }
  }
  z80_candidate_21ca(0xf8cc);
LAB_ram_2bd2:
  bVar5 = bRamf904;
  bRamf904 = DAT_ram_f903;
  pbVar15 = &DAT_ram_f905;
  if (bVar5 != DAT_ram_f903) {
    uVar7 = CONCAT11(uVar8,4);
    z80_candidate_8(uVar7);
    uVar8 = (undefined1)((ushort)uVar7 >> 8);
  }
  bVar9 = *pbVar15;
  bVar5 = pbVar15[1];
  pbVar15[1] = bVar9;
  if (bVar5 == bVar9) {
    return;
  }
  z80_candidate_8(CONCAT11(uVar8,4),CONCAT11(7,(~bVar9 & 7) * '\b' | bVar9));
  return;
}
