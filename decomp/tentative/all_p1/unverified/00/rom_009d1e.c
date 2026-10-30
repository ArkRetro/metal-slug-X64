/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009d1e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d1e(void)

{
  int iVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  ushort in_D0w;
  short sVar6;
  ushort in_D1w;

  sVar6 = (in_D0w & 0xff) << 4;
  iVar1 = (int)sVar6;
  if (*(short *)(iVar1 + 0x1068f2) == 0) {
    return;
  }
  if ((in_D1w != *(ushort *)(iVar1 + 0x1068f4)) || (in_D1w != *(ushort *)(iVar1 + 0x1068f6))) {
    sVar5 = (in_D1w & 0xff) * 0x10 + 4;
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
    *(ushort *)(iVar1 + 0x1068f4) = in_D1w;
    *(ushort *)(iVar1 + 0x1068f6) = in_D1w;
    *(byte *)(iVar1 + 0x1068f9) = *(byte *)(iVar1 + 0x1068f9) | 0x80;
  }
  return;
}
