/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aef30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aef30(void)

{
  char cVar4;
  short sVar2;
  uint uVar1;
  undefined2 uVar3;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  ushort uVar5;
  uint unaff_D6;
  undefined4 unaff_D7;
  int extraout_A0;
  int iVar6;
  int extraout_A1;
  int unaff_A5;
  int unaff_A6;
  bool bVar7;

  candidate_3c52c();
  uVar1 = candidate_260c8();
  if (*(ushort *)(unaff_A6 + 0x70) != 0) {
    uVar1 = (uVar1 & 0xffff) * (uint)*(ushort *)(unaff_A6 + 0x70);
  }
  *(short *)(unaff_A6 + 0x70) = (short)(uVar1 >> 8);
  uVar1 = candidate_260c8();
  uVar5 = *(ushort *)(unaff_A6 + 0x72);
  if (uVar5 == 0) {
    uVar5 = 0x1e;
  }
  *(short *)(unaff_A6 + 0x72) = (short)((uVar1 & 0xffff) * (uint)uVar5 >> 8);
  uVar5 = candidate_29c6();
  *(short *)(unaff_A6 + 0x72) = (uVar5 & 7) + *(short *)(unaff_A6 + 0x72);
  if (*(short *)(unaff_A6 + 0x74) == 0) {
    *(undefined2 *)(unaff_A6 + 0x74) = 0x5a;
  }
  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)(unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8820;
  sVar2 = *(short *)(unaff_A6 + 0x76);
  if ((((sVar2 == 0) || (sVar2 == 7)) || (sVar2 == 5)) || (sVar2 == 6)) {
    bVar7 = false;
    candidate_1495c();
    if (bVar7) {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
    }
    *(char *)(unaff_A6 + 0x6c) = (char)((uint)unaff_D7 >> 0x10);
  }
  else {
    candidate_146e8();
  }
  candidate_1abc8();
  if ((short)(*(short *)(unaff_A6 + 0x3c) + -0xa0) < 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  *(short *)(unaff_A6 + 0x84) = ((short)(*(short *)(unaff_A6 + 0x3c) + -0xa0) >> 2) + 0xa0;
  uVar3 = candidate_29c6();
  *(undefined2 *)(unaff_A6 + 0x86) = uVar3;
  candidate_ae9c0();
  cVar4 = candidate_1ae8e();
  if (cVar4 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x80) = 0xff;
  }
  else {
    if (cVar4 == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x80) = 0;
      iVar6 = unaff_A5 + -0x6b3c;
    }
    else if (cVar4 == '\x02') {
      *(undefined1 *)(unaff_A6 + 0x80) = 1;
      iVar6 = unaff_A5 + -0x6a8c;
    }
    else {
      uVar5 = candidate_29f2();
      iVar6 = extraout_A0;
      if (extraout_D1w < uVar5) {
        iVar6 = extraout_A1;
      }
      *(bool *)(unaff_A6 + 0x80) = extraout_D1w < uVar5;
    }
    if ((char)(-(-1 < (short)(*(short *)(iVar6 + 0x3c) - *(short *)(unaff_A6 + 0x3c))) ^
              -((*(byte *)(unaff_A6 + 0x15) & 1) != 0)) < '\0') {
      *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xfe;
    }
    else {
      *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 1;
    }
  }
  if (*(short *)(unaff_A6 + 0x70) != 0) {
    *(short *)(unaff_A6 + 0x70) = *(short *)(unaff_A6 + 0x70) + -1;
  }
  *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xfd;
  candidate_260c8();
  uVar5 = candidate_29c6();
  if (((uVar5 & 0xff) < extraout_D1w_00) && (uVar5 >> 8 < extraout_D1w_00)) {
    *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 2;
    sVar2 = 0x30;
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      sVar2 = -0x30;
    }
    candidate_510c(CONCAT22(*(undefined2 *)(unaff_A6 + 0x40),*(short *)(unaff_A6 + 0x3c) + sVar2));
    candidate_aeb40();
  }
  sVar2 = *(short *)(unaff_A6 + 0x6e);
  if (sVar2 != 0) {
    bVar7 = (short)(sVar2 + -1) < 0;
    if (sVar2 == 1) {
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x10;
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x20;
    }
    else {
      candidate_29c6();
      if (bVar7) {
        *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xdf;
        *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x10;
      }
      else {
        *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x20;
        *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xef;
      }
    }
  }
  sVar2 = *(short *)(unaff_A6 + 0x74);
  if (sVar2 != 0) {
    sVar2 = sVar2 + -1;
    *(short *)(unaff_A6 + 0x74) = sVar2;
  }
  bVar7 = sVar2 < 0;
  candidate_29c6();
  if (bVar7) {
    *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) ^ 0x80;
  }
  *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 8;
  if (0xff < (ushort)(*(short *)(unaff_A6 + 0x3c) - 0x20U)) {
    *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xf7;
  }
  if (*(short *)(unaff_A6 + 0x92) != 0) {
    sVar2 = *(short *)(unaff_A6 + 0x92) >> 6;
    if (sVar2 == 0) {
      sVar2 = 1;
    }
    *(short *)(unaff_A6 + 0x92) = *(short *)(unaff_A6 + 0x92) - sVar2;
  }
  *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xdf;
  *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xef;
  *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xbf;
  *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) & 0xfb;
  candidate_148c2();
  if ((unaff_D6 & 0x10) == 0) {
    if ((unaff_D6 & 0x100) != 0) {
      *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 0x10;
    }
    if ((unaff_D6 & 4) != 0) {
      *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 0x20;
    }
  }
  else {
    *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 0x40;
  }
  candidate_14850();
  if ((unaff_D6 & 4) != 0) {
    *(byte *)(unaff_A6 + 0x7c) = *(byte *)(unaff_A6 + 0x7c) | 4;
  }
  return;
}
