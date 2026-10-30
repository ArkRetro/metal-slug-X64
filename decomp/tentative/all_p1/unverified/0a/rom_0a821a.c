/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a821a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a821a(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  char cVar4;
  ushort uVar5;
  ushort uVar6;
  short sVar7;
  short sVar8;
  undefined4 *unaff_A6;

  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + 1;
  func_0x00009cce();
  uVar5 = *(ushort *)((int)unaff_A6 + 0x72);
  if (*(ushort *)(unaff_A6 + 0x1d) < uVar5) {
    sVar7 = 0x300;
    sVar8 = uVar5 + *(ushort *)(unaff_A6 + 0x1d) * 2;
    uVar6 = sVar8 - 3;
    if (uVar6 != 0 && SBORROW2(sVar8,3) == (int)((uint)uVar6 << 0x10) < 0) {
      uVar3 = ((uint)(ushort)(uVar5 - 1) * 0x100) / (uint)uVar6;
      cVar4 = (char)uVar3 + -1;
      if ((short)uVar3 == 0) goto LAB_000a8340;
      *(char *)((int)unaff_A6 + 0x12) = cVar4;
      *(char *)((int)unaff_A6 + 0x13) = cVar4;
      sVar7 = (short)(((uint)(ushort)(uVar5 - 1) * 0x300) / (uint)uVar6);
    }
    iVar1 = (uint)*(ushort *)((short)((ushort)*(byte *)(unaff_A6 + 0x1c) << 2) + 0xa7fae) * 0x10000
            - unaff_A6[10];
    iVar2 = (uint)*(ushort *)((short)((ushort)*(byte *)(unaff_A6 + 0x1c) << 2) + 0xa7fb0) * 0x10000
            - unaff_A6[0xb];
    uVar5 = (ushort)(*(short *)((int)unaff_A6 + 0x72) - *(ushort *)(unaff_A6 + 0x1d)) >> 1;
    if (3 < uVar5) {
      uVar5 = 4;
    }
    uVar6 = *(ushort *)(unaff_A6 + 0x1d) >> 1;
    if (uVar6 < 3) {
      uVar6 = 3;
    }
    unaff_A6[10] = ((iVar1 >> (uVar5 & 0x3f)) - (iVar2 >> (uVar6 & 0x3f))) + unaff_A6[10];
    unaff_A6[0xb] = (iVar1 >> (uVar6 & 0x3f)) + (iVar2 >> (uVar5 & 0x3f)) + unaff_A6[0xb];
    uVar5 = (ushort)((uint)((int)(short)(*(short *)(unaff_A6 + 10) + -0xa0) * (int)sVar7) >> 8);
    sVar8 = (short)((uint)((int)(short)(*(short *)(unaff_A6 + 0xb) + -0x110) * (int)sVar7) >> 8) +
            0x110;
    *(ushort *)(unaff_A6 + 0xf) = uVar5 + 0xa0;
    *(short *)(unaff_A6 + 0x10) = sVar8;
    if (-1 < (int)((uint)uVar5 << 0x10)) {
      uVar5 = -uVar5;
    }
    sVar7 = *(short *)((int)unaff_A6 + 0x72) - *(short *)(unaff_A6 + 0x1d);
    if (sVar7 != 0) {
      sVar7 = sVar7 + 0x10;
    }
    *(ushort *)((int)unaff_A6 + 0x1e) = sVar7 * 0x100 + (((uVar5 >> 2) + 0x8000) - sVar8);
    if (*(ushort *)(unaff_A6 + 0x10) < 0x280) {
      candidate_427a();
    }
    return;
  }
LAB_000a8340:
  if (*(char *)(unaff_A6 + 0x1c) == '\t') {
    candidate_120f8();
    *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  }
  else {
    candidate_120f8();
    sVar8 = 0x18;
    do {
      candidate_40bc();
      sVar8 = sVar8 + -1;
    } while (sVar8 != 0);
  }
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0xf) =
       *(undefined2 *)((short)((ushort)*(byte *)(unaff_A6 + 0x1c) << 2) + 0xa7fae);
  *(undefined2 *)(unaff_A6 + 0x10) =
       *(undefined2 *)((short)((ushort)*(byte *)(unaff_A6 + 0x1c) << 2) + 0xa7fb0);
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  *unaff_A6 = 0xa83c2;
  candidate_427a();
  return;
}
