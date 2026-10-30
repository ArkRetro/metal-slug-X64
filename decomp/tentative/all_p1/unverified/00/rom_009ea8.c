/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009ea8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ea8(void)

{
  int iVar1;
  short sVar2;
  short sVar3;
  byte bVar4;
  ushort uVar5;
  short sVar6;
  ushort uVar7;
  ushort in_D0w;
  undefined1 uVar8;
  byte in_D1b;
  ushort uVar9;
  short *psVar10;
  int unaff_A6;

  uVar7 = uRam001078ec;
  if (3 < *(byte *)(unaff_A6 + 0x1d)) {
    __m68k_trap(0xf);
    return;
  }
  if ((in_D1b & 1) == 0) {
    uVar5 = (in_D0w & 0xff) * 0x10 + 4;
    uVar9 = *(ushort *)((short)uVar5 + 0x1068ec);
    while (uVar9 != uVar5) {
      iVar1 = (int)(short)uVar9;
      psVar10 = (short *)(iVar1 + 0x1068ea);
      if ((in_D0w == *(ushort *)(iVar1 + 0x1068f4)) && ((*(byte *)(iVar1 + 0x1068f9) & 1) == 0))
      goto LAB_00009f28;
      uVar9 = *(ushort *)(iVar1 + 0x1068ec);
    }
  }
  iVar1 = (int)(short)uRam001078ec;
  psVar10 = (short *)(iVar1 + 0x1068ea);
  if (uRam001078ec == 0x1000) {
    psVar10 = (short *)0x1078da;
    uVar9 = 0xff0;
  }
  else {
    *(ushort *)(iVar1 + 0x1068f4) = in_D0w;
    *(ushort *)(iVar1 + 0x1068f6) = in_D0w;
    *(byte *)(iVar1 + 0x1068f9) = in_D1b | 0x80;
    uVar9 = uVar7;
  }
LAB_00009f28:
  if (psVar10[4] != -1) {
    psVar10[4] = psVar10[4] + 1;
  }
  sVar6 = (in_D0w & 0xff) * 0x10 + 4;
  iVar1 = (int)sVar6;
  if (uVar9 != *(ushort *)(iVar1 + 0x1068ec)) {
    sVar2 = *psVar10;
    sVar3 = psVar10[1];
    *(short *)(sVar3 + 0x1068ea) = sVar2;
    *(short *)(sVar2 + 0x1068ec) = sVar3;
    sVar2 = *(short *)(iVar1 + 0x1068ec);
    *(ushort *)(sVar2 + 0x1068ea) = uVar9;
    psVar10[1] = sVar2;
    *psVar10 = sVar6;
    *(ushort *)(iVar1 + 0x1068ec) = uVar9;
  }
  bVar4 = *(byte *)(unaff_A6 + 0x1d);
  uVar8 = (undefined1)(uVar9 >> 4);
  *(undefined1 *)(unaff_A6 + 0x19 + (int)(short)(ushort)bVar4) = uVar8;
  *(undefined1 *)(unaff_A6 + 0x14) = uVar8;
  *(byte *)(unaff_A6 + 0x1d) = bVar4 + 1;
  return;
}
