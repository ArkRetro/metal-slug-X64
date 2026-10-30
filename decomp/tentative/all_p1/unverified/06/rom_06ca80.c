/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ca80.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6ca80(void)

{
  undefined4 uVar1;
  byte bVar2;
  short sVar3;
  ushort uVar4;
  ushort uVar5;
  int unaff_A6;

  uVar1 = *(undefined4 *)(unaff_A6 + 0x34);
  uVar4 = *(ushort *)(unaff_A6 + 0x7a) - 1;
  *(ushort *)(unaff_A6 + 0x7a) = uVar4;
  if ((int)((uint)uVar4 << 0x10) < 0) {
    sVar3 = candidate_29f2();
    *(short *)(unaff_A6 + 0x7a) = sVar3 + 0x3c;
    sVar3 = candidate_29f2();
    *(short *)(unaff_A6 + 0x78) = sVar3 + 0x28;
  }
  uVar4 = *(short *)(unaff_A6 + 0x78) - *(short *)(unaff_A6 + 0x3c);
  uVar5 = (ushort)-((int)((uint)uVar4 << 0x10) < 0);
  uVar4 = (short)(((uVar5 ^ uVar4) - uVar5) * 0x100) >> 3;
  if (0x100 < uVar4) {
    uVar4 = 0x100;
  }
  *(int *)(unaff_A6 + 0x30) =
       ((short)((uVar5 ^ uVar4) - uVar5) * 0x100 - *(int *)(unaff_A6 + 0x30) >> 5) +
       *(int *)(unaff_A6 + 0x30);
  sVar3 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x93) * 2) + 0x29fc);
  bVar2 = *(byte *)(unaff_A6 + 0x93) + 1;
  *(byte *)(unaff_A6 + 0x93) = bVar2;
  *(int *)(unaff_A6 + 0x34) =
       ((int)((uint)(ushort)(*(short *)((short)((ushort)bVar2 * 2) + 0x29fc) - sVar3) << 0x10) >> 5)
       + *(int *)(unaff_A6 + 0x34);
  candidate_1ac38();
  *(undefined4 *)(unaff_A6 + 0x34) = uVar1;
  return;
}
