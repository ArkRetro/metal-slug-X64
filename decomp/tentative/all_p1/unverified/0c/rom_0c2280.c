/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c2280.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c2280(void)

{
  code *pcVar1;
  byte bVar2;
  ushort uVar3;
  uint uVar4;
  ushort uVar5;
  undefined2 extraout_D0u;
  byte bVar11;
  ushort uVar9;
  short sVar10;
  uint uVar6;
  undefined4 uVar7;
  int iVar8;
  char cVar12;
  undefined1 extraout_D1b;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  undefined3 uVar13;
  uint unaff_D7;
  int iVar14;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar15;
  bool bVar16;

  candidate_c0f16();
  candidate_c0fb6();
  do {
    *unaff_A6 = 0xc22b6;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c2706;
    uVar6 = candidate_c0fea();
    if (*(char *)(unaff_A6 + 0x1b) != '\f') {
      if ((uVar6 & 0x20) != 0) goto LAB_000c2654;
      if ((uVar6 & 2) == 0) goto LAB_000c1e34;
    }
    if ((uVar6 & 4) == 0) {
      if ((uVar6 & 8) == 0) {
        if ((uVar6 & 0x40) != 0) goto LAB_000c2458;
        if ((uVar6 & 0x10) != 0) goto LAB_000c246a;
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        uVar15 = 0;
        candidate_14594();
        candidate_427a();
        if ((bool)uVar15) {
          *unaff_A6 = 0xc22a4;
        }
      }
      else {
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) goto LAB_000c23ca;
LAB_000c2338:
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
        candidate_c0f32();
        candidate_c0f16();
        *(byte *)((int)unaff_A6 + 0x9b) = *(byte *)((int)unaff_A6 + 0x9b) & 0xef;
        *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) & 0xef;
        *(byte *)((int)unaff_A6 + 0x9b) = *(byte *)((int)unaff_A6 + 0x9b) & 0xbf;
        *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) & 0xbf;
        *unaff_A6 = 0xc2368;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c2706;
        uVar15 = *(byte *)(unaff_A6 + 0x1b) < 0xc;
        if (*(byte *)(unaff_A6 + 0x1b) != 0xc) {
          uVar6 = candidate_c0fea();
          if ((uVar6 & 2) == 0) goto LAB_000c1e34;
          if ((uVar6 & 0x20) != 0) goto LAB_000c2654;
        }
        candidate_14594();
        candidate_427a();
        if ((bool)uVar15) {
          *unaff_A6 = 0xc22a4;
          if ((*(byte *)((int)unaff_A6 + 0x9b) & 0x10) != 0) {
            *unaff_A6 = 0xc246a;
          }
          if ((*(byte *)((int)unaff_A6 + 0x9b) & 0x40) != 0) {
            *unaff_A6 = 0xc2458;
          }
        }
      }
      goto LAB_000c32f4;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) goto LAB_000c2338;
LAB_000c23ca:
    candidate_c0f32();
    candidate_c0f16();
    uVar7 = 0x12000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      uVar7 = 0xfffee000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar7;
    *unaff_A6 = 0xc23f2;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c2706;
    bVar11 = candidate_c0fea();
    if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) goto LAB_000c2338;
    unaff_D7 = CONCAT31((int3)(unaff_D7 >> 8),bVar11) & 0xffffff0c;
    if ((bVar11 & 0xc) != 0) break;
    candidate_c0f32();
    candidate_c0f16();
  } while( true );
  uVar15 = *(byte *)(unaff_A6 + 0x1b) < 0xc;
  if (*(byte *)(unaff_A6 + 0x1b) != 0xc) {
    if ((bVar11 & 0x20) != 0) goto LAB_000c2654;
    if ((bVar11 & 2) == 0) goto LAB_000c1e34;
  }
  if ((bVar11 & 0x40) != 0) goto LAB_000c2458;
  if ((bVar11 & 0x10) != 0) goto LAB_000c246a;
  candidate_14594();
  candidate_427a();
  if ((bool)uVar15) {
    *unaff_A6 = 0xc22a4;
  }
LAB_000c32f4:
  candidate_19432();
  *(bool *)((int)unaff_A6 + 0x79) = *(char *)(unaff_A6 + 0x24) == '\x03';
  (*(code *)unaff_A6[0x1f])();
  if (*(char *)((int)unaff_A6 + 0x5a) == '\x0f') {
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
  }
  bVar11 = 0;
  if ((*(char *)((int)unaff_A6 + 0x93) == '\0') || (*(char *)((int)unaff_A6 + 0x93) == '\x01')) {
    bVar11 = -(*(char *)(unaff_A6 + 0x1b) == '\t') | -(*(char *)(unaff_A6 + 0x1b) == '\a');
  }
  *(byte *)((int)unaff_A6 + 0x9d) = bVar11;
  bVar16 = false;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
