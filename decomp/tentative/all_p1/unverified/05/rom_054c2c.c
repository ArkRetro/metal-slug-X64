/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x054c2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_54c2c(void)

{
  short sVar1;
  undefined2 uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined8 uVar5;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  candidate_552d8();
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x54ca0;
  if (*(char *)(unaff_A6[3] + 0x61) == '\0') {
    candidate_1abc8();
    candidate_427a();
    return;
  }
  candidate_2eb4();
  uVar5 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar5;
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)(unaff_A6 + 0xb);
  candidate_120f8();
  *unaff_A6 = 0x54d0a;
  if ((short)(*(short *)(unaff_A6 + 0xb) - *(short *)((int)unaff_A6 + 0x86)) < 0x20) {
    if (*(ushort *)(unaff_A6 + 0xb) < 0x5f) {
      candidate_1ac38();
    }
    if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
      uVar3 = (ushort)((*(short *)(unaff_A6 + 0xb) + -0x3f) * 0x80) >> 8;
      sVar1 = *(short *)(unaff_A6 + 0x21);
      *(ushort *)(unaff_A6 + 0x21) = uVar3;
      _DAT_0010e960 = (uint)(ushort)(uVar3 - sVar1) << 0x10;
    }
    candidate_427a();
    return;
  }
  *(byte *)(unaff_A6[3] + 0x90) =
       *(byte *)(unaff_A6[3] + 0x90) | '\x01' << (*(ushort *)((int)unaff_A6 + 0x6e) & 7);
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    _DAT_0010e960 = 0;
  }
  *(undefined2 *)(unaff_A6 + 10) =
       *(undefined2 *)((short)(*(short *)((int)unaff_A6 + 0x6e) * 2) + 0x5471a);
  *(undefined2 *)(unaff_A6 + 0xb) = 0x5f;
  candidate_55526();
  candidate_4256();
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(ushort *)(unaff_A6 + 0x1c) =
       *(byte *)((short)(*(short *)((int)unaff_A6 + 0x6e) + (*(byte *)(unaff_A6[3] + 0x93) & 3) * 4)
                + 0x54720) & 0x7f;
  *unaff_A6 = 0x54e00;
  candidate_1abc8();
  candidate_427a();
  return;
}
