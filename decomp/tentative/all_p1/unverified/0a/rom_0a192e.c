/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a192e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a192e(void)

{
  short sVar1;
  short sVar2;
  undefined2 uVar4;
  int iVar3;
  short sVar5;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  bVar6 = *(char *)((int)unaff_A6 + 0x89) == '\0';
  if ('\0' < *(char *)((int)unaff_A6 + 0x89)) {
    candidate_1af9a();
    if (!bVar6) {
      candidate_1b14a();
    }
    *(undefined1 *)(unaff_A6 + 0x22) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x92) & 0x80) != 0) {
    *(undefined1 *)(unaff_A6 + 0x22) = 0x40;
  }
  sVar1 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x22) << 1) + 0x2a7c);
  sVar2 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x22) << 1) + 0x29fc);
  sVar5 = *(short *)(*(int *)(unaff_A6[3] + 0xc) + 0x76);
  unaff_A6[0xc] = (int)sVar1 * (int)sVar5;
  unaff_A6[0xd] = (int)sVar2 * (int)sVar5;
  *(int *)((int)unaff_A6 + 0x62) = (int)sVar5 << 8;
  *(short *)(unaff_A6 + 10) = (sVar1 >> 7) + *(short *)(unaff_A6 + 10);
  *(short *)(unaff_A6 + 0xb) = (sVar2 >> 7) + *(short *)(unaff_A6 + 0xb);
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_a2292();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  candidate_4256();
  *unaff_A6 = 0xa19ea;
  candidate_1abc8();
  if (*(short *)((int)unaff_A6 + 0x82) == 0) {
    candidate_1ac38();
  }
  else {
    sVar5 = *(short *)((int)unaff_A6 + 0x82) + -1;
    *(short *)((int)unaff_A6 + 0x82) = sVar5;
    if (sVar5 == 0) {
      candidate_4256();
    }
  }
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + '\x01';
  if ('\x02' < *(char *)((int)unaff_A6 + 0x8f)) {
    *(undefined1 *)((int)unaff_A6 + 0x8f) = 0;
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)((int)unaff_A6 + (short)(*(byte *)((int)unaff_A6 + 0x8f) & 3) + 0x19);
  if ((*(byte *)((int)unaff_A6 + 0x92) & 0x80) != 0) {
    sVar5 = *(short *)(unaff_A6 + 0xb);
    if ((*(byte *)((int)unaff_A6 + 0x92) & 0x40) == 0) {
      bVar6 = SBORROW2(sVar5,0x98);
      sVar1 = -0x98;
    }
    else {
      bVar6 = SBORROW2(sVar5,0xb0);
      sVar1 = -0xb0;
    }
    if (bVar6 == (short)(sVar5 + sVar1) < 0) {
      *(byte *)((int)unaff_A6 + 0x92) = *(byte *)((int)unaff_A6 + 0x92) & 0x7f;
      candidate_4256();
      uVar4 = candidate_a22fc();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar4;
      unaff_A6[0xd] = 0;
      bVar6 = false;
      candidate_1af9a();
      if (bVar6) {
        iVar3 = *(int *)((int)unaff_A6 + 0x62);
        if (*(short *)(unaff_A6 + 0xc) < 0) {
          iVar3 = -iVar3;
        }
      }
      else {
        iVar3 = *(int *)((int)unaff_A6 + 0x62);
        if ((short)(*(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0xf)) < 0) {
          iVar3 = -iVar3;
        }
      }
      unaff_A6[0xc] = iVar3;
    }
  }
  sVar5 = candidate_49e8();
  if (sVar5 != 0) {
    *unaff_A6 = candidate_a1b14;
  }
  if (0x1e7 < (short)*(ushort *)(unaff_A6 + 0x10)) {
    *unaff_A6 = 0xa1b1e;
  }
  uVar7 = *(ushort *)(unaff_A6 + 0x10) < 0x1e8;
  candidate_19424();
  if ((bool)uVar7) {
    *unaff_A6 = 0xa1b1e;
    uVar7 = 0;
  }
  candidate_19504();
  if ((bool)uVar7) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