LAB_000c337a:
    if (((*(short *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c724) == 0) ||
        ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) &&
       (candidate_c0ee2(), *(char *)((int)unaff_A6 + 0x93) == '\x01')) {
      *(undefined1 *)(unaff_A6 + 0x25) = 0;
    }
    bVar16 = *(byte *)((int)unaff_A6 + 0x93) < 2;
    if (*(byte *)((int)unaff_A6 + 0x93) == 2) {
      candidate_c0bae();
      if (!bVar16) {
        bVar16 = false;
        candidate_ca2f8();
        if (!bVar16) {
          *(undefined2 *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c728) = 0;
          *(undefined1 *)(unaff_A6 + 0x25) = 0;
          goto LAB_000c3402;
        }
      }
      sVar10 = *(short *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c728);
      if ((sVar10 != 0) &&
         (sVar10 = sVar10 + -1,
         *(short *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c728) = sVar10, sVar10 == 0)) {
        *(undefined1 *)(unaff_A6 + 0x25) = 0;
      }
    }
LAB_000c3402:
    if (((*(byte *)((int)unaff_A6 + 0x9a) & 0x80) != 0) || (3 < *(byte *)(unaff_A6 + 0x25)))
    goto LAB_000c35d0;
    bVar11 = *(byte *)((int)unaff_A6 + 0x93);
    bVar2 = BYTE_ARRAY_000c32e4[(short)((ushort)*(byte *)(unaff_A6 + 0x25) + (ushort)bVar11 * 4)];
    if ((char)bVar2 < '\0') goto LAB_000c35d0;
    if (bVar2 == 1) {
      if ((code *)unaff_A6[0x1f] == candidate_c381c) {
        if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) ||
           (uVar6 = candidate_c0fea(), (uVar6 & 2) == 0)) goto LAB_000c35d0;
        *(undefined1 *)((int)unaff_A6 + 0x99) = 6;
        *unaff_A6 = 0xc22a4;
      }
      else {
        *(undefined1 *)((int)unaff_A6 + 0x99) = 6;
        unaff_A6[0x1f] = candidate_c397e;
      }
    }
    else if (bVar2 == 3) {
      candidate_c0ee2();
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      *(undefined2 *)(unaff_A6 + 0x21) = 0x14;
      *unaff_A6 = candidate_c17a0;
    }
    else if (bVar2 == 2) {
      if (((*(byte *)((int)unaff_A6 + 0x9a) & 0x80) != 0) ||
         ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) goto LAB_000c35d0;
      *unaff_A6 = candidate_c1d70;
    }
    else {
      if ((bVar11 == 2) && ((*(byte *)((int)unaff_A6 + 0x9a) & 0x80) == 0)) {
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          if ((code *)unaff_A6[0x1f] == candidate_c381c) goto LAB_000c3524;
          *unaff_A6 = candidate_c1d82;
        }
        else {
          *unaff_A6 = candidate_c1d82;
        }
      }
      else {
LAB_000c3524:
        if (bVar11 == 3) {
          *unaff_A6 = candidate_c1740;
        }
        else if ((code *)unaff_A6[0x1f] == candidate_c381c) {
          if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) ||
             (uVar6 = candidate_c0fea(), (uVar6 & 2) == 0)) goto LAB_000c35d0;
          *unaff_A6 = candidate_c2280;
        }
        else {
          unaff_A6[0x1f] = candidate_c393c;
        }
      }
      *(undefined1 *)(*(short *)(unaff_A6 + 0x1c) + 0x10c71a) = 0;
    }
    if ((*(char *)((int)unaff_A6 + 0x93) == '\x01') && (*(char *)(unaff_A6 + 0x25) != '\x01')) {
      candidate_411c();
      *(undefined2 *)(extraout_A0_00 + 0x6e) = *(undefined2 *)((int)unaff_A6 + 0x6e);
      candidate_1ac94();
      candidate_1acf2();
    }
    *(undefined1 *)((int)unaff_A6 + 0x93) = *(undefined1 *)(unaff_A6 + 0x25);
    *(undefined1 *)(unaff_A6 + 0x25) = 0xff;
