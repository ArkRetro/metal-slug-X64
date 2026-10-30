/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009d8c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d8c(void)

{
  int iVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  short sVar5;
  short sVar6;
  undefined1 in_D0b;
  short sVar7;
  short *in_A0;

  sVar7 = *in_A0;
  DAT_00300001 = in_D0b;
  while (sVar7 != -1) {
    sVar7 = sVar7 << 4;
    iVar1 = (int)sVar7;
    uVar4 = in_A0[1];
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
      *(byte *)(iVar1 + 0x1068f9) = *(byte *)(in_A0 + 2) | 0x80;
    }
    in_A0 = in_A0 + 3;
    sVar7 = *in_A0;
  }
  return;
}
