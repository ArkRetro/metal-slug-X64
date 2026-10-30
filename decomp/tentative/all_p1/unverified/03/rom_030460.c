/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030460.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_30460(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  byte bVar3;
  char cVar4;
  uint3 uVar5;
  int unaff_A6;

  uVar2 = *(undefined4 *)((short)((*(byte *)(unaff_A6 + 0x8e) & 0xf) * 4) + 0x2d0a74);
  *(undefined4 *)(unaff_A6 + 0x76) = uVar2;
  uVar1 = *(undefined4 *)(unaff_A6 + 0x72);
  bVar3 = (char)uVar2 + (char)uVar1;
  bcdAdjust(bVar3);
  uVar5 = (uint3)(((uint)bVar3 << 0x18) >> 8);
  cVar4 = (char)((uint)uVar2 >> 8) + (char)((uint)uVar1 >> 8);
  bcdAdjust(cVar4);
  uVar5 = (uint3)((uint)(CONCAT31(uVar5,cVar4) << 0x18) >> 8) | uVar5 >> 8;
  cVar4 = (char)((uint)uVar2 >> 0x10) + (char)((uint)uVar1 >> 0x10);
  bcdAdjust(cVar4);
  uVar5 = (uint3)((uint)(CONCAT31(uVar5,cVar4) << 0x18) >> 8) | uVar5 >> 8;
  cVar4 = (char)((uint)uVar2 >> 0x18) + (char)((uint)uVar1 >> 0x18);
  bcdAdjust(cVar4);
  *(uint *)(unaff_A6 + 0x72) = CONCAT31(uVar5,cVar4) << 0x18 | (uint)uVar5;
  candidate_57948();
  return;
}
