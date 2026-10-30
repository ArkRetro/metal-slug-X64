/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b05fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b05fe(void)

{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  char cVar10;
  ushort uVar8;
  short sVar9;
  undefined4 uVar7;
  ushort extraout_D1w;
  ushort extraout_D1w_00;
  short extraout_D1w_01;
  ushort uVar11;
  undefined4 unaff_D2;
  int iVar12;
  short unaff_D3w;
  short sVar13;
  uint unaff_D6;
  int extraout_A0;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar14;
  undefined1 uVar15;

  candidate_b05dc();
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x80;
  candidate_4256();
  *unaff_A6 = 0xb077a;
  uVar15 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    uVar7 = 0x28000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar7 = 0xfffd8000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar7;
    unaff_A6[0xc] = uVar7;
    candidate_4256();
    *unaff_A6 = 0xb086e;
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x62) - unaff_A6[0xc] >> 5) + unaff_A6[0xc];
    uVar5 = -0x100000 - unaff_A6[0xd] >> 5;
    uVar2 = unaff_A6[0xd];
    uVar15 = CARRY4(uVar5,uVar2);
    unaff_A6[0xd] = uVar5 + uVar2;
    func_0x000143e4();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_b08ac;
      uVar15 = 0;
    }
  }
  else {
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar15) {
      *unaff_A6 = 0xb063e;
      uVar15 = 0;
    }
  }
  candidate_19504();
  if ((bool)uVar15) {
    candidate_4126();
    iVar12 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar12 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  uVar7 = candidate_1903a();
  sVar9 = extraout_D1w_01 >> 1;
  if (sVar9 < 0) {
    sVar9 = -sVar9;
  }
  uVar8 = (short)uVar7 - *(short *)(unaff_A6 + 0x21);
  uVar6 = (ushort)((uint)uVar7 >> 0x10);
  if ((int)((uint)uVar8 << 0x10) < 0) {
    sVar9 = -sVar9;
  }
  sVar13 = unaff_D3w >> 1;
  if (sVar13 < 0) {
    sVar13 = -sVar13;
  }
  uVar4 = (short)unaff_D2 - *(short *)(unaff_A6 + 0x21);
  uVar11 = (ushort)((uint)unaff_D2 >> 0x10);
  if ((int)((uint)uVar4 << 0x10) < 0) {
    sVar13 = -sVar13;
  }
  iVar12 = ((int)(CONCAT22(uVar6,sVar9 + uVar8 + -0x8000) << 0x10 | (uint)uVar6) >> 8) +
           ((int)(CONCAT22(uVar11,sVar13 + uVar4 + -0x8000) << 0x10 | (uint)uVar11) >> 8);
  if (*(short *)((int)unaff_A6 + 0x92) != 0) {
    iVar12 = ((int)((uint)(ushort)(((*(short *)(unaff_A5 + 0x697c) +
                                    *(short *)((int)unaff_A6 + 0x8e)) - *(short *)(unaff_A6 + 0xf))
                                  + 0x8000) << 0x10) >> 6) + iVar12;
  }
  sVar9 = *(short *)(unaff_A6 + 0x23) + 0x77;
  *(short *)(unaff_A6 + 0x23) = sVar9;
  iVar3 = unaff_A6[0x22];
  if (-1 < iVar3) {
    sVar9 = -sVar9;
  }
  iVar12 = unaff_A6[0x22] +
           ((iVar12 >> 6) - CONCAT22((short)(iVar3 >> 0x16),sVar9 * 2 + (short)(iVar3 >> 6)));
  unaff_A6[0x22] = iVar12;
  if (!SCARRY4(unaff_A6[0x21],iVar12)) {
    unaff_A6[0x21] = unaff_A6[0x21] + iVar12;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_406b6();
  }
  cVar10 = candidate_1ae8e();
  if (cVar10 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  else {
    if (cVar10 == '\x01') {
      *(undefined1 *)(unaff_A6 + 0x20) = 0;
      iVar12 = unaff_A5 + -0x6b3c;
    }
    else if (cVar10 == '\x02') {
      *(undefined1 *)(unaff_A6 + 0x20) = 1;
      iVar12 = unaff_A5 + -0x6a8c;
    }
    else {
      uVar8 = candidate_29f2();
      iVar12 = extraout_A0;
      if (extraout_D1w < uVar8) {
        iVar12 = extraout_A1;
      }
      *(bool *)(unaff_A6 + 0x20) = extraout_D1w < uVar8;
    }
    if ((char)(-(-1 < (short)(*(short *)(iVar12 + 0x3c) - *(short *)(unaff_A6 + 0xf))) ^
              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) < '\0') {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfe;
    }
    else {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 1;
    }
  }
  if (*(short *)(unaff_A6 + 0x1c) != 0) {
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + -1;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfd;
  candidate_260c8();
  uVar8 = candidate_29c6();
  if (((uVar8 & 0xff) < extraout_D1w_00) && (uVar8 >> 8 < extraout_D1w_00)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 2;
    sVar9 = 0x30;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar9 = -0x30;
    }
    candidate_510c(CONCAT22(*(undefined2 *)(unaff_A6 + 0x10),*(short *)(unaff_A6 + 0xf) + sVar9));
    candidate_aeb40();
  }
  sVar9 = *(short *)((int)unaff_A6 + 0x6e);
  if (sVar9 != 0) {
    bVar14 = (short)(sVar9 + -1) < 0;
    if (sVar9 == 1) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
    }
    else {
      candidate_29c6();
      if (bVar14) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
      }
      else {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
      }
    }
  }
  sVar9 = *(short *)(unaff_A6 + 0x1d);
  if (sVar9 != 0) {
    sVar9 = sVar9 + -1;
    *(short *)(unaff_A6 + 0x1d) = sVar9;
  }
  bVar14 = sVar9 < 0;
  candidate_29c6();
  if (bVar14) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) ^ 0x80;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
  if (0xff < (ushort)(*(short *)(unaff_A6 + 0xf) - 0x20U)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xf7;
  }
  if (*(short *)((int)unaff_A6 + 0x92) != 0) {
    sVar9 = *(short *)((int)unaff_A6 + 0x92) >> 6;
    if (sVar9 == 0) {
      sVar9 = 1;
    }
    *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) - sVar9;
  }
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xdf;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xef;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xbf;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) & 0xfb;
  candidate_148c2();
  if ((unaff_D6 & 0x10) == 0) {
    if ((unaff_D6 & 0x100) != 0) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x10;
    }
    if ((unaff_D6 & 4) != 0) {
      *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x20;
    }
  }
  else {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 0x40;
  }
  candidate_14850();
  if ((unaff_D6 & 4) != 0) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 4;
  }
  return;
}