LAB_000c35d0:
    if (*(char *)((int)unaff_A6 + 0x99) != '\0') {
      *(char *)((int)unaff_A6 + 0x99) = *(char *)((int)unaff_A6 + 0x99) + -1;
      candidate_9d1e();
    }
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
    if (*(short *)(unaff_A6 + 0x21) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
      sVar10 = *(short *)(unaff_A6 + 0x21) + -1;
      *(short *)(unaff_A6 + 0x21) = sVar10;
      if (sVar10 == 0) {
        *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0xfb;
      }
      else if ((((*(byte *)((int)unaff_A6 + 0x9a) & 4) != 0) && (0xf < *(ushort *)(unaff_A6 + 0x21))
               ) && ((*(ushort *)(unaff_A6 + 0x21) & 1) != 0)) {
        *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
      }
    }
    if ((DAT_0010f000 & 0x80) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    bVar16 = false;
    candidate_ca2f8();
    if (bVar16) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x40) != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    }
    cVar12 = candidate_4c24();
    if (cVar12 == '\x10') {
      if (*(char *)((int)unaff_A6 + 0x93) == '\x03') {
        *unaff_A6 = 0xc2956;
      }
      else {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
        *(undefined1 *)(unaff_A6 + 0x25) = 3;
        candidate_c0ee2();
      }
    }
    else if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = 0xc2956;
    }
    if (*(short *)(unaff_A6 + 0x10) < *(short *)(unaff_A5 + 0x472c)) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
      *unaff_A6 = candidate_c2d6a;
    }
    if (*(char *)((int)unaff_A6 + 0x9e) != '\0') {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 6;
      *unaff_A6 = 0xc2956;
    }
    *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0xfe;
    cVar12 = candidate_49e8();
    if ((cVar12 == '\x05') || (cVar12 == '\x04')) {
      *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) | 1;
    }
    *(undefined1 *)(unaff_A6 + 0x27) = 0xff;
    return;
  }
  candidate_aa458();
  if (bVar16) {
    bVar11 = *(byte *)((int)unaff_A6 + 0x9a);
    *(byte *)((int)unaff_A6 + 0x9a) = bVar11 | 0x80;
    if ((bVar11 & 0x80) == 0) {
      *unaff_A6 = candidate_c3078;
    }
    goto LAB_000c337a;
  }
  bVar11 = *(byte *)((int)unaff_A6 + 0x9a);
  *(byte *)((int)unaff_A6 + 0x9a) = bVar11 & 0x7f;
  if ((bVar11 & 0x80) == 0) goto LAB_000c337a;
  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0x7f;
  cVar12 = candidate_aa5ac();
  bVar16 = false;
  if (cVar12 != '\0') {
    if (cVar12 == '\x01') goto LAB_000c3232;
    if (cVar12 == '\x02') {
      unaff_A6[0xc] = 0;
      unaff_A6[0xd] = 0x20000;
      goto LAB_000c271c;
    }
    if (cVar12 == '\x03') {
LAB_000c1f4a:
      if ((*(char *)((int)unaff_A6 + 0x93) == '\x02') && (0x1f < *(byte *)((int)unaff_A6 + 0x86))) {
        *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
        unaff_A6[0x14] = 0x2d0aa8;
        goto LAB_000c1f24;
      }
      *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) | 0x20;
      *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
      *(undefined1 *)((int)unaff_A6 + 0x7a) = 0xff;
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8834;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8834;
      unaff_A6[0x14] = 0x2d0aa8;
      *(undefined1 *)(unaff_A6 + 0x24) = 0;
      candidate_c0f32();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
      *unaff_A6 = 0xc1fa6;
LAB_000c1fa6:
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
      if (*(char *)(unaff_A6 + 0x1b) != '\f') {
        uVar6 = candidate_c0fea();
        if ((uVar6 & 4) != 0) goto LAB_000c285c;
        if ((uVar6 & 8) != 0) goto LAB_000c285c;
        if ((uVar6 & 0x20) != 0) goto LAB_000c2654;
        if ((uVar6 & 2) == 0) {
          if (((*(char *)((int)unaff_A6 + 0x9d) != '\0') &&
              ((*(byte *)((int)unaff_A6 + 0x9a) & 1) != 0)) &&
             (uVar6 = candidate_c0fea(), (uVar6 & 0x10) != 0)) {
            do {
              *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
              unaff_A6[0x14] = 0x2d0aa8;
              *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) ^ 2;
              candidate_c0f32();
              candidate_c0f16();
              *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
              *unaff_A6 = 0xc2198;
              uVar6 = candidate_c0fea();
              if ((uVar6 & 0x20) != 0) goto LAB_000c1f4a;
              if (*(char *)((int)unaff_A6 + 0x76) == '\0') {
                candidate_ca2ce();
                *(ushort *)(extraout_A0 + 4) = *(ushort *)(extraout_A0 + 4) | 1;
                if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) == 0) goto LAB_000c21f2;
              }
              else {
                if ((uVar6 & 2) != 0) goto LAB_000c2214;
                if (((uVar6 & 0x40) != 0) || ((uVar6 & 0x10) != 0)) goto LAB_000c1f4a;
                if (((uVar6 & 4) == 0) && ((uVar6 & 8) == 0)) {
LAB_000c21f2:
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                  uVar15 = 0;
                  candidate_14594();
                  candidate_427a();
                  if ((bool)uVar15) {
                    *unaff_A6 = 0xc1f4a;
                  }
                  goto LAB_000c32f4;
                }
              }
LAB_000c285c:
              while( true ) {
                unaff_A6[0x14] = 0x2d0aa8;
                *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
                candidate_c0f32();
                *(undefined2 *)((int)unaff_A6 + 0x26) = 0x1c1;
                *(undefined1 *)(unaff_A6 + 0x24) = 1;
                *(undefined2 *)(unaff_A6 + 0x20) = 0x3f;
                *unaff_A6 = 0xc2888;
                if (*(char *)(unaff_A6 + 0x1b) == '\r') {
                  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x237;
                }
                bVar11 = *(byte *)((int)unaff_A6 + 0x86);
                *(byte *)((int)unaff_A6 + 0x86) = bVar11 + 1;
                *(byte *)((int)unaff_A6 + 0x86) = -(0xfe < bVar11) | *(byte *)((int)unaff_A6 + 0x86)
                ;
                *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) >> 1;
                if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                if (*(char *)(unaff_A6 + 0x1b) == '\f') goto LAB_000c2214;
                bVar11 = candidate_c0fea();
                uVar13 = (undefined3)(unaff_D7 >> 8);
                if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) == 0) goto LAB_000c28d8;
                if (*(char *)(unaff_A6 + 0x26) == '\x02') break;
                unaff_A6[0x14] = 0x2d0aa8;
                if (*(char *)(unaff_A6 + 0x26) == '\x01') {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                  candidate_c0f32();
                  *unaff_A6 = 0xc27fa;
                  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                  if (*(char *)(unaff_A6 + 0x26) == '\x01') {
                    bVar11 = candidate_c0fea();
                    unaff_D7 = CONCAT31(uVar13,bVar11) & 0xffffff0c;
                    if ((bVar11 & 0xc) == 0) goto LAB_000c1f4a;
                    if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) {
                      if ((bVar11 & 0x20) != 0) goto LAB_000c2654;
                      if ((bVar11 & 2) != 0) goto LAB_000c2214;
                      unaff_D7 = 0x30000;
                      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
                        unaff_D7 = 0xfffd0000;
                      }
                      *(uint *)((int)unaff_A6 + 0x62) = unaff_D7;
                      candidate_14594();
                      candidate_427a();
                      goto LAB_000c32f4;
                    }
                  }
                }
                else {
                  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
                  if (*(char *)(unaff_A6 + 0x26) != '\x02') {
                    candidate_c0f32();
                    candidate_c0f16();
                    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
                      *(undefined4 *)((int)unaff_A6 + 0x62) = 0x30000;
                    }
                    else {
                      *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd0000;
                    }
                    *unaff_A6 = 0xc27a8;
                    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                    uVar15 = 0;
                    if (*(char *)(unaff_A6 + 0x26) == '\0') {
                      uVar6 = candidate_c0fea();
                      if ((uVar6 & 0x20) != 0) goto LAB_000c2642;
                      if ((uVar6 & 2) != 0) goto LAB_000c2214;
                      candidate_14594();
                      candidate_427a();
                      if ((bool)uVar15) {
                        *unaff_A6 = 0xc285c;
                      }
                      goto LAB_000c32f4;
                    }
                  }
                }
              }
              *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
