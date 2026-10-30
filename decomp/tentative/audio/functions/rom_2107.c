/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2107; tentative_c. */

void z80_candidate_2107(short param_1)

{
  undefined1 uVar1;
  byte bVar2;
  byte bVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  char cVar6;
  short sVar7;
  byte *pbVar8;

  sVar7 = 0x24a8;
  func_0x5204(CONCAT11(uRam24a9,uRam24a8));
  func_0x5204(*(undefined2 *)(sVar7 + 2));
  func_0x5204(*(undefined2 *)(sVar7 + 4));
  func_0x5204(*(undefined2 *)(sVar7 + 6));
  uVar1 = *(undefined1 *)(sVar7 + 8);
  pbVar8 = (byte *)(sVar7 + 9);
  while( true ) {
    uVar4 = *(undefined2 *)pbVar8;
    bVar3 = pbVar8[2] | *pbVar8 | pbVar8[1];
    if (bVar3 == 0) break;
    bVar2 = pbVar8[4];
    uVar5 = uVar4;
    z80_candidate_50cf(bVar3);
    cVar6 = (char)((ushort)uVar5 >> 8);
    if (cVar6 != '\0') {
      z80_candidate_5245(cVar6,uVar4);
      *(byte *)(param_1 + 2) = bVar2;
      *(undefined1 *)(param_1 + 3) = uVar1;
      *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 1;
    }
    pbVar8 = pbVar8 + 5;
  }
  return;
}
