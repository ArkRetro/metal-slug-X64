/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002eb4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_2eb4(void)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  undefined3 uVar4;
  undefined4 in_D0;
  byte bVar5;
  short in_D1w;
  byte bVar6;
  byte bVar7;

  sVar3 = (short)in_D0 >> 1;
  bVar6 = -(sVar3 < 0);
  bVar5 = (bVar6 ^ (byte)sVar3) + (sVar3 < 0);
  sVar3 = in_D1w >> 1;
  bVar7 = -(sVar3 < 0);
  bVar1 = (bVar7 ^ (byte)sVar3) + (sVar3 < 0);
  uVar4 = (undefined3)(CONCAT22((short)((uint)in_D0 >> 0x10),(ushort)bVar5) >> 8);
  if (bVar5 == 0) {
    bVar5 = 0x40;
    uVar4 = 0;
  }
  else if (bVar1 == 0) {
    bVar5 = 0;
    uVar4 = 0;
  }
  else {
    bVar1 = *(byte *)((short)(ushort)bVar1 + 0x2c7c);
    bVar5 = *(byte *)((short)(ushort)bVar5 + 0x2c7c);
    bVar2 = bVar1 - bVar5;
    if (bVar1 < bVar5) {
      bVar5 = *(byte *)((short)(ushort)bVar2 + 0x2d7c);
    }
    else {
      bVar5 = 0x40 - *(char *)((short)(ushort)(byte)~bVar2 + 0x2d7c);
    }
  }
  bVar7 = bVar6 ^ bVar7;
  return CONCAT31(uVar4,(bVar6 & 0x80) + ((bVar7 ^ bVar5) - bVar7));
}
