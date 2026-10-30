/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x014c2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x000152e4) */
/* WARNING: Removing unreachable block (ram,0x0001528e) */
/* WARNING: Removing unreachable block (ram,0x00015306) */

undefined2 candidate_14c2e(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  undefined1 uVar8;
  byte *pbVar9;
  byte bVar10;
  short sVar11;
  undefined2 uVar12;
  int iVar13;
  int iVar14;
  short unaff_D5w;
  byte unaff_D6b;
  int unaff_D7;
  undefined4 in_A0;
  undefined4 extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar15;

  *(undefined4 *)(unaff_A5 + 0x42a6) = in_A0;
  candidate_128da();
  *(undefined4 *)(unaff_A6 + 0x62) = *(undefined4 *)(unaff_A6 + 0x30);
  BYTE_ARRAY_000042aa[unaff_A5] = 0;
  uVar1 = *(undefined4 *)(unaff_A6 + 0x62);
  bVar10 = *(byte *)(unaff_A6 + 0x15);
  *(byte *)(unaff_A6 + 0x15) = bVar10 & 0xf7;
  if (((bVar10 & 8) != 0) && (-1 < *(int *)(unaff_A6 + 0x62))) {
    *(undefined4 *)(unaff_A6 + 0x62) = 0;
  }
  bVar10 = *(byte *)(unaff_A6 + 0x15);
  *(byte *)(unaff_A6 + 0x15) = bVar10 & 0xef;
  if (((bVar10 & 0x10) != 0) && (*(int *)(unaff_A6 + 0x62) < 0)) {
    *(undefined4 *)(unaff_A6 + 0x62) = 0;
  }
  pbVar9 = BYTE_ARRAY_000042aa + unaff_A5 + 2;
  pbVar9[0] = 0;
  pbVar9[1] = 0;
  pbVar9[2] = 0;
  pbVar9[3] = 0;
  pbVar9 = BYTE_ARRAY_000042aa + unaff_A5 + 6;
  pbVar9[0] = 0;
  pbVar9[1] = 0;
  pbVar9[2] = 0;
  pbVar9[3] = 0;
  pbVar9 = BYTE_ARRAY_000042aa + unaff_A5 + 10;
  pbVar9[0] = 0;
  pbVar9[1] = 0;
  pbVar9[2] = 0;
  pbVar9[3] = 0;
  pbVar9 = BYTE_ARRAY_000042aa + unaff_A5 + 0xe;
  pbVar9[0] = 0;
  pbVar9[1] = 0;
  pbVar9[2] = 0;
  pbVar9[3] = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(undefined1 *)(unaff_A5 + 0x4008) = 0xff;
  candidate_13c1a();
  if ((*(byte *)(unaff_A6 + 0x5f) & 0x10) == 0) {
    if ((*(byte *)(unaff_A6 + 0x5f) & 0x20) == 0) {
      *(undefined4 *)(unaff_A5 + 0x4004) = *(undefined4 *)(unaff_D5w + 0x14522);
    }
  }
  else {
    *(undefined4 *)(unaff_A5 + 0x4004) = 0xfffffffe;
  }
  candidate_1abc8();
  iVar2 = *(int *)(unaff_A5 + 0x42a6);
  *(undefined4 *)(iVar2 + 0x38) = 0;
  *(undefined4 *)(iVar2 + 0x28) = *(undefined4 *)(unaff_A6 + 0x44);
  *(undefined4 *)(iVar2 + 0x2c) = *(undefined4 *)(unaff_A6 + 0x48);
  if (BYTE_ARRAY_000042aa[unaff_A5] == 0) {
    iVar13 = *(int *)(unaff_A6 + 0x62);
    iVar14 = *(int *)(unaff_A6 + 0x34);
  }
  else {
    iVar13 = 0;
    iVar14 = 0;
  }
  iVar3 = *(int *)(unaff_A6 + 0x48);
  iVar4 = *(int *)(unaff_A6 + 0x40);
  *(int *)(iVar2 + 0x30) = *(int *)(unaff_A6 + 0x3c) + (iVar13 - *(int *)(unaff_A6 + 0x44));
  *(int *)(iVar2 + 0x34) = iVar4 + (iVar14 - iVar3);
  candidate_1abc8();
  *(undefined1 *)(iVar2 + 0x15) = *(undefined1 *)(unaff_A6 + 0x15);
  *(undefined1 *)(iVar2 + 0x5f) = *(undefined1 *)(unaff_A6 + 0x5f);
  *(undefined4 *)(iVar2 + 0x62) = *(undefined4 *)(unaff_A6 + 0x62);
  if (*(short *)(iVar2 + 0x8a) != 0) {
    iVar13 = *(int *)(iVar2 + 0x8c) + 0x8000;
    *(int *)(iVar2 + 0x8c) = iVar13;
    *(int *)(iVar2 + 0x34) = *(int *)(iVar2 + 0x34) - iVar13;
  }
  sVar11 = CONCAT11(-((*(byte *)(iVar2 + 0x93) & 5) != 0) >> 7,-((*(byte *)(iVar2 + 0x92) & 5) != 0)
                   );
  if (sVar11 == 0) {
    *(undefined2 *)(iVar2 + 0x72) = *(undefined2 *)(iVar2 + 0x88);
    if ((*(byte *)(iVar2 + 0x94) & 5) == 0) {
      candidate_14b90();
      goto LAB_00015076;
    }
  }
  else {
    if (sVar11 == -1) {
      uVar12 = candidate_14b6e();
      *(undefined2 *)(iVar2 + 0x90) = uVar12;
      if ((-1 < (short)((*(short *)(iVar2 + 0x84) - *(short *)(iVar2 + 0x86)) + -0x1000)) &&
         (*(int *)(iVar2 + 0x62) != 0)) {
        if (*(int *)(iVar2 + 0x62) < 0) {
          *(undefined2 *)(iVar2 + 0x90) = *(undefined2 *)(iVar2 + 0x86);
        }
        else {
          *(undefined2 *)(iVar2 + 0x90) = *(undefined2 *)(iVar2 + 0x84);
        }
      }
      candidate_14b90();
      *(undefined2 *)(iVar2 + 0x72) = *(undefined2 *)(iVar2 + 0x6e);
      if ((int)((uint)(ushort)(*(short *)(iVar2 + 0x84) - *(short *)(iVar2 + 0x86)) << 0x10) < 0) {
        uVar15 = candidate_2e98();
        iVar13 = *(int *)(iVar2 + 0x80);
        iVar14 = *(int *)(iVar2 + 0x40);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10) =
             ((*(int *)(iVar2 + 0x7c) + (int)((ulonglong)uVar15 >> 0x20)) - *(int *)(iVar2 + 0x3c))
             + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe) =
             ((iVar13 + (int)uVar15) - iVar14) + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe);
        uVar15 = candidate_2e98();
        iVar13 = *(int *)(iVar2 + 0x78);
        iVar14 = *(int *)(iVar2 + 0x40);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10) =
             ((*(int *)(iVar2 + 0x74) + (int)((ulonglong)uVar15 >> 0x20)) - *(int *)(iVar2 + 0x3c))
             + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe) =
             ((iVar13 + (int)uVar15) - iVar14) + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe);
      }
      else {
        uVar15 = candidate_2e98();
        iVar13 = (int)((ulonglong)uVar15 >> 0x20);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 2) =
             iVar13 + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 2);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 6) =
             (int)uVar15 + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 6);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10) =
             iVar13 + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe) =
             (int)uVar15 + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe);
      }
      goto LAB_00015076;
    }
    if (sVar11 == 0xff) {
      *(undefined2 *)(iVar2 + 0x72) = *(undefined2 *)(iVar2 + 0x84);
      if (*(int *)(iVar2 + 0x62) < 1) {
        *(undefined2 *)(iVar2 + 0x90) = 0x2000;
        if (*(short *)(iVar2 + 0x6e) == 0x2000) {
          *(undefined4 *)(iVar2 + 0x74) = *(undefined4 *)(iVar2 + 0x3c);
          *(undefined4 *)(iVar2 + 0x78) = *(undefined4 *)(iVar2 + 0x40);
          *(short *)(iVar2 + 0x34) = *(short *)(iVar2 + 0x34) + -1;
          goto LAB_00015076;
        }
      }
      else {
        if ((*(byte *)(iVar2 + 0x94) & 4) != 0) goto LAB_00015068;
        *(undefined2 *)(iVar2 + 0x90) = *(undefined2 *)(iVar2 + 0x84);
      }
      unaff_D6b = *(byte *)(iVar2 + 0x94) & 5;
      if ((*(byte *)(iVar2 + 0x94) & 5) == 0) {
        candidate_14b90();
        uVar15 = candidate_2e98();
        *(int *)(iVar2 + 0x30) =
             *(int *)(iVar2 + 0x30) -
             (*(int *)(iVar2 + 0x3c) + ((int)((ulonglong)uVar15 >> 0x20) - *(int *)(iVar2 + 0x74)));
        *(int *)(iVar2 + 0x34) =
             *(int *)(iVar2 + 0x34) -
             (*(int *)(iVar2 + 0x40) + ((int)uVar15 - *(int *)(iVar2 + 0x78)));
        goto LAB_00015076;
      }
      if ((int)((uint)(ushort)(*(short *)(iVar2 + 0x88) - *(short *)(iVar2 + 0x84)) << 0x10) < 0) {
        uVar12 = candidate_14b6e();
        *(undefined2 *)(iVar2 + 0x90) = uVar12;
        candidate_14b90();
        uVar15 = candidate_2e98();
        iVar13 = *(int *)(iVar2 + 0x78);
        iVar14 = *(int *)(iVar2 + 0x40);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 2) =
             ((*(int *)(iVar2 + 0x74) + (int)((ulonglong)uVar15 >> 0x20)) - *(int *)(iVar2 + 0x3c))
             + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 2);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 6) =
             ((iVar13 + (int)uVar15) - iVar14) + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 6);
        goto LAB_00015076;
      }
    }
    else if (sVar11 == -0x100) {
      *(undefined2 *)(iVar2 + 0x72) = *(undefined2 *)(iVar2 + 0x86);
      if (*(int *)(iVar2 + 0x62) < 0) {
        if ((*(byte *)(iVar2 + 0x94) & 4) != 0) goto LAB_00015068;
        *(undefined2 *)(iVar2 + 0x90) = *(undefined2 *)(iVar2 + 0x86);
      }
      else {
        *(undefined2 *)(iVar2 + 0x90) = 0xe000;
        if (*(short *)(iVar2 + 0x6e) == -0x2000) {
          *(undefined4 *)(iVar2 + 0x7c) = *(undefined4 *)(iVar2 + 0x3c);
          *(undefined4 *)(iVar2 + 0x80) = *(undefined4 *)(iVar2 + 0x40);
          *(short *)(iVar2 + 0x34) = *(short *)(iVar2 + 0x34) + -1;
          goto LAB_00015076;
        }
      }
      unaff_D6b = *(byte *)(iVar2 + 0x94) & 5;
      if ((*(byte *)(iVar2 + 0x94) & 5) == 0) {
        candidate_14b90();
        uVar15 = candidate_2e98();
        *(int *)(iVar2 + 0x30) =
             *(int *)(iVar2 + 0x30) -
             (*(int *)(iVar2 + 0x3c) + ((int)((ulonglong)uVar15 >> 0x20) - *(int *)(iVar2 + 0x7c)));
        *(int *)(iVar2 + 0x34) =
             *(int *)(iVar2 + 0x34) -
             (*(int *)(iVar2 + 0x40) + ((int)uVar15 - *(int *)(iVar2 + 0x80)));
        goto LAB_00015076;
      }
      if ((int)((uint)(ushort)(*(short *)(iVar2 + 0x86) - *(short *)(iVar2 + 0x88)) << 0x10) < 0) {
        uVar12 = candidate_14b6e();
        *(undefined2 *)(iVar2 + 0x90) = uVar12;
        candidate_14b90();
        uVar15 = candidate_2e98();
        iVar13 = *(int *)(iVar2 + 0x80);
        iVar14 = *(int *)(iVar2 + 0x40);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10) =
             ((*(int *)(iVar2 + 0x7c) + (int)((ulonglong)uVar15 >> 0x20)) - *(int *)(iVar2 + 0x3c))
             + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 10);
        *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe) =
             ((iVar13 + (int)uVar15) - iVar14) + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe);
        goto LAB_00015076;
      }
    }
  }