LAB_000c28d8:
              unaff_D7 = CONCAT31(uVar13,bVar11) & 0xffffff0c;
              if ((bVar11 & 0xc) == 0) {
                if (*(short *)(unaff_A6 + 0x20) != 0) goto LAB_000c1f4a;
                *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
                unaff_A6[0x14] = 0x2d0aa8;
                if (('\0' < *(char *)((int)unaff_A6 + 0x92)) ||
                   (*(undefined4 *)((int)unaff_A6 + 0x62) = 0, *(char *)(unaff_A6 + 0x26) != '\0'))
                goto LAB_000c1f4a;
                goto LAB_000c1f24;
              }
              if ((bVar11 & 0x20) != 0) {
LAB_000c2642:
                candidate_c0f32();
                *(undefined1 *)(unaff_A6 + 0x24) = 3;
                goto LAB_000c266e;
              }
              if ((bVar11 & 2) != 0) goto LAB_000c2214;
              if (((*(char *)((int)unaff_A6 + 0x9d) == '\0') ||
                  ((*(byte *)((int)unaff_A6 + 0x9a) & 1) == 0)) || ((bVar11 & 0x10) == 0)) {
                iVar8 = 0x30000;
                if (*(char *)((int)unaff_A6 + 0x93) == '\x02') {
                  iVar8 = 0x28000;
                }
                if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
                  iVar8 = -iVar8;
                }
                *(int *)((int)unaff_A6 + 0x62) = iVar8;
                candidate_14594();
                candidate_427a();
                goto LAB_000c32f4;
              }
            } while( true );
          }
          if ((*(byte *)((int)unaff_A6 + 0x9a) & 0x20) == 0) {
            bVar11 = candidate_c0fea();
            uVar15 = 0;
            if ((bVar11 & 0x51) != 0) goto LAB_000c1f4a;
          }
          else {
            bVar11 = candidate_c0fea();
            uVar15 = false;
            if ((bVar11 & 0x51) == 0) {
              candidate_18f24();
              if ((bool)uVar15) {
                bVar11 = candidate_18ffe();
                bVar16 = false;
                if (((*(byte *)((int)unaff_A6 + 0x5e) & bVar11) != 0) && (candidate_c0f96(), bVar16)
                   ) {
                  *unaff_A6 = candidate_c1e84;
                }
              }
              bVar16 = *(byte *)(unaff_A6 + 0x1b) < 2;
              if ((*(byte *)(unaff_A6 + 0x1b) == 2) && (candidate_c0f96(), bVar16)) {
                bVar16 = (*(byte *)((int)unaff_A6 + 0x15) & 1) != 0 &&
                         CONCAT22(extraout_D0u,0x10) != 0;
                candidate_14850();
                if (!bVar16) {
                  *unaff_A6 = candidate_c1eaa;
                }
              }
              bVar11 = (*(byte *)((int)unaff_A6 + 0x96) + 1) -
                       (0xfe < *(byte *)((int)unaff_A6 + 0x96));
              *(byte *)((int)unaff_A6 + 0x96) = bVar11;
              if ((0xf < bVar11) &&
                 (uVar9 = candidate_29c6(), uVar9 < *(byte *)((int)unaff_A6 + 0x96))) {
                *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
                uVar15 = false;
                candidate_c0f96();
                if ((bool)uVar15) {
                  uVar9 = candidate_29c6();
                  uVar15 = uVar9 < 0x4000;
                  if ((bool)uVar15) {
                    *unaff_A6 = candidate_c1e54;
                    goto LAB_000c20ec;
                  }
                }
                candidate_c0f96();
                if ((bool)uVar15) {
                  *unaff_A6 = candidate_c1e44;
                }
              }
LAB_000c20ec:
              sVar10 = candidate_191fc();
              uVar15 = 0;
              if (sVar10 != 0) {
                uVar15 = sVar10 == 0;
                if (sVar10 == 1) {
                  candidate_c0f96();
                  if ((bool)uVar15) {
                    uVar9 = candidate_29c6();
                    uVar15 = uVar9 < 0x100;
                    if ((bool)uVar15) {
                      *unaff_A6 = candidate_c1e64;
                      uVar15 = 0;
                    }
                  }
                }
                else {
                  candidate_c0f96();
                  if ((bool)uVar15) {
                    uVar9 = candidate_29c6();
                    uVar15 = uVar9 < 0x100;
                    if ((bool)uVar15) {
                      *unaff_A6 = candidate_c1e74;
                      uVar15 = 0;
                    }
                  }
                }
              }
            }
          }
          candidate_14594();
          candidate_427a();
          if ((bool)uVar15) {
            *unaff_A6 = 0xc1f4a;
          }
          goto LAB_000c32f4;
        }
      }
