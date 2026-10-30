/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06721e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_6721e(void)

{
  ushort uVar2;
  uint uVar1;
  ushort uVar3;
  char cVar4;
  int unaff_A2;
  int unaff_A5;
  bool bVar5;
  undefined2 uStack_2;

  uVar2 = (ushort)*(byte *)(unaff_A2 + 6) * 0x40;
  uVar3 = *(ushort *)(unaff_A2 + 4) + (ushort)*(byte *)(unaff_A2 + 6) * -0x40;
  if (*(ushort *)(unaff_A2 + 4) < uVar2) {
    uVar2 = uVar3 + uVar2;
    uVar3 = 0;
  }
  *(ushort *)(unaff_A2 + 4) = uVar3;
  candidate_66ac2();
  for (uStack_2 = (ushort)(uVar2 + (uVar3 & 0xff)) >> 8; uStack_2 != 0; uStack_2 = uStack_2 - 1) {
    for (cVar4 = *(char *)(*(int *)(unaff_A5 + 0x4586) + 0xb); bVar5 = false, cVar4 != '\0';
        cVar4 = cVar4 + -1) {
      uVar1 = candidate_66f7a();
      if (!bVar5) {
        _DAT_003c0000 = uVar1 << 0x10 | uVar1 >> 0x10;
      }
    }
  }
  return;
}
