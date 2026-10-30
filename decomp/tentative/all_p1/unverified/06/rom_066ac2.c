/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066ac2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_66ac2(void)

{
  int *piVar1;
  uint in_D0;
  int iVar2;
  uint uVar3;
  int iVar4;
  ushort uVar5;
  byte bVar6;
  byte bVar7;
  char cVar8;
  char cVar9;
  int unaff_A5;
  bool bVar10;

  piVar1 = *(int **)(unaff_A5 + 0x4586);
  bVar7 = *(byte *)(piVar1 + 1);
  if (bVar7 == 0) {
    return in_D0;
  }
  bVar10 = (*(ushort *)(*piVar1 + 0xe) & 1) != 0;
  if (bVar10) {
    if (!bVar10) {
      return *(ushort *)(*piVar1 + 0xe) & 0xffff0001;
    }
    uVar5 = (short)CONCAT31((int3)(((uint)*(byte *)(_DAT_0010c586 + 9) << 5) >> 8),
                            *(char *)(_DAT_0010c586 + 10) + *(byte *)(_DAT_0010c586 + 9) * ' ') +
            0x7000;
    for (cVar9 = *(char *)(_DAT_0010c586 + 4); cVar9 != '\0'; cVar9 = cVar9 + -1) {
      iVar2 = CONCAT22(uVar5,0xff);
      for (cVar8 = *(char *)(_DAT_0010c586 + 0xb); iVar4 = iVar2, cVar8 != '\0'; cVar8 = cVar8 + -1)
      {
        iVar2 = iVar4 + 0x200000;
        _DAT_003c0000 = iVar4;
      }
      uVar5 = uVar5 + 1;
    }
    return (uint)uVar5;
  }
  bVar10 = false;
  iVar2 = candidate_66f7a();
  if (!bVar10) {
    _DAT_003c0000 =
         CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf4);
  }
  for (bVar6 = *(byte *)((int)piVar1 + 0xb); bVar10 = bVar6 < 2, 2 < bVar6; bVar6 = bVar6 - 1) {
    iVar2 = candidate_66f7a();
    if (!bVar10) {
      _DAT_003c0000 =
           CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf0);
    }
  }
  iVar2 = candidate_66f7a();
  if (!bVar10) {
    _DAT_003c0000 =
         CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf5);
  }
  for (; 2 < bVar7; bVar7 = bVar7 - 1) {
    bVar10 = false;
    iVar2 = candidate_66f7a();
    if (!bVar10) {
      _DAT_003c0000 =
           CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf2);
    }
    for (bVar6 = *(byte *)((int)piVar1 + 0xb); bVar10 = bVar6 < 2, 2 < bVar6; bVar6 = bVar6 - 1) {
      iVar2 = candidate_66f7a();
      if (!bVar10) {
        _DAT_003c0000 =
             CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0x60)
        ;
      }
    }
    iVar2 = candidate_66f7a();
    if (!bVar10) {
      _DAT_003c0000 =
           CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf3);
    }
  }
  bVar10 = false;
  iVar2 = candidate_66f7a();
  if (!bVar10) {
    _DAT_003c0000 =
         CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf6);
  }
  for (bVar7 = *(byte *)((int)piVar1 + 0xb); bVar10 = bVar7 < 2, 2 < bVar7; bVar7 = bVar7 - 1) {
    iVar2 = candidate_66f7a();
    if (!bVar10) {
      _DAT_003c0000 =
           CONCAT31((uint3)((uint)(iVar2 << 0x10) >> 8) | (uint3)(byte)((uint)iVar2 >> 0x18),0xf1);
    }
  }
  uVar3 = candidate_66f7a();
  if (!bVar10) {
    _DAT_003c0000 = CONCAT31((uint3)((uVar3 << 0x10) >> 8) | (uint3)(byte)(uVar3 >> 0x18),0xf7);
    uVar3 = (uint)(ushort)((short)uVar3 + 0x20);
  }
  return uVar3;
}
