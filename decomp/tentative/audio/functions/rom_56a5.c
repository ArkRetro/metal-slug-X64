/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x56a5; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_56a5(byte *param_1,undefined1 *param_2)

{
  char cVar1;
  byte bVar2;
  undefined1 uVar3;
  ushort uVar4;
  short sVar5;
  byte *pbVar6;
  char *pcVar7;

code_r0x56a5:
  param_2[10] = *param_1;
  _DAT_ram_ff11 = param_1 + 1;
LAB_ram_5332:
  pbVar6 = _DAT_ram_ff11;
  if (param_2[0x16] != '\0') {
    param_2[0x14] = (char)_DAT_ram_ff11;
    param_2[0x15] = (char)((ushort)pbVar6 >> 8);
    param_2[0x16] = param_2[0x16] + -1;
    if ((param_2[9] & 2) != 0) {
      return;
    }
    bVar2 = 0;
    if ((param_2[9] & 0x20) == 0) {
      return;
    }
    z80_candidate_4b09();
    if ((bool)(bVar2 & 1)) {
      return;
    }
    goto LAB_ram_577a;
  }
  if (DAT_ram_ff13 != param_2[4]) {
    DAT_ram_ff13 = param_2[4];
  }
  bVar2 = *_DAT_ram_ff11;
  param_1 = _DAT_ram_ff11 + 1;
  if (0xd9 < bVar2) goto SUB_ram_5781;
  if (bVar2 < 0xc0) goto LAB_ram_56de;
  uVar4 = (ushort)(byte)((bVar2 + 0x40) * '\x03');
  bVar2 = 0xac68 < uVar4;
  sVar5 = uVar4 + 0x5397;
                    /* WARNING: Switch is manually overridden */
  switch(sVar5) {
  case 0x5397:
    goto override_jmp_ram_5396_case_0;
  case 0x539a:
    param_2[10] = *param_1;
    param_2[0xb] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    goto LAB_ram_5332;
  case 0x539d:
    _DAT_ram_ff11 = _DAT_ram_ff11 + *(short *)param_1 + 3;
    param_2[0x17] = 0x40;
    param_2[0x18] = 0xc0;
    param_2[0x19] = 0xc0;
    param_2[0x1a] = 0x7f;
    goto LAB_ram_5332;
  case 0x53a0:
    if (*param_1 == 0) {
      param_2[9] = param_2[9] & 0xfe;
    }
    else {
      param_2[9] = param_2[9] | 1;
    }
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0x53a3:
    pbVar6 = _DAT_ram_ff11 + 3;
    _DAT_ram_ff11 = pbVar6 + *(short *)param_1;
    if (param_2[0x22] != '\0') {
      bVar2 = param_2[0x22] - 2;
      if (5 < bVar2) break;
      param_2[0x22] = bVar2;
      *(byte **)(param_2 + bVar2 + 0x1c) = pbVar6;
      param_2[0x17] = 0x40;
      param_2[0x18] = 0xc0;
      param_2[0x19] = 0xc0;
      param_2[0x1a] = 0x7f;
    }
    goto LAB_ram_5332;
  case 0x53a6:
    bVar2 = param_2[0x22];
    if (5 < bVar2) goto LAB_ram_5785;
    param_2[0x22] = bVar2 + 2;
    _DAT_ram_ff11 = *(byte **)(param_2 + bVar2 + 0x1c);
    param_2[0x17] = 0x40;
    param_2[0x18] = 0xc0;
    param_2[0x19] = 0xc0;
    param_2[0x1a] = 0x7f;
    goto LAB_ram_5332;
  case 0x53a9:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_2[0xc] = *param_1 & 0x7f;
    goto LAB_ram_5332;
  case 0x53ac:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_2[0xd] = *param_1 & 0x7f;
    goto LAB_ram_5332;
  case 0x53af:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_2[0xe] = *param_1 & 0x7f;
    goto LAB_ram_5332;
  case 0x53b2:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    if (*param_1 == 0) {
      param_2[9] = param_2[9] & 0xef;
    }
    else {
      param_2[9] = param_2[9] | 0x10;
    }
    goto LAB_ram_5332;
  case 0x53b5:
    sVar5 = *(short *)param_1;
    param_2[0x16] = _DAT_ram_ff11[3];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 4;
    uVar4 = sVar5 * 4;
    param_2[0x10] = (char)((ushort)(sVar5 * 8) >> 8);
    param_2[0x11] = -CARRY2(uVar4,uVar4);
    goto LAB_ram_5332;
  case 0x53b8:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_2[0x16] = *param_1;
    goto LAB_ram_5332;
  case 0x53bb:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    uVar3 = 3;
    if ((*param_1 != 0x40) && (uVar3 = 2, 0x3f < *param_1)) {
      uVar3 = 1;
    }
    param_2[0xf] = uVar3;
    goto LAB_ram_5332;
  case 0x53be:
    pbVar6 = _DAT_ram_ff11 + 2;
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4f60(*pbVar6,sVar5);
    goto LAB_ram_5332;
  case 0x53c1:
    param_2[0x12] = *param_1 * '\x02' + (0x7f < (byte)(*param_1 * '\x02'));
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0x53c4:
    bVar2 = param_2[0x22] - 1;
    _DAT_ram_ff11 = param_1;
    if (bVar2 < 6) {
      param_2[0x22] = bVar2;
      param_2[bVar2 + 0x1c] = 0;
      param_2[0x17] = 0x40;
      param_2[0x18] = 0xc0;
      param_2[0x19] = 0xc0;
      param_2[0x1a] = 0x7f;
      goto LAB_ram_5332;
    }
    break;
  case 0x53c7:
    if ((byte)param_2[0x22] < 6) {
      pcVar7 = param_2 + (byte)param_2[0x22] + 0x1c;
      if (*pcVar7 == '\0') {
        if (*param_1 != 0) {
          cVar1 = *param_1 - 1;
          if (cVar1 == '\0') goto LAB_ram_563c;
          *pcVar7 = cVar1;
        }
      }
      else {
        cVar1 = *pcVar7 + -1;
        if (cVar1 == '\0') {
LAB_ram_563c:
          if (5 < (byte)param_2[0x22]) break;
          param_2[0x22] = param_2[0x22] + 1;
          _DAT_ram_ff11 = _DAT_ram_ff11 + 4;
          goto LAB_ram_5332;
        }
        *pcVar7 = cVar1;
      }
      _DAT_ram_ff11 = _DAT_ram_ff11 + *(short *)(_DAT_ram_ff11 + 2) + 4;
      param_2[0x17] = 0x40;
      param_2[0x18] = 0xc0;
      param_2[0x19] = 0xc0;
      param_2[0x1a] = 0x7f;
      goto LAB_ram_5332;
    }
    break;
  case 0x53ca:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    if (*param_1 < 0x40) {
      param_2[9] = param_2[9] & 0xbf;
      param_2[0x10] = 0;
      param_2[0x11] = 0;
    }
    else {
      bVar2 = param_2[9];
      param_2[9] = param_2[9] | 0x40;
      if ((bVar2 & 0x40) == 0) {
        param_2[0x1b] = 0xff;
      }
    }
    goto LAB_ram_5332;
  case 0x53cd:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_2[0x13] = *param_1;
    goto LAB_ram_5332;
  case 0x53d0:
    param_2[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b32(sVar5);
    goto joined_r0x544a;
  case 0x53d3:
    param_2[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b4d(sVar5);
    goto joined_r0x544a;
  case 0x53d6:
    param_2[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b68(sVar5);
joined_r0x544a:
    if ((bVar2 & 1) == 0) goto LAB_ram_577a;
    goto LAB_ram_5332;
  case 0x53d9:
    param_2[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b76(sVar5);
    goto joined_r0x5774;
  case 0x53dc:
    bVar2 = *param_1;
    param_2[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    sVar5 = (*(short *)(param_2 + 0x10) - 0x1000U) + (ushort)bVar2;
    if (CARRY2(*(short *)(param_2 + 0x10) - 0x1000U,(ushort)bVar2)) {
      sVar5 = -1;
    }
    *(short *)(param_2 + 0x10) = sVar5 + 0x1000;
    goto LAB_ram_5332;
  case 0x53df:
    bVar2 = *param_1;
    param_2[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    sVar5 = (*(short *)(param_2 + 0x10) + 0x1000U) - (ushort)bVar2;
    if (*(short *)(param_2 + 0x10) + 0x1000U < (ushort)bVar2) {
      sVar5 = 0;
    }
    *(short *)(param_2 + 0x10) = sVar5 + -0x1000;
    goto LAB_ram_5332;
  case 0x53e2:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    DAT_ram_ff10 = DAT_ram_ff10 + 1;
    bIo000c = DAT_ram_ff10 & 1 | (*param_1 & 0x3f) * '\x02' + 0x80;
    goto LAB_ram_5332;
  }
  goto SUB_ram_5781;
override_jmp_ram_5396_case_0:
  goto code_r0x56a5;
LAB_ram_56de:
  if ((bVar2 & 0xf8) == 0xb8) {
    DAT_ram_fbf9 = *param_1;
    param_1 = _DAT_ram_ff11 + 2;
  }
  else {
    DAT_ram_fbf9 = ((bVar2 >> 3) - 0xb) + param_2[0x17];
  }
  param_2[0x17] = DAT_ram_fbf9;
  DAT_ram_fbfa = param_2[0x18];
  if (!(bool)(bVar2 & 1)) {
    DAT_ram_fbfa = *param_1;
    param_1 = param_1 + 1;
    param_2[0x18] = DAT_ram_fbfa;
  }
  param_2[0x16] = DAT_ram_fbfa;
  DAT_ram_fbfb = param_2[0x19];
  if (!(bool)(bVar2 >> 1 & 1)) {
    DAT_ram_fbfb = *param_1;
    param_1 = param_1 + 1;
    param_2[0x19] = DAT_ram_fbfb;
  }
  DAT_ram_fbfc = param_2[0x1a];
  if (!(bool)(bVar2 >> 2 & 1)) {
    DAT_ram_fbfc = *param_1;
    param_1 = param_1 + 1;
    param_2[0x1a] = DAT_ram_fbfc;
  }
  _DAT_ram_ff11 = param_1;
  if (((param_2[9] & 0x40) != 0) && (bVar2 = param_2[0x1b], bVar2 != 0xff)) {
    *(short *)(param_2 + 0x10) =
         (CONCAT11(*(undefined1 *)CONCAT11(1,bVar2 | 0x80),*(undefined1 *)CONCAT11(1,bVar2)) -
         CONCAT11(*(undefined1 *)CONCAT11(1,param_2[0x17] | 0x80),
                  *(undefined1 *)CONCAT11(1,param_2[0x17]))) + *(short *)(param_2 + 0x10);
  }
  param_2[0x1b] = param_2[0x17];
  bVar2 = 0;
  if ((DAT_ram_fbfb == 0) || (DAT_ram_fbfb == 0)) goto LAB_ram_5332;
  z80_candidate_4a2a(DAT_ram_fbfc);
joined_r0x5774:
  if ((bVar2 & 1) != 0) goto LAB_ram_5332;
LAB_ram_577a:
  if ((param_2[9] & 2) != 0) goto LAB_ram_5332;
SUB_ram_5781:
  param_2[0x12] = 0;
LAB_ram_5785:
  param_2[9] = 8;
  *param_2 = 0;
  param_2[1] = 0;
  param_2[2] = 0xff;
  return;
}
