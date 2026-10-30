/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d4ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3d4ee(void)

{
  int iVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  short *psVar8;

  candidate_9e6c();
  DAT_00300001 = candidate_3d4ac();
  psVar8 = (short *)0x3d452;
  if (DAT_0010c2e7 != '\0') {
    psVar8 = (short *)0x3d47e;
  }
  sVar7 = *psVar8;
  while (sVar7 != -1) {
    sVar7 = sVar7 << 4;
    iVar1 = (int)sVar7;
    uVar4 = psVar8[1];
    sVar6 = (uVar4 & 0xff) * 0x10 + 4;
    iVar2 = (int)sVar6;
    if (sVar7 != *(short *)(iVar2 + 0x1068ec)) {
      sVar3 = *(short *)(iVar1 + 0x1068ea);
      sVar5 = *(short *)(iVar1 + 0x1068ec);
      *(short *)(sVar5 + 0x1068ea) = sVar3;
      *(short *)(sVar3 + 0x1068ec) = sVar5;
      sVar3 = *(short *)(iVar2 + 0x1068ec);
      *(short *)(sVar3 + 0x1068ea) = sVar7;
      *(short *)(iVar1 + 0x1068ec) = sVar3;
      *(short *)(iVar1 + 0x1068ea) = sVar6;
      *(short *)(iVar2 + 0x1068ec) = sVar7;
    }
    *(undefined2 *)(iVar1 + 0x1068f2) = 0xffff;
    if (uVar4 == 0xffff) {
      *(undefined2 *)(iVar1 + 0x1068f4) = 0xffff;
      *(undefined2 *)(iVar1 + 0x1068f6) = 0xffff;
      *(undefined1 *)(iVar1 + 0x1068f9) = 2;
    }
    else {
      *(ushort *)(iVar1 + 0x1068f4) = uVar4;
      *(ushort *)(iVar1 + 0x1068f6) = uVar4;
      *(byte *)(iVar1 + 0x1068f9) = *(byte *)(psVar8 + 2) | 0x80;
    }
    psVar8 = psVar8 + 3;
    sVar7 = *psVar8;
  }
  return;
}