LAB_000c2214:
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      unaff_A6[0x14] = 0x2d0ab4;
      candidate_c0f32();
      candidate_c0f16();
      *unaff_A6 = 0xc2236;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
      uVar6 = candidate_c0fea();
      uVar15 = *(byte *)(unaff_A6 + 0x1b) < 0xc;
      if ((*(byte *)(unaff_A6 + 0x1b) != 0xc) && ((uVar6 & 0x20) != 0)) goto LAB_000c2654;
      if ((uVar6 & 0x10) == 0) {
        if ((uVar6 & 0x40) == 0) {
          candidate_14594();
          candidate_427a();
          if ((bool)uVar15) {
            *unaff_A6 = 0xc22a4;
          }
          goto LAB_000c32f4;
        }
LAB_000c2458:
        *(undefined1 *)(unaff_A6 + 0x20) = 0;
        candidate_c0f32();
      }
      else {
LAB_000c246a:
        *(undefined1 *)(unaff_A6 + 0x20) = 1;
        if ((*(byte *)((int)unaff_A6 + 0x9a) & 1) == 0) {
          bVar11 = *(byte *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x1c));
          if (0xf < bVar11) {
            __m68k_trap(0xf);
          }
          unaff_D7 = (uint)bVar11 << 2;
        }
        else if (*(char *)((int)unaff_A6 + 0x9d) == '\0') {
          *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) ^ 2;
        }
        else {
          *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) ^ 2;
        }
        candidate_c0f32();
        *(undefined1 *)((int)unaff_A6 + 0x81) = 0xff;
        cVar12 = *(char *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x1c));
        if ((cVar12 == '\x05') || (cVar12 == '\x06')) {
          *(undefined1 *)((int)unaff_A6 + 0x81) = 4;
        }
        else if ((cVar12 == '\n') || (cVar12 == '\v')) {
          *(undefined1 *)((int)unaff_A6 + 0x81) = 4;
        }
      }
      *(byte *)((int)unaff_A6 + 0x9b) = *(byte *)((int)unaff_A6 + 0x9b) & 0xef;
      *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) & 0xef;
      *(byte *)((int)unaff_A6 + 0x9b) = *(byte *)((int)unaff_A6 + 0x9b) & 0xbf;
      *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) & 0xbf;
      candidate_c0f16();
      *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
      *unaff_A6 = 0xc2588;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c2706;
      uVar6 = candidate_c0fea();
      if (*(char *)(unaff_A6 + 0x1b) == '\f') {
LAB_000c25b0:
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        uVar15 = 0;
        candidate_14594();
        candidate_427a();
        if ((bool)uVar15) {
          if (*(char *)(unaff_A6 + 0x20) == '\0') {
            *unaff_A6 = candidate_c2294;
          }
          else {
            *unaff_A6 = candidate_c229c;
          }
        }
        if (*(char *)((int)unaff_A6 + 0x76) != '\0') {
          if ((*(byte *)((int)unaff_A6 + 0x9b) & 0x10) != 0) {
            *unaff_A6 = 0xc246a;
          }
          if ((*(byte *)((int)unaff_A6 + 0x9b) & 0x40) != 0) {
            *unaff_A6 = 0xc2458;
          }
          if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) {
            *unaff_A6 = 0xc2338;
          }
        }
        cVar12 = *(char *)((int)unaff_A6 + 0x81);
        if (-1 < cVar12) {
          if (cVar12 == '\0') {
            if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) {
              *unaff_A6 = 0xc2338;
            }
          }
          else {
            *(char *)((int)unaff_A6 + 0x81) = cVar12 + -1;
          }
        }
        goto LAB_000c32f4;
      }
      if ((uVar6 & 2) == 0) {
LAB_000c1e34:
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
LAB_000c1f24:
        *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
        candidate_c0f32();
        candidate_c0f16();
        *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0xdf;
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0xc1fa6;
        goto LAB_000c1fa6;
      }
      if ((uVar6 & 0x20) == 0) goto LAB_000c25b0;
