/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x28b8; tentative_c_with_diagnostics. */

/* WARNING: Instruction at (ram,0x2b8c) overlaps instruction at (ram,0x2b8b)
    */
/* WARNING: Removing unreachable block (ram,0x2b04) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_28b8(ushort param_1,short param_2,short param_3)

{
  bool bVar1;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  char cVar6;
  byte bVar7;
  ushort uVar2;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined1 uVar10;
  undefined2 uVar11;
  short sVar12;
  byte *pbVar13;
  byte *pbVar14;
  byte *pbVar15;
  byte *pbVar16;
  undefined2 unaff_AF_;
  undefined *puVar17;
  short sVar18;

  bVar7 = (byte)((ushort)unaff_AF_ >> 8);
  uVar2 = param_1 * 2 + (ushort)CARRY2(param_1,param_1);
  sVar18 = uVar2 * 2 + (ushort)CARRY2(uVar2,uVar2);
  bVar4 = (byte)((ushort)sVar18 >> 8);
  uVar9 = CONCAT11(bVar4,bVar4);
  uVar10 = (undefined1)sVar18;
  if (bRamf87e < bVar4) {
    uVar11 = CONCAT11(2,uVar10);
    uVar8 = CONCAT11(uVar10,4);
    bRamf87e = bVar4;
    z80_candidate_8(uVar8,uVar9,uVar11);
    uVar10 = (undefined1)((ushort)uVar8 >> 8);
    z80_candidate_8(CONCAT11((char)((ushort)uVar11 >> 8) + '\x01',(char)uVar9));
  }
  else {
    cVar6 = '\x03';
    uVar9 = CONCAT11(uVar10,4);
    bRamf87e = bVar4;
    z80_candidate_8(uVar9);
    uVar10 = (undefined1)((ushort)uVar9 >> 8);
    z80_candidate_8(CONCAT11(cVar6 + -1,uVar10));
  }
  bVar4 = DAT_ram_f874;
  if (((DAT_ram_f874 != 0) && (DAT_ram_f811 != 0xff)) &&
     (bVar4 = DAT_ram_f811, bVar7 = DAT_ram_f874, DAT_ram_f811 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar5 = *(byte *)CONCAT11(6,DAT_ram_f874);
    bVar4 = -CARRY1(bVar3,bVar5);
    if (CARRY1(bVar3,bVar5)) {
      bVar4 = *(byte *)CONCAT11(7,bVar3 + bVar5);
    }
  }
  bVar5 = *(byte *)(param_2 + 0x12);
  bVar3 = bVar4;
  if (((bVar4 != 0) && (bVar5 != 0xff)) && (bVar3 = bVar5, bVar7 = bVar4, bVar5 != 0)) {
    bVar5 = *(byte *)CONCAT11(6,bVar5);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar3 = -CARRY1(bVar5,bVar4);
    if (CARRY1(bVar5,bVar4)) {
      bVar3 = *(byte *)CONCAT11(7,bVar5 + bVar4);
    }
  }
  bVar4 = 0xff;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar4 = DAT_ram_f805;
  }
  bVar5 = bVar4;
  if (((bVar4 != 0) && (bVar3 != 0xff)) && (bVar5 = bVar3, bVar7 = bVar4, bVar3 != 0)) {
    bVar3 = *(byte *)CONCAT11(6,bVar3);
    bVar4 = *(byte *)CONCAT11(6,bVar4);
    bVar5 = -CARRY1(bVar3,bVar4);
    if (CARRY1(bVar3,bVar4)) {
      bVar5 = *(byte *)CONCAT11(7,bVar3 + bVar4);
    }
  }
  bVar1 = bVar5 != bRamf879;
  bRamf879 = bVar5;
  if (bVar1) {
    uVar9 = CONCAT11(uVar10,4);
    z80_candidate_8(uVar9,CONCAT11(9,*(undefined1 *)CONCAT11(10,bVar5)));
    uVar10 = (undefined1)((ushort)uVar9 >> 8);
  }
  bVar4 = 1;
  if ((*(byte *)(param_2 + 9) & 2) != 0) {
    bVar4 = DAT_ram_f809;
  }
  pbVar16 = (byte *)CONCAT11(uVar10,bVar4);
  DAT_ram_f86f = (DAT_ram_f86f + bVar4) - CARRY1(DAT_ram_f86f,bVar4);
  if (DAT_ram_f86e == '\0') {
    if (((*(byte *)(param_2 + 9) & 0x10) == 0) && ((bRamf86c & 0x10) != 0)) {
      bRamf86c = bRamf86c & 0xef;
LAB_ram_299e:
      z80_candidate_21ca(0xf87f);
    }
  }
  else {
    DAT_ram_f86e = DAT_ram_f86e - bVar4;
    if (DAT_ram_f86e == '\0') {
      if ((*(byte *)(param_2 + 9) & 0x10) == 0) goto LAB_ram_299e;
      *(byte *)(param_3 + 3) = *(byte *)(param_3 + 3) | 0x10;
    }
  }
  puVar17 = &DAT_ram_f8b6;
  sVar18 = _DAT_ram_f8b6;
  if ((*(byte *)(_DAT_ram_f8b6 + 9) & 8) != 0) {
    z80_candidate_1fcc();
  }
  DAT_ram_f811 = z80_candidate_21de(0xf8cc);
  if (DAT_ram_f811 == 0) goto LAB_ram_2b20;
  cVar6 = z80_candidate_22cf(0xf8da);
  cVar6 = cVar6 - (char)pbVar16;
  bVar4 = cVar6 - 1;
  bVar5 = DAT_ram_f811;
  if (((DAT_ram_f811 != 0) && (cVar6 != '\0')) && (bVar5 = bVar4, bVar7 = DAT_ram_f811, bVar4 != 0))
  {
    bVar3 = *(byte *)CONCAT11(6,bVar4);
    bVar4 = *(byte *)CONCAT11(6,DAT_ram_f811);
    bVar5 = -CARRY1(bVar3,bVar4);
    if (CARRY1(bVar3,bVar4)) {
      bVar5 = *(byte *)CONCAT11(7,bVar3 + bVar4);
    }
  }
  DAT_ram_f811 = bVar5;
  if ((DAT_ram_f811 == 0) || (DAT_ram_f8c1 == '\0')) goto LAB_ram_2b20;
  if (DAT_ram_f8c9 != '\0') {
    bVar7 = z80_candidate_22cf(0xf8e8);
    sVar12 = (ushort)bVar7 * 2 - ((ushort)pbVar16 & 0xff);
    cVar6 = (char)((ushort)sVar12 >> 8);
    bVar7 = (byte)((byte)((byte)sVar12 >> 1 | cVar6 << 7) >> 1 | (cVar6 >> 1) << 7) >> 1 |
            (cVar6 >> 2) << 7;
    pbVar16 = (byte *)((ushort)bVar7 << 8);
    DAT_ram_f903 = (bRamf8be & 0x1f) + bVar7;
    if ((char)DAT_ram_f903 < '\0') {
      DAT_ram_f903 = 0;
    }
    if (0x1f < DAT_ram_f903) {
      DAT_ram_f903 = 0x1f;
    }
    goto LAB_ram_2b20;
  }
  _DAT_ram_f80f = _DAT_ram_f8bf + _DAT_ram_f8c7 + *(short *)(sVar18 + 0x10);
  bVar4 = z80_candidate_22cf(0xf8e8);
  pbVar16 = (byte *)((ushort)pbVar16 & 0xff);
  _DAT_ram_f80f = ((ushort)bVar4 * 2 - (short)pbVar16) + _DAT_ram_f80f;
  sVar12 = -0x70a;
  z80_candidate_243f();
  pbVar13 = (byte *)(sVar12 + _DAT_ram_f80f);
  if (_DAT_ram_f8c4 == pbVar13) goto LAB_ram_2b20;
  bVar5 = (byte)((ushort)pbVar13 >> 8);
  bVar4 = bVar5;
  if (0xf < bVar5) {
    bVar4 = 0xf;
  }
  bVar4 = bVar4 << 2;
  pbVar16 = (byte *)(ushort)bVar4;
  bVar3 = *(byte *)CONCAT11(2,(char)pbVar13);
  pbVar15 = (byte *)(ushort)*(byte *)CONCAT11(3,(char)pbVar13);
  bVar1 = (byte *)0xd56f < pbVar16;
  uVar2 = CONCAT11(bVar3,bVar1);
  pbVar14 = pbVar16 + 0x2a90;
  _DAT_ram_f8c4 = pbVar13;
  switch(bVar4) {
  case 0:
    pbVar14 = pbVar15;
    break;
  case 4:
    goto z80_candidate_2afd;
  case 8:
    goto z80_candidate_2afa;
  case 0xc:
    goto z80_candidate_2af7;
  case 0x10:
    pbVar14 = pbVar15;
  case 100:
z80_candidate_2af4:
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(byte)pbVar14 >> 1);
    uVar2 = (ushort)(byte)((byte)(uVar2 >> 9) | (byte)pbVar14 << 7) << 8;
z80_candidate_2af7:
    bVar1 = (bool)((byte)pbVar15 & 1);
    bVar3 = (byte)(uVar2 >> 8);
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
code_r0x2af9:
    uVar2 = (ushort)(byte)(bVar3 >> 1 | bVar1 << 7) << 8;
z80_candidate_2afa:
    uVar2 = CONCAT11((char)(uVar2 >> 8),(byte)pbVar15) & 0xff01;
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
code_r0x2afc:
    pbVar15 = pbVar14;
    uVar2 = (ushort)(byte)((byte)(uVar2 >> 9) | (char)uVar2 << 7) << 8;
z80_candidate_2afd:
    uVar2 = (ushort)(byte)((byte)(uVar2 >> 9) | (byte)pbVar15 << 7) << 8;
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
    break;
  case 0x14:
    goto z80_candidate_2af1;
  case 0x18:
    goto z80_candidate_2ae6;
  case 0x1c:
    goto LAB_ram_2ae9;
  case 0x20:
    pbVar14 = pbVar15;
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
    pbVar14 = pbVar15;
  case 0x40:
    pbVar15 = (byte *)((short)pbVar14 * 2);
LAB_ram_2ad1:
    pbVar15 = (byte *)((short)pbVar15 * 2);
LAB_ram_2ad2:
    pbVar14 = (byte *)((short)pbVar15 * 2);
    uVar2 = (ushort)pbVar14 & 0xff00;
code_r0x2ad4:
    pbVar14 = (byte *)((ushort)pbVar14 & 0xff00);
    break;
  case 0x38:
    goto LAB_ram_2ad1;
  case 0x3c:
    goto LAB_ram_2ad2;
  case 0x44:
    goto code_r0x2ad4;
  case 0x48:
    pbVar15 = pbVar16 + 0x2a8f;
z80_candidate_2ad9:
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
z80_candidate_2adb:
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
    goto z80_candidate_2add;
  case 0x4c:
    pbVar15 = pbVar14;
z80_candidate_2add:
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
z80_candidate_2adf:
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
code_r0x2ae1:
    uVar2 = (short)pbVar14 << 8;
    uVar10 = (undefined1)((ushort)pbVar14 >> 8);
    pbVar14 = (byte *)CONCAT11(uVar10,uVar10);
    break;
  case 0x50:
    goto code_r0x2ae1;
  case 0x54:
    pbVar15 = pbVar16 + 0x2a8f;
z80_candidate_2ae6:
    pbVar15 = (byte *)((short)pbVar15 * 2 + (ushort)CARRY2((ushort)pbVar15,(ushort)pbVar15));
    goto LAB_ram_2ae9;
  case 0x58:
    pbVar15 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),bVar5);
LAB_ram_2ae9:
    pbVar14 = (byte *)((short)pbVar15 * 2 + (ushort)CARRY2((ushort)pbVar15,(ushort)pbVar15));
LAB_ram_2aec:
    uVar2 = (short)pbVar14 << 8;
    uVar10 = (undefined1)((ushort)pbVar14 >> 8);
    pbVar14 = (byte *)CONCAT11(uVar10,uVar10);
    break;
  case 0x5c:
    goto LAB_ram_2aec;
  case 0x60:
    pbVar15 = pbVar16 + 0x2a8f;
z80_candidate_2af1:
    uVar2 = (ushort)(byte)(bVar3 >> 1 | (byte)pbVar15 << 7) << 8;
    pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar15 >> 8),(byte)pbVar15 >> 1);
    goto z80_candidate_2af4;
  case 0x68:
    bVar3 = bVar3 - 1;
    pbVar15 = pbVar14;
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
    pbVar16 = (byte *)CONCAT11(1,bVar4);
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
    *pbVar13 = bVar3;
    goto code_r0x2b2d;
  case 0xa0:
    goto code_r0x2b30;
  case 0xa4:
    goto code_r0x2b34;
  case 0xa8:
    goto code_r0x2b38;
  case 0xac:
    *pbVar16 = bVar3;
    goto code_r0x2b3d;
  case 0xb0:
    *pbVar13 = bVar3;
    goto LAB_ram_2b3f;
  case 0xb4:
    goto code_r0x2b45;
  case 0xb8:
    goto code_r0x2b48;
  case 0xbc:
    pbVar16 = (byte *)0x0;
    goto code_r0x2b4d;
  case 0xc0:
    goto code_r0x2b50;
  case 0xc4:
    goto code_r0x2b54;
  case 200:
    goto code_r0x2b58;
  case 0xcc:
    bVar7 = RST7();
    uVar2 = (ushort)bVar7 << 8;
    goto LAB_ram_2b5a;
  case 0xd0:
    pbVar16 = (byte *)0x0;
    goto LAB_ram_2b5a;
  case 0xd4:
    goto code_r0x2b64;
  case 0xd8:
    goto code_r0x2b68;
  case 0xdc:
    *pbVar13 = bVar3;
    goto code_r0x2b6d;
  case 0xe0:
    goto code_r0x2b70;
  case 0xe4:
    goto code_r0x2b74;
  case 0xe8:
    goto code_r0x2b78;
  case 0xec:
    *pbVar16 = bVar3;
    goto code_r0x2b7d;
  case 0xf0:
    uVar2 = (ushort)(byte)(bVar3 - 8) << 8;
    goto LAB_ram_2b7f;
  case 0xf4:
    goto code_r0x2b84;
  case 0xf8:
    goto code_r0x2b88;
  case 0xfc:
    uVar2 = (ushort)*pbVar16 << 8;
    goto code_r0x2b88;
  }
  pbVar13 = pbVar14;
  pbVar16 = (byte *)(uVar2 & 0xff00);
  pbVar14 = (byte *)0xf8cb;
code_r0x2b05:
  bVar7 = *pbVar14;
  *pbVar14 = (byte)pbVar13;
  uVar2 = CONCAT11(bVar7,bVar7 < (byte)pbVar13);
code_r0x2b08:
  pbVar14 = pbVar13;
  if ((uVar2 & 1) == 0) {
LAB_ram_2b18:
    pbVar13 = (byte *)0x500;
    pbVar16 = (byte *)CONCAT11((char)((ushort)pbVar16 >> 8),4);
code_r0x2b1c:
    cVar6 = (char)((ushort)pbVar13 >> 8);
    z80_candidate_8(pbVar16);
    z80_candidate_8(CONCAT11(cVar6 + -1,(char)((ushort)pbVar16 >> 8)));
  }
  else {
code_r0x2b0c:
    uVar10 = (undefined1)((ushort)pbVar16 >> 8);
    pbVar13 = (byte *)CONCAT11(4,uVar10);
    pbVar16 = (byte *)CONCAT11(uVar10,4);
code_r0x2b11:
    z80_candidate_8(pbVar16,pbVar14,pbVar13);
    pbVar13 = (byte *)CONCAT11((char)((ushort)pbVar13 >> 8) + '\x01',(char)pbVar14);
code_r0x2b14:
    z80_candidate_8(pbVar13);
  }
LAB_ram_2b20:
  pbVar14 = (byte *)(ushort)DAT_ram_f811;
code_r0x2b24:
  uVar2 = CONCAT11(DAT_ram_f8c1,(DAT_ram_f8c1 == '\0') << 6);
code_r0x2b28:
  if (!(bool)((byte)uVar2 >> 6)) {
    bVar7 = (char)pbVar14 + 1;
    pbVar14 = (byte *)(ushort)bVar7;
    uVar2 = uVar2 & 0xff00;
    if (bVar7 != 0) {
code_r0x2b2d:
      bVar7 = (byte)(uVar2 >> 8);
      bVar4 = (char)pbVar14 - 1;
      pbVar14 = (byte *)(ushort)bVar4;
      uVar2 = CONCAT11(bVar4,(bVar4 == 0) << 6);
code_r0x2b30:
      if (!(bool)((byte)uVar2 >> 6)) {
        pbVar14 = (byte *)CONCAT11(6,(char)pbVar14);
code_r0x2b34:
        uVar2 = (ushort)*pbVar14 << 8;
        pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),bVar7);
code_r0x2b38:
        bVar7 = (byte)(uVar2 >> 8);
        bVar1 = CARRY1(bVar7,*pbVar14);
        pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),bVar7 + *pbVar14);
        uVar2 = CONCAT11(-bVar1,bVar1);
        if (bVar1) {
code_r0x2b3d:
          uVar2 = (ushort)*(byte *)CONCAT11((char)((ushort)pbVar14 >> 8) + '\x01',(char)pbVar14) <<
                  8;
        }
      }
    }
  }
LAB_ram_2b3f:
  pbVar14 = (byte *)(ushort)*(byte *)(sVar18 + 0x12);
  bVar3 = (byte)(uVar2 >> 8);
  uVar2 = (ushort)bVar3 << 8;
  if (bVar3 != 0) {
code_r0x2b45:
    bVar7 = (char)pbVar14 + 1;
    pbVar14 = (byte *)(ushort)bVar7;
    uVar2 = (ushort)bVar3 << 8;
    if (bVar7 != 0) {
code_r0x2b48:
      bVar7 = (byte)(uVar2 >> 8);
      bVar4 = (char)pbVar14 - 1;
      pbVar14 = (byte *)(ushort)bVar4;
      uVar2 = (ushort)bVar4 << 8;
      if (bVar4 != 0) {
code_r0x2b4d:
        pbVar14 = (byte *)CONCAT11(6,(char)pbVar14);
        uVar2 = (ushort)*pbVar14 << 8;
code_r0x2b50:
        pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),bVar7);
        bVar7 = (byte)(uVar2 >> 8);
        uVar2 = CONCAT11(bVar7 + *pbVar14,CARRY1(bVar7,*pbVar14));
code_r0x2b54:
        pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),(char)(uVar2 >> 8));
        bVar1 = (uVar2 & 1) != 0;
        uVar2 = CONCAT11(-(char)uVar2,bVar1);
        if (bVar1) {
code_r0x2b58:
          uVar2 = (ushort)*(byte *)CONCAT11((char)((ushort)pbVar14 >> 8) + '\x01',(char)pbVar14) <<
                  8;
        }
      }
    }
  }
LAB_ram_2b5a:
  pbVar14 = (byte *)(uVar2 >> 8);
  bVar7 = 0xff;
  if ((*(byte *)(sVar18 + 9) & 2) != 0) {
code_r0x2b64:
    bVar7 = DAT_ram_f805;
  }
  uVar2 = CONCAT11(bVar7,(bVar7 == 0) << 6);
code_r0x2b68:
  if (!(bool)((byte)uVar2 >> 6)) {
    bVar7 = (char)pbVar14 + 1;
    pbVar14 = (byte *)(ushort)bVar7;
    uVar2 = uVar2 & 0xff00;
    if (bVar7 != 0) {
code_r0x2b6d:
      bVar7 = (byte)(uVar2 >> 8);
      bVar4 = (char)pbVar14 - 1;
      pbVar14 = (byte *)(ushort)bVar4;
      uVar2 = CONCAT11(bVar4,(bVar4 == 0) << 6);
code_r0x2b70:
      if (!(bool)((byte)uVar2 >> 6)) {
        pbVar14 = (byte *)CONCAT11(6,(char)pbVar14);
code_r0x2b74:
        uVar2 = (ushort)*pbVar14 << 8;
        pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),bVar7);
code_r0x2b78:
        bVar7 = (byte)(uVar2 >> 8);
        bVar1 = CARRY1(bVar7,*pbVar14);
        pbVar14 = (byte *)CONCAT11((char)((ushort)pbVar14 >> 8),bVar7 + *pbVar14);
        uVar2 = CONCAT11(-bVar1,bVar1);
        if (bVar1) {
code_r0x2b7d:
          uVar2 = (ushort)*(byte *)CONCAT11((char)((ushort)pbVar14 >> 8) + '\x01',(char)pbVar14) <<
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
  uVar10 = (undefined1)((ushort)pbVar16 >> 8);
  if (!(bool)((byte)uVar2 >> 6)) {
    pbVar14 = (byte *)(uVar2 >> 8);
code_r0x2b88:
    uVar9 = CONCAT11((char)((ushort)pbVar16 >> 8),4);
    z80_candidate_8((char)(uVar2 >> 8),uVar9,CONCAT11(10,*(undefined1 *)CONCAT11(10,(char)pbVar14)))
    ;
    uVar10 = (undefined1)((ushort)uVar9 >> 8);
  }
  bVar7 = 1;
  if ((*(byte *)(sVar18 + 9) & 2) != 0) {
    bVar7 = DAT_ram_f809;
  }
  DAT_ram_f8bc = (DAT_ram_f8bc + bVar7) - CARRY1(DAT_ram_f8bc,bVar7);
  if (DAT_ram_f8bb == '\0') {
    if (((*(byte *)(sVar18 + 9) & 0x10) != 0) || ((bRamf8b9 & 0x10) == 0)) goto LAB_ram_2bd2;
    bRamf8b9 = bRamf8b9 & 0xef;
  }
  else {
    DAT_ram_f8bb = DAT_ram_f8bb - bVar7;
    if (DAT_ram_f8bb != '\0') goto LAB_ram_2bd2;
    if ((*(byte *)(sVar18 + 9) & 0x10) != 0) {
      puVar17[3] = puVar17[3] | 0x10;
      goto LAB_ram_2bd2;
    }
  }
  z80_candidate_21ca(0xf8cc);
LAB_ram_2bd2:
  bVar7 = bRamf904;
  bRamf904 = DAT_ram_f903;
  pbVar16 = &DAT_ram_f905;
  if (bVar7 != DAT_ram_f903) {
    uVar9 = CONCAT11(uVar10,4);
    z80_candidate_8(uVar9);
    uVar10 = (undefined1)((ushort)uVar9 >> 8);
  }
  bVar4 = *pbVar16;
  bVar7 = pbVar16[1];
  pbVar16[1] = bVar4;
  if (bVar7 == bVar4) {
    return;
  }
  z80_candidate_8(CONCAT11(uVar10,4),CONCAT11(7,(~bVar4 & 7) * '\b' | bVar4));
  return;
}
