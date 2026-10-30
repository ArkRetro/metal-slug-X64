/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x5592; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_5592(undefined1 *param_1)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  undefined1 uVar4;
  byte *pbVar5;
  ushort uVar6;
  short sVar7;
  char *pcVar8;
  byte *pbVar9;

code_r0x5592:
  bVar3 = param_1[0x22];
  if (5 < bVar3) {
LAB_ram_5785:
    param_1[9] = 8;
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0xff;
    return;
  }
  param_1[0x22] = bVar3 + 2;
  _DAT_ram_ff11 = *(byte **)(param_1 + bVar3 + 0x1c);
  param_1[0x17] = 0x40;
  param_1[0x18] = 0xc0;
  param_1[0x19] = 0xc0;
  param_1[0x1a] = 0x7f;
LAB_ram_5332:
  pbVar5 = _DAT_ram_ff11;
  if (param_1[0x16] != '\0') {
    param_1[0x14] = (char)_DAT_ram_ff11;
    param_1[0x15] = (char)((ushort)pbVar5 >> 8);
    param_1[0x16] = param_1[0x16] + -1;
    if ((param_1[9] & 2) != 0) {
      return;
    }
    bVar3 = 0;
    if ((param_1[9] & 0x20) == 0) {
      return;
    }
    z80_candidate_4b09();
    if ((bool)(bVar3 & 1)) {
      return;
    }
    goto LAB_ram_577a;
  }
  if (DAT_ram_ff13 != param_1[4]) {
    DAT_ram_ff13 = param_1[4];
  }
  bVar3 = *_DAT_ram_ff11;
  pbVar5 = _DAT_ram_ff11 + 1;
  if (0xd9 < bVar3) goto SUB_ram_5781;
  if (bVar3 < 0xc0) goto LAB_ram_56de;
  uVar6 = (ushort)(byte)((bVar3 + 0x40) * '\x03');
  bVar2 = 0xac68 < uVar6;
  sVar7 = uVar6 + 0x5397;
  switch(bVar3) {
  case 0xc0:
    param_1[10] = *pbVar5;
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0xc1:
    param_1[10] = *pbVar5;
    param_1[0xb] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    goto LAB_ram_5332;
  case 0xc2:
    _DAT_ram_ff11 = _DAT_ram_ff11 + *(short *)pbVar5 + 3;
    param_1[0x17] = 0x40;
    param_1[0x18] = 0xc0;
    param_1[0x19] = 0xc0;
    param_1[0x1a] = 0x7f;
    goto LAB_ram_5332;
  case 0xc3:
    if (*pbVar5 == 0) {
      param_1[9] = param_1[9] & 0xfe;
    }
    else {
      param_1[9] = param_1[9] | 1;
    }
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0xc4:
    pbVar9 = _DAT_ram_ff11 + 3;
    _DAT_ram_ff11 = pbVar9 + *(short *)pbVar5;
    if (param_1[0x22] != '\0') {
      bVar3 = param_1[0x22] - 2;
      if (5 < bVar3) break;
      param_1[0x22] = bVar3;
      *(byte **)(param_1 + bVar3 + 0x1c) = pbVar9;
      param_1[0x17] = 0x40;
      param_1[0x18] = 0xc0;
      param_1[0x19] = 0xc0;
      param_1[0x1a] = 0x7f;
    }
    goto LAB_ram_5332;
  case 0xc5:
    goto z80_candidate_56a5_override_jmp_ram_5396_case_5;
  case 0xc6:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_1[0xc] = *pbVar5 & 0x7f;
    goto LAB_ram_5332;
  case 199:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_1[0xd] = *pbVar5 & 0x7f;
    goto LAB_ram_5332;
  case 200:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_1[0xe] = *pbVar5 & 0x7f;
    goto LAB_ram_5332;
  case 0xc9:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    if (*pbVar5 == 0) {
      param_1[9] = param_1[9] & 0xef;
    }
    else {
      param_1[9] = param_1[9] | 0x10;
    }
    goto LAB_ram_5332;
  case 0xca:
    sVar7 = *(short *)pbVar5;
    param_1[0x16] = _DAT_ram_ff11[3];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 4;
    uVar6 = sVar7 * 4;
    param_1[0x10] = (char)((ushort)(sVar7 * 8) >> 8);
    param_1[0x11] = -CARRY2(uVar6,uVar6);
    goto LAB_ram_5332;
  case 0xcb:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_1[0x16] = *pbVar5;
    goto LAB_ram_5332;
  case 0xcc:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    uVar4 = 3;
    if ((*pbVar5 != 0x40) && (uVar4 = 2, 0x3f < *pbVar5)) {
      uVar4 = 1;
    }
    param_1[0xf] = uVar4;
    goto LAB_ram_5332;
  case 0xcd:
    pbVar5 = _DAT_ram_ff11 + 2;
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4f60(*pbVar5,sVar7);
    goto LAB_ram_5332;
  case 0xce:
    param_1[0x12] = *pbVar5 * '\x02' + (0x7f < (byte)(*pbVar5 * '\x02'));
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    goto LAB_ram_5332;
  case 0xcf:
    bVar3 = param_1[0x22] - 1;
    _DAT_ram_ff11 = pbVar5;
    if (bVar3 < 6) {
      param_1[0x22] = bVar3;
      param_1[bVar3 + 0x1c] = 0;
      param_1[0x17] = 0x40;
      param_1[0x18] = 0xc0;
      param_1[0x19] = 0xc0;
      param_1[0x1a] = 0x7f;
      goto LAB_ram_5332;
    }
    break;
  case 0xd0:
    if ((byte)param_1[0x22] < 6) {
      pcVar8 = param_1 + (byte)param_1[0x22] + 0x1c;
      if (*pcVar8 == '\0') {
        if (*pbVar5 != 0) {
          cVar1 = *pbVar5 - 1;
          if (cVar1 == '\0') goto LAB_ram_563c;
          *pcVar8 = cVar1;
        }
      }
      else {
        cVar1 = *pcVar8 + -1;
        if (cVar1 == '\0') {
LAB_ram_563c:
          if (5 < (byte)param_1[0x22]) break;
          param_1[0x22] = param_1[0x22] + 1;
          _DAT_ram_ff11 = _DAT_ram_ff11 + 4;
          goto LAB_ram_5332;
        }
        *pcVar8 = cVar1;
      }
      _DAT_ram_ff11 = _DAT_ram_ff11 + *(short *)(_DAT_ram_ff11 + 2) + 4;
      param_1[0x17] = 0x40;
      param_1[0x18] = 0xc0;
      param_1[0x19] = 0xc0;
      param_1[0x1a] = 0x7f;
      goto LAB_ram_5332;
    }
    break;
  case 0xd1:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    if (*pbVar5 < 0x40) {
      param_1[9] = param_1[9] & 0xbf;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
    }
    else {
      bVar3 = param_1[9];
      param_1[9] = param_1[9] | 0x40;
      if ((bVar3 & 0x40) == 0) {
        param_1[0x1b] = 0xff;
      }
    }
    goto LAB_ram_5332;
  case 0xd2:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    param_1[0x13] = *pbVar5;
    goto LAB_ram_5332;
  case 0xd3:
    param_1[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b32(sVar7);
    goto joined_r0x544a;
  case 0xd4:
    param_1[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b4d(sVar7);
    goto joined_r0x544a;
  case 0xd5:
    param_1[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b68(sVar7);
joined_r0x544a:
    if ((bVar2 & 1) == 0) goto LAB_ram_577a;
    goto LAB_ram_5332;
  case 0xd6:
    param_1[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    z80_candidate_4b76(sVar7);
    goto joined_r0x5774;
  case 0xd7:
    bVar3 = *pbVar5;
    param_1[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    sVar7 = (*(short *)(param_1 + 0x10) - 0x1000U) + (ushort)bVar3;
    if (CARRY2(*(short *)(param_1 + 0x10) - 0x1000U,(ushort)bVar3)) {
      sVar7 = -1;
    }
    *(short *)(param_1 + 0x10) = sVar7 + 0x1000;
    goto LAB_ram_5332;
  case 0xd8:
    bVar3 = *pbVar5;
    param_1[0x16] = _DAT_ram_ff11[2];
    _DAT_ram_ff11 = _DAT_ram_ff11 + 3;
    sVar7 = (*(short *)(param_1 + 0x10) + 0x1000U) - (ushort)bVar3;
    if (*(short *)(param_1 + 0x10) + 0x1000U < (ushort)bVar3) {
      sVar7 = 0;
    }
    *(short *)(param_1 + 0x10) = sVar7 + -0x1000;
    goto LAB_ram_5332;
  case 0xd9:
    _DAT_ram_ff11 = _DAT_ram_ff11 + 2;
    DAT_ram_ff10 = DAT_ram_ff10 + 1;
    bIo000c = DAT_ram_ff10 & 1 | (*pbVar5 & 0x3f) * '\x02' + 0x80;
    goto LAB_ram_5332;
  }
  goto SUB_ram_5781;
LAB_ram_56de:
  if ((bVar3 & 0xf8) == 0xb8) {
    DAT_ram_fbf9 = *pbVar5;
    pbVar5 = _DAT_ram_ff11 + 2;
  }
  else {
    DAT_ram_fbf9 = ((bVar3 >> 3) - 0xb) + param_1[0x17];
  }
  param_1[0x17] = DAT_ram_fbf9;
  DAT_ram_fbfa = param_1[0x18];
  if (!(bool)(bVar3 & 1)) {
    DAT_ram_fbfa = *pbVar5;
    pbVar5 = pbVar5 + 1;
    param_1[0x18] = DAT_ram_fbfa;
  }
  param_1[0x16] = DAT_ram_fbfa;
  DAT_ram_fbfb = param_1[0x19];
  if (!(bool)(bVar3 >> 1 & 1)) {
    DAT_ram_fbfb = *pbVar5;
    pbVar5 = pbVar5 + 1;
    param_1[0x19] = DAT_ram_fbfb;
  }
  DAT_ram_fbfc = param_1[0x1a];
  if (!(bool)(bVar3 >> 2 & 1)) {
    DAT_ram_fbfc = *pbVar5;
    pbVar5 = pbVar5 + 1;
    param_1[0x1a] = DAT_ram_fbfc;
  }
  _DAT_ram_ff11 = pbVar5;
  if (((param_1[9] & 0x40) != 0) && (bVar3 = param_1[0x1b], bVar3 != 0xff)) {
    *(short *)(param_1 + 0x10) =
         (CONCAT11(*(undefined1 *)CONCAT11(1,bVar3 | 0x80),*(undefined1 *)CONCAT11(1,bVar3)) -
         CONCAT11(*(undefined1 *)CONCAT11(1,param_1[0x17] | 0x80),
                  *(undefined1 *)CONCAT11(1,param_1[0x17]))) + *(short *)(param_1 + 0x10);
  }
  param_1[0x1b] = param_1[0x17];
  bVar2 = 0;
  if ((DAT_ram_fbfb == 0) || (DAT_ram_fbfb == 0)) goto LAB_ram_5332;
  z80_candidate_4a2a(DAT_ram_fbfc);
joined_r0x5774:
  if ((bVar2 & 1) != 0) goto LAB_ram_5332;
LAB_ram_577a:
  if ((param_1[9] & 2) != 0) goto LAB_ram_5332;
SUB_ram_5781:
  param_1[0x12] = 0;
  goto LAB_ram_5785;
z80_candidate_56a5_override_jmp_ram_5396_case_5:
  goto code_r0x5592;
}