LAB_000c2654:
      unaff_A6[0xc] = 0;
      candidate_c0f32();
      unaff_A6[0x14] = 0x2d0ac0;
      *(undefined1 *)(unaff_A6 + 0x24) = 2;
LAB_000c266e:
      if (*(char *)(unaff_A6 + 0x1b) == '\r') {
        unaff_A6[0xd] = unaff_A6[0xd] + 0xa9000;
      }
      else {
        unaff_A6[0xd] = 0xa9000;
      }
      *unaff_A6 = 0xc2692;
      goto LAB_000c2692;
    }
    if (cVar12 == '\x05') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      goto LAB_000c1f4a;
    }
    if (cVar12 != '\x04') goto LAB_000c3232;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x87f4;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x87f4;
    candidate_9d1e();
    candidate_c0fb6();
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0xc0000;
    unaff_A6[0x20] = 0x100000;
    *unaff_A6 = 0xc31b8;
    candidate_aa4c8();
    unaff_A6[0x20] = (-0x180000 - unaff_A6[0x20] >> 4) + unaff_A6[0x20];
    unaff_A6[0xd] = ((int)(unaff_A6[0x20] - unaff_A6[0xd]) >> 4) + unaff_A6[0xd];
    candidate_143ee();
    candidate_427a();
    if ((int)unaff_A6[0xd] < -0x3ffff) {
      *unaff_A6 = candidate_c3214;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_c3214;
    }
    *(ushort *)(unaff_A6 + 0x21) = *(ushort *)(unaff_A6 + 0x21) | 6;
    goto LAB_000c32f4;
  }
  candidate_19424();
  if (bVar16) {
LAB_000c3232:
    candidate_c0f32();
    candidate_c0f16();
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0xb7000;
    *unaff_A6 = 0xc3250;
    candidate_aa4c8();
    *(ushort *)(unaff_A6 + 0x21) = *(ushort *)(unaff_A6 + 0x21) | 0x1e;
    candidate_c0fea();
    uVar4 = -0x160000 - unaff_A6[0xd] >> 5;
    uVar6 = unaff_A6[0xd];
    bVar16 = CARRY4(uVar4,uVar6);
    unaff_A6[0xd] = uVar4 + uVar6;
    candidate_19424();
    if (!bVar16) {
      uVar6 = candidate_c0fea();
      iVar14 = 0;
      iVar8 = 5;
      if ((uVar6 & 4) != 0) {
        iVar14 = -0x40000;
        iVar8 = 2;
      }
      if ((uVar6 & 8) != 0) {
        iVar14 = 0x40000;
        iVar8 = 2;
      }
      unaff_D7 = iVar14 - unaff_A6[0xc] >> iVar8;
      unaff_A6[0xc] = unaff_D7 + unaff_A6[0xc];
    }
    candidate_c109c();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_c1ebe;
    }
    goto LAB_000c32f4;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = extraout_D1b;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0x7f;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_120f8();
  unaff_A6[0x14] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ec;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ec;
  if (*(char *)((int)unaff_A6 + 0x93) == '\x02') {
    candidate_c0fb6();
    candidate_c0f16();
    *unaff_A6 = 0xc2a48;
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
    uVar6 = unaff_A6[0xd];
    uVar15 = uVar6 < (uint)((int)unaff_A6[0xd] >> 5);
    unaff_A6[0xd] = uVar6 - ((int)unaff_A6[0xd] >> 5);
    candidate_143ee();
    candidate_427a();
    if ((bool)uVar15) goto LAB_000c2f90;
    goto LAB_000c2f4a;
  }
  if (*(char *)((int)unaff_A6 + 0x93) == '\x03') {
LAB_000c2c40:
    candidate_9f88();
    candidate_9ea8();
    candidate_9ea8();
LAB_000c2cb0:
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_c0f32();
    candidate_c0f16();
    *(undefined2 *)(unaff_A6 + 0x20) = 0x80;
    goto LAB_000c2e44;
  }
  switch(*(undefined1 *)((int)unaff_A6 + 0x5a)) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    candidate_9f88();
    candidate_9ea8();
    candidate_40f38();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      unaff_A6[0xc] = 0;
      unaff_A6[0xd] = 0;
    }
    candidate_c0f32();
    candidate_c0f16();
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    *(undefined2 *)(unaff_A6 + 0x20) = 0x100;
    *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
    goto LAB_000c2e84;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    break;
  case 7:
    candidate_9f88();
    candidate_9ea8();
    candidate_9ea8();
    goto LAB_000c2cb0;
  case 8:
    candidate_9f88();
    if ((&DAT_0010c30c)[*(short *)(unaff_A6 + 0x1c)] == '\0') {
      candidate_9ea8();
    }
    else {
      candidate_9ea8();
    }
    candidate_9ea8();
    candidate_9ea8();
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_c0f32();
    candidate_c0f16();
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
      unaff_A6[0xc] = 0xfffe0000;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      unaff_A6[0xc] = 0x20000;
    }
    unaff_A6[0xd] = 0xa9000;
    *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
    *unaff_A6 = candidate_c2b8a;
    unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
    uVar6 = unaff_A6[0xc];
    uVar15 = uVar6 < (uint)((int)unaff_A6[0xc] >> 6);
    unaff_A6[0xc] = uVar6 - ((int)unaff_A6[0xc] >> 6);
    candidate_143ee();
    candidate_427a();
    if ((bool)uVar15) {
      *unaff_A6 = candidate_c2f06;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *(undefined1 *)(unaff_A6 + 0x1e) = 0;
    }
    goto LAB_000c2f4a;
  case 9:
    break;
  case 10:
    break;
  case 0xb:
    break;
  case 0xc:
    break;
  case 0xd:
    break;
  case 0xe:
    break;
  case 0xf:
    break;
  case 0x10:
    goto LAB_000c2c40;
  case 0x11:
    break;
  case 0x12:
    candidate_40f38();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) break;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    candidate_c0f32();
    candidate_c0f16();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = candidate_c2ab2;
    uVar15 = 0;
    candidate_14594();
    candidate_427a();
    if ((bool)uVar15) {
      *unaff_A6 = candidate_c2f06;
    }
    goto LAB_000c2f4a;
  case 0x13:
  }
  candidate_9f88();
  if ((&DAT_0010c30c)[*(short *)(unaff_A6 + 0x1c)] == '\0') {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  candidate_c0f32();
  candidate_c0f16();
  *(undefined2 *)(unaff_A6 + 0x20) = 0x20;
LAB_000c2e44:
  unaff_A6[0xd] = 0xa9000;
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    unaff_A6[0xc] = 0xfffe0000;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    unaff_A6[0xc] = 0x20000;
  }
  candidate_c4c28();
  *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
