/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03ce00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3ce00(void)

{
  int iVar1;
  bool bVar2;
  byte bVar3;
  ushort uVar4;
  short sVar5;
  byte bVar6;
  ushort uVar7;
  ushort uVar8;
  int unaff_A5;

  bVar6 = ~DAT_00300000;
  bVar3 = ~DAT_00340000;
  uVar4 = *(ushort *)(unaff_A5 + 0x4374);
  *(ushort *)(unaff_A5 + 0x4374) = CONCAT11(bVar6,bVar3);
  uVar4 = uVar4 ^ CONCAT11(bVar6,bVar3);
  iVar1 = *(int *)(unaff_A5 + 0x4376);
  bVar6 = *(byte *)(unaff_A5 + 0x437e);
  uVar7 = *(ushort *)(unaff_A5 + 0x437c);
  uVar8 = *(ushort *)(unaff_A5 + 0x437a);
  if ((byte)(-(uVar8 != uVar7) | 0xfU) == bVar6) {
    *(byte *)(iVar1 + (short)uVar7) = bVar6;
    uVar7 = uVar7 + 1;
    if (0x1ff < uVar7) {
LAB_0003ce86:
      *(undefined1 *)(unaff_A5 + 0x4382) = 2;
      return;
    }
    bVar6 = 0;
  }
  if (uVar4 != 0) {
    sVar5 = 0xf;
    do {
      bVar2 = (short)uVar4 < 0;
      uVar4 = uVar4 << 1;
      if (bVar2) {
        *(byte *)(iVar1 + (short)uVar7) = bVar6;
        bVar6 = 0;
        uVar7 = uVar7 + 1;
        *(byte *)(iVar1 + (short)uVar8) = (char)sVar5 << 4 | *(byte *)(iVar1 + (short)uVar8);
        uVar8 = uVar7;
        if (0x1ff < uVar7) goto LAB_0003ce86;
      }
      sVar5 = sVar5 + -1;
    } while (sVar5 != -1);
  }
  *(ushort *)(unaff_A5 + 0x437c) = uVar7;
  *(ushort *)(unaff_A5 + 0x437a) = uVar8;
  *(byte *)(unaff_A5 + 0x437e) = bVar6 + 1;
  return;
}
