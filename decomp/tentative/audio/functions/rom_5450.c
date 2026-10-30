/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x5450; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_5450(short param_1,byte *param_2,undefined1 *param_3)

{
  char cVar1;
  byte in_F;
  byte bVar2;
  undefined1 uVar3;
  ushort uVar4;
  short sVar5;
  byte *pbVar6;
  char *pcVar7;

code_r0x5450:
  param_3[0x16] = param_2[1];
  _DAT_ram_ff11 = param_2 + 2;
  z80_candidate_4b76(param_1);
joined_r0x545e:
  if ((in_F & 1) == 0) goto LAB_ram_577a;
LAB_ram_5332:
  pbVar6 = _DAT_ram_ff11;
  if (param_3[0x16] != '\0') {
    param_3[0x14] = (char)_DAT_ram_ff11;
    param_3[0x15] = (char)((ushort)pbVar6 >> 8);
    param_3[0x16] = param_3[0x16] + -1;
    if ((param_3[9] & 2) != 0) {
      return;
    }
    bVar2 = 0;
    if ((param_3[9] & 0x20) == 0) {
      return;
    }
    z80_candidate_4b09();
    if ((bool)(bVar2 & 1)) {
      return;
    }
    goto LAB_ram_577a;
  }
  if (DAT_ram_ff13 != param_3[4]) {
    DAT_ram_ff13 = param_3[4];
  }
  bVar2 = *_DAT_ram_ff11;
  param_2 = _DAT_ram_ff11 + 1;
  if (0xd9 < bVar2) goto SUB_ram_5781;
  if (bVar2 < 0xc0) goto LAB_ram_56de;
  uVar4 = (ushort)(byte)((bVar2 + 0x40) * '\x03');
  in_F = 0xac68 < uVar4;
  param_1 = uVar4 + 0x5397;
  switch(bVar2) {
  case 0xc0:
    param_3[10] = *param_2;
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0xc1:
    param_3[10] = *param_2;
    param_3[0xb] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    goto LAB_ram_5332;
  case 0xc2:
    _DAT_ram_ff11 = _DAT_ram_ff11 + *(short *)param_2 + 3;
    param_3[0x17] = 0x40;
    param_3[0x18] = 0xc0;
    param_3[0x19] = 0xc0;
    param_3[0x1a] = 0x7f;
    goto LAB_ram_5332;
  case 0xc3:
    if (*param_2 == 0) {
      param_3[9] = param_3[9] & 0xfe;
    }
    else {
      param_3[9] = param_3[9] | 1;
    }
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0xc4:
    pbVar6 = _DAT_ram_ff11 + 3;
    _DAT_ram_ff11 = pbVar6 + *(short *)param_2;
    if (param_3[0x22] != '\0') {
      bVar2 = param_3[0x22] - 2;
      if (5 < bVar2) break;
      param_3[0x22] = bVar2;
      *(byte **)(param_3 + bVar2 + 0x1c) = pbVar6;
      param_3[0x17] = 0x40;
      param_3[0x18] = 0xc0;
      param_3[0x19] = 0xc0;
      param_3[0x1a] = 0x7f;
    }
    goto LAB_ram_5332;
  case 0xc5:
    bVar2 = param_3[0x22];
    if (5 < bVar2) goto LAB_ram_5785;
    param_3[0x22] = bVar2 + 2;
    _DAT_ram_ff11 = *(byte **)(param_3 + bVar2 + 0x1c);
    param_3[0x17] = 0x40;
    param_3[0x18] = 0xc0;
    param_3[0x19] = 0xc0;
    param_3[0x1a] = 0x7f;
    goto LAB_ram_5332;
  case 0xc6:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_3[0xc] = *param_2 & 0x7f;
    goto LAB_ram_5332;
  case 199:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_3[0xd] = *param_2 & 0x7f;
    goto LAB_ram_5332;
  case 200:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_3[0xe] = *param_2 & 0x7f;
    goto LAB_ram_5332;
  case 0xc9:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    if (*param_2 == 0) {
      param_3[9] = param_3[9] & 0xef;
    }
    else {
      param_3[9] = param_3[9] | 0x10;
    }
    goto LAB_ram_5332;
  case 0xca:
    sVar5 = *(short *)param_2;
    param_3[0x16] = _DAT_ram_ff11[3];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 4;
    uVar4 = sVar5 * 4;
    param_3[0x10] = (char)((ushort)(sVar5 * 8) >> 8);
    param_3[0x11] = -CARRY2(uVar4,uVar4);
    goto LAB_ram_5332;
  case 0xcb:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_3[0x16] = *param_2;
    goto LAB_ram_5332;
  case 0xcc:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    uVar3 = 3;
    if ((*param_2 != 0x40) && (uVar3 = 2, 0x3f < *param_2)) {
      uVar3 = 1;
    }
    param_3[0xf] = uVar3;
    goto LAB_ram_5332;
  case 0xcd:
    pbVar6 = _DAT_ram_ff11 + 2;
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4f60(*pbVar6,param_1);
    goto LAB_ram_5332;
  case 0xce:
    param_3[0x12] = *param_2 * '\x02' + (0x7f < (byte)(*param_2 * '\x02'));
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0xcf:
    bVar2 = param_3[0x22] - 1;
    _DAT_ram_ff11 = param_2;
    if (bVar2 < 6) {
      param_3[0x22] = bVar2;
      param_3[bVar2 + 0x1c] = 0;
      param_3[0x17] = 0x40;
      param_3[0x18] = 0xc0;
      param_3[0x19] = 0xc0;
      param_3[0x1a] = 0x7f;
      goto LAB_ram_5332;
    }
    break;
  case 0xd0:
    if ((byte)param_3[0x22] < 6) {
      pcVar7 = param_3 + (byte)param_3[0x22] + 0x1c;
      if (*pcVar7 == '\0') {
        if (*param_2 != 0) {
          cVar1 = *param_2 - 1;
          if (cVar1 == '\0') goto LAB_ram_563c;
          *pcVar7 = cVar1;
        }
      }
      else {
        cVar1 = *pcVar7 + -1;
        if (cVar1 == '\0') {
LAB_ram_563c:
          if (5 < (byte)param_3[0x22]) break;
          param_3[0x22] = param_3[0x22] + 1;
          _DAT_ram_ff11 = _DAT_ram_ff11 + 4;
          goto LAB_ram_5332;
        }
        *pcVar7 = cVar1;
      }
      _DAT_ram_ff11 = _DAT_ram_ff11 + *(short *)(_DAT_ram_ff11 + 2) + 4;
      param_3[0x17] = 0x40;
      param_3[0x18] = 0xc0;
      param_3[0x19] = 0xc0;
      param_3[0x1a] = 0x7f;
      goto LAB_ram_5332;
    }
    break;
  case 0xd1:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    if (*param_2 < 0x40) {
      param_3[9] = param_3[9] & 0xbf;
      param_3[0x10] = 0;
      param_3[0x11] = 0;
    }
    else {
      bVar2 = param_3[9];
      param_3[9] = param_3[9] | 0x40;
      if ((bVar2 & 0x40) == 0) {
        param_3[0x1b] = 0xff;
      }
    }
    goto LAB_ram_5332;
  case 0xd2:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_3[0x13] = *param_2;
    goto LAB_ram_5332;
  case 0xd3:
    goto z80_candidate_5414;
  case 0xd4:
    param_3[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b4d(param_1);
    goto joined_r0x545e;
  case 0xd5:
    param_3[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b68(param_1);
    goto joined_r0x545e;
  case 0xd6:
    goto code_r0x5450;
  case 0xd7:
    bVar2 = *param_2;
    param_3[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    sVar5 = (*(short *)(param_3 + 0x10) - 0x1000U) + (ushort)bVar2;
    if (CARRY2(*(short *)(param_3 + 0x10) - 0x1000U,(ushort)bVar2)) {
      sVar5 = -1;
    }
    *(short *)(param_3 + 0x10) = sVar5 + 0x1000;
    goto LAB_ram_5332;
  case 0xd8:
    bVar2 = *param_2;
    param_3[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    sVar5 = (*(short *)(param_3 + 0x10) + 0x1000U) - (ushort)bVar2;
    if (*(short *)(param_3 + 0x10) + 0x1000U < (ushort)bVar2) {
      sVar5 = 0;
    }
    *(short *)(param_3 + 0x10) = sVar5 + -0x1000;
    goto LAB_ram_5332;
  case 0xd9:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    DAT_ram_ff10 = DAT_ram_ff10 + 1;
    bIo000c = DAT_ram_ff10 & 1 | (*param_2 & 0x3f) * '\x02' + 0x80;
    goto LAB_ram_5332;
  }
  goto SUB_ram_5781;
z80_candidate_5414:
  param_3[0x16] = _DAT_ram_ff11[2];
  _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
  z80_candidate_4b32(param_1);
  goto joined_r0x545e;
LAB_ram_56de:
  if ((bVar2 & 0xf8) == 0xb8) {
    DAT_ram_fbf9 = *param_2;
    param_2 = _DAT_ram_ff11 + 2;
  }
  else {
    DAT_ram_fbf9 = ((bVar2 >> 3) - 0xb) + param_3[0x17];
  }
  param_3[0x17] = DAT_ram_fbf9;
  DAT_ram_fbfa = param_3[0x18];
  if (!(bool)(bVar2 & 1)) {
    DAT_ram_fbfa = *param_2;
    param_2 = param_2 + 1;
    param_3[0x18] = DAT_ram_fbfa;
  }
  param_3[0x16] = DAT_ram_fbfa;
  DAT_ram_fbfb = param_3[0x19];
  if (!(bool)(bVar2 >> 1 & 1)) {
    DAT_ram_fbfb = *param_2;
    param_2 = param_2 + 1;
    param_3[0x19] = DAT_ram_fbfb;
  }
  DAT_ram_fbfc = param_3[0x1a];
  if (!(bool)(bVar2 >> 2 & 1)) {
    DAT_ram_fbfc = *param_2;
    param_2 = param_2 + 1;
    param_3[0x1a] = DAT_ram_fbfc;
  }
  _DAT_ram_ff11 = param_2;
  if (((param_3[9] & 0x40) != 0) && (bVar2 = param_3[0x1b], bVar2 != 0xff)) {
    *(short *)(param_3 + 0x10) =
         (CONCAT11(*(undefined1 *)CONCAT11(1,bVar2 | 0x80),*(undefined1 *)CONCAT11(1,bVar2)) -
         CONCAT11(*(undefined1 *)CONCAT11(1,param_3[0x17] | 0x80),
                  *(undefined1 *)CONCAT11(1,param_3[0x17]))) + *(short *)(param_3 + 0x10);
  }
  param_3[0x1b] = param_3[0x17];
  bVar2 = 0;
  if (((DAT_ram_fbfb == 0) || (DAT_ram_fbfb == 0)) ||
     (z80_candidate_4a2a(DAT_ram_fbfc), (bVar2 & 1) != 0)) goto LAB_ram_5332;
LAB_ram_577a:
  if ((param_3[9] & 2) != 0) goto LAB_ram_5332;
SUB_ram_5781:
  param_3[0x12] = 0;
LAB_ram_5785:
  param_3[9] = 8;
  *param_3 = 0;
  param_3[1] = 0;
  param_3[2] = 0xff;
  return;
}