LAB_000c2e84:
  *unaff_A6 = 0xc2e8a;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
    candidate_143ee();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    candidate_14594();
  }
  uVar3 = (short)(*(short *)(unaff_A6 + 0x20) - *(short *)((int)unaff_A6 + 0x26)) >> 4;
  uVar9 = *(ushort *)((int)unaff_A6 + 0x26);
  bVar16 = CARRY2(uVar3,uVar9);
  *(ushort *)((int)unaff_A6 + 0x26) = uVar3 + uVar9;
  candidate_427a();
  if (bVar16) {
    *unaff_A6 = candidate_c2f06;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xc] = 0;
    *(undefined2 *)(unaff_A6 + 0x20) = 0x100;
    *(undefined1 *)(unaff_A6 + 0x1e) = 0;
  }
LAB_000c2f4a:
  *(undefined1 *)(unaff_A5 + 0x430f) = 0xff;
  candidate_c8d4e();
  if ((short)unaff_D7 == 0) {
    *(undefined1 *)((int)*(short *)(unaff_A6 + 0x1c) + unaff_A5 + 0x430a) = 0xff;
  }
  candidate_aa4c8();
  if ((*(ushort *)(unaff_A6 + 0xf) < 0x140) &&
     ((ushort)(*(short *)(unaff_A6 + 0x10) - 0xc0U) < 0x200)) {
    return;
  }
