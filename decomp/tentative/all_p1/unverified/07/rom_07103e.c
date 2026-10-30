/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07103e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7103e(void)

{
  int iVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  ushort uVar7;
  ushort *puVar8;
  ushort *puVar9;
  int unaff_A6;

  puVar9 = *(ushort **)(unaff_A6 + 0x6e) + 1;
  uVar7 = **(ushort **)(unaff_A6 + 0x6e);
  if (uVar7 == 0xffff) {
    puVar8 = (ushort *)((int)(short)*puVar9 + (int)puVar9);
    puVar9 = puVar8 + 1;
    uVar7 = *puVar8;
  }
  *(ushort **)(unaff_A6 + 0x6e) = puVar9;
  sVar6 = (ushort)*(byte *)(unaff_A6 + 0x14) << 4;
  iVar1 = (int)sVar6;
  if (*(short *)(iVar1 + 0x1068f2) == 0) {
    return;
  }
  if ((uVar7 != *(ushort *)(iVar1 + 0x1068f4)) || (uVar7 != *(ushort *)(iVar1 + 0x1068f6))) {
    sVar5 = (uVar7 & 0xff) * 0x10 + 4;
    iVar2 = (int)sVar5;
    if (sVar6 != *(short *)(iVar2 + 0x1068ec)) {
      sVar3 = *(short *)(iVar1 + 0x1068ea);
      sVar4 = *(short *)(iVar1 + 0x1068ec);
      *(short *)(sVar4 + 0x1068ea) = sVar3;
      *(short *)(sVar3 + 0x1068ec) = sVar4;
      sVar3 = *(short *)(iVar2 + 0x1068ec);
      *(short *)(sVar3 + 0x1068ea) = sVar6;
      *(short *)(iVar1 + 0x1068ec) = sVar3;
      *(short *)(iVar1 + 0x1068ea) = sVar5;
      *(short *)(iVar2 + 0x1068ec) = sVar6;
    }
    *(ushort *)(iVar1 + 0x1068f4) = uVar7;
    *(ushort *)(iVar1 + 0x1068f6) = uVar7;
    *(byte *)(iVar1 + 0x1068f9) = *(byte *)(iVar1 + 0x1068f9) | 0x80;
  }
  return;
}
