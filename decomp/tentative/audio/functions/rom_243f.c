/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x243f; tentative_c. */

void z80_candidate_243f(byte *param_1)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  short sVar6;

  bVar5 = *param_1;
  bVar4 = param_1[1];
  if (bVar5 == 0 && bVar4 == 0) {
    return;
  }
  bVar3 = param_1[2];
  if (bVar3 == 0) {
    bVar3 = param_1[3];
    param_1[3] = bVar3 - bVar5;
    uVar2 = (ushort)(bVar3 < bVar5);
    uVar1 = *(ushort *)(param_1 + 4) - (ushort)bVar4;
    sVar6 = uVar1 - uVar2;
    if (*(ushort *)(param_1 + 4) < (ushort)bVar4 || uVar1 < uVar2) {
      sVar6 = 0;
    }
    param_1[5] = (byte)((ushort)sVar6 >> 8);
    param_1[4] = (byte)sVar6;
  }
  else {
    param_1[2] = bVar3 - 1;
  }
  bVar4 = param_1[6];
  if (!CARRY1(bVar4,bVar4)) {
    return;
  }
  if (CARRY1(bVar4 * '\x02',bVar4 * '\x02')) {
    return;
  }
  return;
}