LAB_00015068:
  *(undefined2 *)(iVar2 + 0x90) = *(undefined2 *)(iVar2 + 0x88);
  candidate_14b90();
LAB_00015076:
  if (BYTE_ARRAY_000042aa[unaff_A5] != 0) {
    uVar15 = candidate_2e98();
    iVar13 = (int)((ulonglong)uVar15 >> 0x20);
    *(int *)(iVar2 + 0x30) = iVar13 + *(int *)(iVar2 + 0x30);
    *(int *)(iVar2 + 0x34) = (int)uVar15 + *(int *)(iVar2 + 0x34);
    if (*(short *)(iVar2 + 0x8a) != 0) {
      iVar13 = iVar13 >> 1;
      if (*(int *)(iVar2 + 0x62) != 0) {
        if (*(int *)(iVar2 + 0x62) < 0) {
          *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe) =
               iVar13 + *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 0xe);
        }
        else {
          *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 6) =
               *(int *)(BYTE_ARRAY_000042aa + unaff_A5 + 6) - iVar13;
        }
      }
    }
  }
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  uVar8 = *(undefined1 *)(unaff_A5 + 0x400b);
  *(undefined1 *)(unaff_A5 + 0x400b) = 0xff;
  uVar5 = *(undefined4 *)(unaff_A5 + 0x4004);
  if (*(int *)(iVar2 + 0x62) < 1) {
    *(undefined4 *)(unaff_A5 + 0x4004) = 0xfffffffe;
  }
  else {
    unaff_D6b = *(byte *)(iVar2 + 0x92) | *(byte *)(iVar2 + 0x94);
    if ((unaff_D6b & 0x20) != 0) {
      *(undefined4 *)(iVar2 + 0x74) = *(undefined4 *)(iVar2 + 0x3c);
      *(undefined4 *)(iVar2 + 0x78) = *(undefined4 *)(iVar2 + 0x40);
    }
  }
  candidate_2e98();
  uVar15 = candidate_14b12();
  *(undefined8 *)(iVar2 + 0x74) = uVar15;
  *(byte *)(iVar2 + 0x92) = unaff_D6b;
  if (((unaff_D6b & 4) != 0) && (*(int *)(iVar2 + 0x62) != 0)) {
    uVar12 = (undefined2)((uint)unaff_D7 >> 0x10);
    if (*(int *)(iVar2 + 0x62) < 0) {
      unaff_D7 = CONCAT22(uVar12,0xe000);
    }
    else {
      unaff_D7 = CONCAT22(uVar12,0x2000);
    }
  }
  *(short *)(iVar2 + 0x84) = (short)unaff_D7;
  *(undefined4 *)(unaff_A5 + 0x4004) = uVar5;
  if (*(int *)(iVar2 + 0x62) < 0) {
    unaff_D6b = *(byte *)(iVar2 + 0x93) | *(byte *)(iVar2 + 0x94);
    if ((unaff_D6b & 0x20) != 0) {
      *(undefined4 *)(iVar2 + 0x7c) = *(undefined4 *)(iVar2 + 0x3c);
      *(undefined4 *)(iVar2 + 0x80) = *(undefined4 *)(iVar2 + 0x40);
    }
  }
  else {
    *(undefined4 *)(unaff_A5 + 0x4004) = 0xfffffffe;
  }
  candidate_2e98();
  uVar15 = candidate_14b12();
  *(undefined8 *)(iVar2 + 0x7c) = uVar15;
  *(byte *)(iVar2 + 0x93) = unaff_D6b;
  if (((unaff_D6b & 4) != 0) && (*(int *)(iVar2 + 0x62) != 0)) {
    uVar12 = (undefined2)((uint)unaff_D7 >> 0x10);
    if (*(int *)(iVar2 + 0x62) < 0) {
      unaff_D7 = CONCAT22(uVar12,0xe000);
    }
    else {
      unaff_D7 = CONCAT22(uVar12,0x2000);
    }
  }
  *(short *)(iVar2 + 0x86) = (short)unaff_D7;
  *(undefined4 *)(unaff_A5 + 0x4004) = uVar5;
  *(undefined1 *)(unaff_A5 + 0x400b) = uVar8;
  if ((*(byte *)(iVar2 + 0x15) & 0x40) == 0) {
    unaff_D7 = *(int *)(iVar2 + 0x34) >> 7;
    if ((*(byte *)(iVar2 + 0x92) & 5) != 0) {
      *(short *)(iVar2 + 0x70) = *(short *)(iVar2 + 0x70) - (short)unaff_D7;
    }
    if ((*(byte *)(iVar2 + 0x93) & 5) != 0) {
      *(short *)(iVar2 + 0x70) = (short)unaff_D7 + *(short *)(iVar2 + 0x70);
    }
  }
  iVar13 = *(int *)(iVar2 + 0x74);
  iVar14 = *(int *)(iVar2 + 0x78);
  iVar3 = *(int *)(iVar2 + 0x7c);
  iVar4 = *(int *)(iVar2 + 0x80);
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0x7f;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xbf;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xfe;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xfb;
  *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xfd;
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  iVar6 = *(int *)(unaff_A6 + 0x44);
  iVar7 = *(int *)(unaff_A6 + 0x48);
  bVar10 = *(byte *)(iVar2 + 0x93) | *(byte *)(iVar2 + 0x92);
  if ((bVar10 & 0x20) != 0) {
    *(byte *)(unaff_A5 + 0x4009) = BYTE_ARRAY_000042aa[unaff_A5];
  }
  uVar15 = candidate_13968();
  *(undefined8 *)(unaff_A6 + 0x28) = uVar15;
  if (BYTE_ARRAY_000042aa[unaff_A5] != 0) {
    *(int *)(unaff_A6 + 0x30) = (iVar3 + iVar13 >> 1) - iVar6;
    *(int *)(unaff_A6 + 0x34) = (iVar4 + iVar14 >> 1) - iVar7;
  }
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A0;
  iVar2 = *(int *)(unaff_A5 + 0x42a6);
  *(byte *)(iVar2 + 0x94) = bVar10 & 0x20;
  sVar11 = (short)unaff_D7 + -0x4000;
  if (*(int *)(unaff_A6 + 0x62) != 0) {
    if (*(int *)(unaff_A6 + 0x62) < 0) {
      if ((bVar10 & 0x20) != 0) {
        *(byte *)(iVar2 + 0x93) = *(byte *)(iVar2 + 0x93) & 0xfe;
      }
    }
    else if ((bVar10 & 0x20) != 0) {
      *(byte *)(iVar2 + 0x92) = *(byte *)(iVar2 + 0x92) & 0xfe;
    }
  }
  *(short *)(iVar2 + 0x88) = sVar11;
  *(short *)(unaff_A6 + 0x6c) = (short)(CONCAT22((short)((uint)unaff_D7 >> 0x10),sVar11) >> 8);
  bVar10 = *(byte *)(iVar2 + 0x92) | *(byte *)(iVar2 + 0x93);
  if ((bVar10 & 1) == 0) {
    if (BYTE_ARRAY_000042aa[unaff_A5] == 0) {
      *(undefined2 *)(iVar2 + 0x8a) = 0;
    }
    else if (*(short *)(iVar2 + 0x8a) == 0) {
      *(undefined2 *)(iVar2 + 0x8a) = 5;
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
    }
    else {
      sVar11 = *(short *)(iVar2 + 0x8a) + -1;
      *(short *)(iVar2 + 0x8a) = sVar11;
      if (sVar11 != 0) {
        *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
      }
    }
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
    *(undefined4 *)(iVar2 + 0x8c) = 0;
    *(undefined2 *)(iVar2 + 0x8a) = 0;
  }
  if ((bVar10 & 2) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x80;
  }
  if (*(int *)(unaff_A6 + 0x62) != 0) {
    if (*(int *)(unaff_A6 + 0x62) < 0) {
      if ((*(byte *)(iVar2 + 0x93) & 4) != 0) {
        *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 1;
      }
    }
    else if ((*(byte *)(iVar2 + 0x92) & 4) != 0) {
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 1;
    }
  }
  uVar12 = *(undefined2 *)(iVar2 + 0x6e);
  candidate_1abc8();
  candidate_1ad8c();
  *(undefined4 *)(unaff_A6 + 0x62) = uVar1;
  return uVar12;
}
