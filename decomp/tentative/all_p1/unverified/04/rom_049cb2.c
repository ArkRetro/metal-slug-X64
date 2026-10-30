/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049cb2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49cb2(void)

{
  int iVar1;
  undefined4 uVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  int unaff_A6;
  byte bVar8;

  iVar1 = *(int *)(unaff_A6 + 0x7c);
  uVar2 = *(undefined4 *)(iVar1 + 0x40);
  *(short *)(iVar1 + 0x40) = *(short *)(iVar1 + 0x40) + 0x10;
  sVar3 = candidate_1b12e();
  *(undefined4 *)(iVar1 + 0x40) = uVar2;
  iVar1 = *(int *)((short)(*(short *)(unaff_A6 + 0x70) << 2) + 0xe0962);
  sVar4 = 0;
  while (*(short *)(iVar1 + sVar4) <= sVar3) {
    sVar4 = sVar4 + 4;
    if (*(short *)(iVar1 + sVar4) == -1) {
      __m68k_trap(0xf);
      return;
    }
  }
  bVar8 = *(short *)(iVar1 + 2 + (int)sVar4) != 0;
  sVar6 = *(short *)(iVar1 + sVar4);
  if (*(short *)(iVar1 + 2 + (int)(short)(sVar4 + -4)) != 0) {
    bVar8 = bVar8 | 2;
  }
  sVar4 = *(short *)(iVar1 + (short)(sVar4 + -4));
  if (bVar8 == 0) {
    sVar5 = sVar4 - sVar3;
    if (sVar5 < 0) {
      sVar5 = -sVar5;
    }
    sVar7 = sVar6 - sVar3;
    if (sVar7 < 0) {
      sVar7 = -sVar7;
    }
    if (sVar5 <= sVar7) {
      sVar6 = sVar4;
    }
    if (sVar6 < 0x80) {
      *(ushort *)(unaff_A6 + 0x80) = -sVar6 & 0xff;
      sVar6 = sVar6 - sVar3;
      if (sVar6 < 0) {
        *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x3c) + sVar6;
      }
      else {
        *(short *)(unaff_A6 + 0x78) = sVar6 + *(short *)(unaff_A6 + 0x3c);
      }
    }
    else {
      *(short *)(unaff_A6 + 0x80) = sVar6;
      sVar6 = sVar6 - sVar3;
      if (sVar6 < 0) {
        *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x3c) - sVar6;
      }
      else {
        *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x3c) - sVar6;
      }
    }
    return;
  }
  if (bVar8 != 3) {
    if ((bVar8 & 2) != 0) {
      sVar4 = sVar6;
    }
    if (sVar4 < 0x80) {
      *(ushort *)(unaff_A6 + 0x80) = -sVar4 & 0xff;
      sVar4 = sVar4 - sVar3;
      if (sVar4 < 0) {
        *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x3c) + sVar4;
      }
      else {
        *(short *)(unaff_A6 + 0x78) = sVar4 + *(short *)(unaff_A6 + 0x3c);
      }
    }
    else {
      *(short *)(unaff_A6 + 0x80) = sVar4;
      sVar4 = sVar4 - sVar3;
      if (sVar4 < 0) {
        *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x3c) - sVar4;
      }
      else {
        *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x3c) - sVar4;
      }
    }
    return;
  }
  *(undefined2 *)(unaff_A6 + 0x80) = 0xffff;
  return;
}
