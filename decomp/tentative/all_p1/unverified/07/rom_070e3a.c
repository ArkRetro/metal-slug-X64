/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x070e3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_70e3a(void)

{
  byte bVar1;
  ushort uVar2;
  short sVar4;
  uint uVar3;
  int unaff_A6;
  bool bVar5;

  sVar4 = *(short *)(unaff_A6 + 0x34);
  bVar5 = sVar4 < 0;
  if (bVar5) {
    sVar4 = sVar4 + 1;
    bVar5 = sVar4 < 0;
  }
  if (sVar4 == 0) {
    uVar2 = 0x80;
    sVar4 = 0;
  }
  else if (bVar5) {
    uVar2 = 0x100;
    sVar4 = -0xe00;
  }
  else {
    uVar2 = 0x100;
    sVar4 = 0xe00;
  }
  sVar4 = sVar4 - *(short *)(unaff_A6 + 0x84);
  uVar3 = (uint)sVar4;
  if ((int)uVar3 < 0) {
    uVar3 = CONCAT22(sVar4 >> 0xf,-sVar4);
  }
  if (uVar2 < (ushort)uVar3) {
    uVar3 = CONCAT22((short)(uVar3 >> 0x10),uVar2);
  }
  if ((int)uVar3 < 0) {
    uVar3 = (uint)(ushort)-(short)uVar3;
  }
  *(short *)(unaff_A6 + 0x84) = (short)uVar3 + *(short *)(unaff_A6 + 0x84);
  bVar1 = *(char *)(unaff_A6 + 0x84) + 0xe;
  *(byte *)(unaff_A6 + 0x87) = bVar1;
  *(undefined2 *)(unaff_A6 + 0x80) = *(undefined2 *)((short)((ushort)bVar1 * 6) + 0xe2cba);
  return;
}