LAB_000c2f90:
  *unaff_A6 = 0xc2f96;
  cVar12 = candidate_19028();
  if (cVar12 == '\0') {
    return;
  }
  sVar10 = *(short *)(unaff_A6 + 0x1c);
  *(undefined1 *)(sVar10 + 0x10c718) = 0;
  *(undefined1 *)(sVar10 + 0x10c71e) = 0;
  *(undefined1 *)(sVar10 + 0x10c720) = 0;
  *(undefined2 *)((short)(sVar10 * 2) + 0x10c724) = 0;
  if (DAT_0010c2e7 == '\x01') {
    *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
    *unaff_A6 = 0xc2fe0;
    sVar10 = *(short *)(unaff_A6 + 0x20) + -1;
    *(short *)(unaff_A6 + 0x20) = sVar10;
    if (sVar10 != 0) {
      return;
    }
  }
  uVar9 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
  if (0xfc10 < uVar9) {
    uVar9 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar9 & 0xfff;
    uVar9 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                      0));
    uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar9 ^ uVar5) & 1) != 0) {
      *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
      _DAT_002ffff0 = 1;
      do {
      } while (DAT_002fffff != '\x01');
      uRam00400000 = 0;
      uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
      uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
      uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
      uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
      uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
      uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
      uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
      uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
      uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
      uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
      uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
      uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
      uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
      uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
      uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
      _DAT_00401ffe = 0;
      func_0x0003cf12();
      candidate_3e2a8();
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  candidate_4126();
  iVar8 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar8 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
LAB_000c2706:
  unaff_A6[0xd] = 0xfff00000;
  candidate_143ee();
  unaff_A6[0xd] = 0;
LAB_000c271c:
  unaff_A6[0x14] = 0x2d0aa8;
  *(undefined1 *)((int)unaff_A6 + 0x7a) = 0xff;
  candidate_c0f32();
  *(undefined1 *)(unaff_A6 + 0x24) = 2;
  *(undefined1 *)(unaff_A6 + 0x1e) = 0;
  *unaff_A6 = 0xc2692;
LAB_000c2692:
  candidate_c0fea();
  unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
  uVar6 = candidate_c0fea();
  iVar14 = 0;
  iVar8 = 5;
  if ((uVar6 & 4) != 0) {
    iVar14 = -0x40000;
    iVar8 = 2;
  }
  if ((uVar6 & 8) != 0) {
    iVar14 = 0x40000;
    iVar8 = 2;
  }
  unaff_D7 = iVar14 - unaff_A6[0xc] >> iVar8;
  unaff_A6[0xc] = unaff_D7 + unaff_A6[0xc];
  candidate_c109c();
  *(char *)(unaff_A6 + 0x1e) = -(-2 < *(short *)(unaff_A6 + 0xd));
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_c1ebe;
  }
  goto LAB_000c32f4;
}
