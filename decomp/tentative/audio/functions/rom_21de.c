/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x21de; tentative_c. */

byte z80_candidate_21de(byte *param_1)

{
  bool bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte *pbVar8;

  bVar2 = *param_1;
  if (bVar2 == 0) {
    return bVar2;
  }
  pbVar8 = param_1 + 0xc;
  bVar7 = (byte)((ushort)*(undefined2 *)(param_1 + 10) >> 8);
  bVar6 = (byte)*(undefined2 *)(param_1 + 10);
  if (bVar2 == 1) {
    bVar5 = *pbVar8;
    *pbVar8 = bVar5 + bVar6;
    pbVar8 = param_1 + 0xd;
    bVar3 = *pbVar8;
    bVar2 = bVar3 + bVar7;
    bVar1 = CARRY1(bVar2,CARRY1(bVar5,bVar6));
    bVar2 = bVar2 + CARRY1(bVar5,bVar6);
    *pbVar8 = bVar2;
    if (!CARRY1(bVar3,bVar7) && !bVar1) {
      return bVar2;
    }
    *pbVar8 = -(CARRY1(bVar3,bVar7) || bVar1);
    *param_1 = *param_1 + 1;
    param_1[0xb] = param_1[5];
    param_1[10] = param_1[4];
    return 0xff;
  }
  if (bVar2 != 2) {
    if (bVar6 == 0 && bVar7 == 0) {
      return param_1[0xd];
    }
    bVar3 = *pbVar8;
    *pbVar8 = bVar3 - bVar6;
    pbVar8 = param_1 + 0xd;
    bVar4 = *pbVar8;
    bVar2 = bVar4 - bVar7;
    bVar5 = bVar2 - (bVar3 < bVar6);
    *pbVar8 = bVar5;
    if (bVar7 <= bVar4 && (bVar3 < bVar6) <= bVar2) {
      return bVar5;
    }
    *pbVar8 = 0;
    *param_1 = 0;
    return 0;
  }
  if (bVar6 == 0 && bVar7 == 0) {
    return param_1[0xd];
  }
  bVar3 = *pbVar8;
  *pbVar8 = bVar3 - bVar6;
  pbVar8 = param_1 + 0xd;
  bVar2 = *pbVar8 - bVar7;
  bVar5 = bVar2 - (bVar3 < bVar6);
  bVar4 = param_1[1];
  if ((bVar7 <= *pbVar8 && (bVar3 < bVar6) <= bVar2) && (bVar4 <= bVar5)) {
    *pbVar8 = bVar5;
    return bVar5;
  }
  *pbVar8 = bVar4;
  if (bVar4 == 0) {
    *param_1 = 0;
    return 0;
  }
  *param_1 = *param_1 + 1;
  param_1[0xb] = param_1[7];
  param_1[10] = param_1[6];
  return bVar4;
}
